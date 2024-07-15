param(
    [string]$VisualInstanceName
)

$validInstances = @("vs2017", "vs2019", "vs2022")

if (-Not $VisualInstanceName)
{
    Write-Error "[Generate] Please provide instance name required to generate visual studio solution files."
    exit 1
}

if ($validInstances -notcontains $VisualInstanceName)
{
    $validInstancesString = $validInstances -join ", "
    Write-Error "[Generate] Unsupported visual studio version. Please use one of $validInstancesString."
    exit 1
}



Write-Output "[Generate] Starting to generate project files for $VisualInstanceName."
./premake5 $VisualInstanceName --file=../../../premake5.lua
