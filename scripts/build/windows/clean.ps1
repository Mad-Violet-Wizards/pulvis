# Quick desc:
# Script is taking data from clear_keys.txt & deleting the found matching files all accross the project directory.

$keysFilePath = "./clear_keys.txt"
$directoryToClean = "../../../"

$keysToDelete = Get-Content $keysFilePath

if ($keysToDelete.Length -eq 0)
{
    Write-Host "No keys to delete"
    exit
}

foreach($key in $keysToDelete)
{
    # Detect keys with filextensions.
    if ($key -match "^\.")
    {
        $filesToDelete = Get-ChildItem -Path $directoryToClean -Recurse -File | Where-Object { $_.Extension -eq $key }
        
        if ($filesToDelete)
        {
            foreach($file in $filesToDelete)
            {
                try
                {
                    Write-Host "File deleted: ", $file.FullName
                    Remove-Item -Path $file.FullName -Force -ErrorAction Stop
                }
                catch
                {
                    Write-Host "Error deleting file: ", $file.FullName, "Error: ", $_.Exception.Message
                }
            }
        }
        else
        {
            Write-Host "No files found with extension: " $key
        }
    }
    else
    {
        $dirToDelete = Get-ChildItem -Path $directoryToClean -Recurse | Where-Object { $_.Name -eq $key -and $_.PSIsContainer }

        if ($dirToDelete)
        {
            try
            {
                Write-Host "Directory deleted: ", $dirToDelete.FullName
                Remove-Item -Path $dirToDelete.FullName -Force -Recurse -ErrorAction Stop
            }
            catch
            {
                Write-Host "Error deleting directory: ", $dirToDelete.FullName "Error: ", $_.Exception.Message
            }
        }
        else
        {
            Write-Host "No directory found with name: " $key
        }
    }
}