$ErrorActionPreference = "Stop"

$RaylibUrl = "https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip"
$ZipFile = "raylib.zip"
$ExtractPath = "raylib_temp"
$FinalDir = "raylib"

Write-Host "Downloading Raylib 5.0..."
Invoke-WebRequest -Uri $RaylibUrl -OutFile $ZipFile

Write-Host "Extracting..."
Expand-Archive -Path $ZipFile -DestinationPath $ExtractPath -Force

Write-Host "Setting up directory..."
if (Test-Path $FinalDir) { Remove-Item -Recurse -Force $FinalDir }
# Move the inner folder (e.g., raylib-5.0_win64_mingw-w64) to 'raylib'
$InnerFolder = Get-ChildItem -Path $ExtractPath | Select-Object -First 1
Move-Item -Path $InnerFolder.FullName -Destination $FinalDir

# Clean up
Remove-Item -Path $ZipFile -Force
Remove-Item -Recurse -Force $ExtractPath

Write-Host "Raylib setup complete in '$FinalDir'."
