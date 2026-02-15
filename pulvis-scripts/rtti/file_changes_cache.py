import json
import hashlib
import subprocess
from pathlib import Path
from typing import Dict, Optional
from datetime import datetime
import log

class FileChangesCache:
    def __init__(self, cache_file: Path = None):
        if cache_file is None:
            cache_file = Path(__file__).parent / ".rtti_cache.json"
        self.cache_file = cache_file
        self.cache_data = self._load_cache()
    
    def _load_cache(self) -> Dict:
        if self.cache_file.exists():
            try:
                with open(self.cache_file, 'r', encoding='utf-8') as f:
                    return json.load(f)
            except (json.JSONDecodeError, FileNotFoundError):
                pass
        return {
            "version": "1.0",
            "files": {},
            "last_update": datetime.now().isoformat()
        }
    
    def _save_cache(self):
        self.cache_data["last_update"] = datetime.now().isoformat()
        try:
            with open(self.cache_file, 'w', encoding='utf-8') as f:
                json.dump(self.cache_data, f, indent=2, ensure_ascii=False)
        except Exception as e:
            print(f"Warning: Could not save cache file: {e}")
    
    def _get_file_content_hash(self, file_path: Path, content: str) -> str:
        from markers import RTTI_MARKERS
        
        lines = content.split('\n')
        rtti_lines = []
        
        for i, line in enumerate(lines):
            if any(marker in line for marker in RTTI_MARKERS):
                start = max(0, i - 2) 
                end = min(len(lines), i + 10)
                rtti_lines.extend(lines[start:end])
        

        if not rtti_lines:
            return hashlib.sha256(content.encode('utf-8')).hexdigest()[:16]
        
        seen = set()
        unique_rtti_lines = []
        for line in rtti_lines:
            if line not in seen:
                seen.add(line)
                unique_rtti_lines.append(line)
        
        rtti_content = '\n'.join(unique_rtti_lines)
        return hashlib.sha256(rtti_content.encode('utf-8')).hexdigest()[:16]
    
    def _get_git_hash(self, file_path: Path) -> Optional[str]:
        try:
            result = subprocess.run(
                ['git', 'log', '-1', '--format=%H', str(file_path)],
                capture_output=True,
                text=True,
                cwd=file_path.parent,
                timeout=5
            )
            if result.returncode == 0:
                return result.stdout.strip()[:16]
        except (subprocess.SubprocessError, FileNotFoundError):
            pass
        return None
    
    def has_file_changed(self, file_path: Path, content: str) -> bool:
        file_key = str(file_path.absolute())
        
        content_hash = self._get_file_content_hash(file_path, content)
        git_hash = self._get_git_hash(file_path)
        file_mtime = file_path.stat().st_mtime
        
        if file_key not in self.cache_data["files"]:
            log.verbose_log(f"{file_path.name}: NEW FILE (not in cache)")
            return True
        
        cached_info = self.cache_data["files"][file_key]
        
        old_content_hash = cached_info.get("content_hash")
        if old_content_hash != content_hash:
            log.verbose_log(f"{file_path.name}: CONTENT CHANGED (hash: {old_content_hash} -> {content_hash})")
            return True
        
        old_git_hash = cached_info.get("git_hash")
        if git_hash and old_git_hash != git_hash:
            log.verbose_log(f"{file_path.name}: GIT CHANGED (hash: {old_git_hash} -> {git_hash})")
            return True
        
        old_mtime = cached_info.get("mtime", 0)
        if old_mtime < file_mtime:
            log.verbose_log(f"{file_path.name}: MTIME CHANGED ({old_mtime} -> {file_mtime})")
            return True
        
        log.verbose_log(f"{file_path.name}: UNCHANGED")
        return False
    
    def update_file_info(self, file_path: Path, content: str):
        file_key = str(file_path.absolute())
        
        content_hash = self._get_file_content_hash(file_path, content)
        git_hash = self._get_git_hash(file_path)
        mtime = file_path.stat().st_mtime
        
        self.cache_data["files"][file_key] = {
            "content_hash": content_hash,
            "git_hash": git_hash,
            "mtime": mtime,
            "last_generated": datetime.now().isoformat()
        }
        
        log.verbose_log(f"{file_path.name}: UPDATED CACHE (hash: {content_hash})")
    
    def remove_file(self, file_path: Path):
        file_key = str(file_path.absolute())
        if file_key in self.cache_data["files"]:
            del self.cache_data["files"][file_key]
    
    def save(self):
        self._save_cache()
    
    def clear(self):
        self.cache_data = {
            "version": "1.0", 
            "files": {},
            "last_update": datetime.now().isoformat()
        }
        if self.cache_file.exists():
            self.cache_file.unlink()
    
    def get_stats(self) -> dict:
        return {
            "total_files": len(self.cache_data["files"]),
            "last_update": self.cache_data.get("last_update", "Never"),
            "cache_file": str(self.cache_file)
        }

file_changes_cache: FileChangesCache = FileChangesCache()