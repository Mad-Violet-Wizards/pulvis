import config

def verbose_log(message: str):
    if config.verbose_mode:
        print(f"[VERBOSE] {message}")