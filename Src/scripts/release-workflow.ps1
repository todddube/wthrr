# Simple Release Workflow for wthrr
# Usage: .\release-workflow.ps1 [-Version 0.7.5] [-PreRelease] [-Draft] [-QuickRelease]
# If no version is specified, it will auto-detect from VersionRC.h and increment patch version

[CmdletBinding()]
param(
    [string]$Version = "",
    [switch]$PreRelease,
    [switch]$Draft,
    [switch]$QuickRelease,  # Skip tests and prompts for faster releases
    [switch]$Force          # Force overwrite existing tags/releases
)

$ErrorActionPreference = "Stop"

# Simple status functions
function Write-Step { param([string]$Message) Write-Host "?? $Message" -ForegroundColor Cyan }
function Write-Success { param([string]$Message) Write-Host "? $Message" -ForegroundColor Green }
function Write-Warning { param([string]$Message) Write-Host "??  $Message" -ForegroundColor Yellow }
function Write-Error { param([string]$Message) Write-Host "? $Message" -ForegroundColor Red }

Write-Host @"
???  wthrr Release Workflow - Simple & Fast
?????????????????????????????????????????
"@ -ForegroundColor Magenta

# Step 1: Auto-detect or validate version
Write-Step "Determining version..."

if ([string]::IsNullOrEmpty($Version)) {
    $versionFile = "wthrr/VersionRC.h"
    if (-not (Test-Path $versionFile)) {
        Write-Error "VersionRC.h not found. Please specify -Version parameter."
        exit 1
    }
    
    try {
        $content = Get-Content $versionFile -Raw
        
        # More robust regex patterns with error handling
        $majorMatch = [regex]::Match($content, '#define\s+WTHRR_VERSION_MAJOR\s+(\d+)')
        $minorMatch = [regex]::Match($content, '#define\s+WTHRR_VERSION_MINOR\s+(\d+)')
        $patchMatch = [regex]::Match($content, '#define\s+WTHRR_VERSION_PATCH\s+(\d+)')
        
        if (-not ($majorMatch.Success -and $minorMatch.Success -and $patchMatch.Success)) {
            Write-Error "Could not parse version from VersionRC.h. Please specify -Version parameter."
            exit 1
        }
        
        $major = [int]$majorMatch.Groups[1].Value
        $minor = [int]$minorMatch.Groups[1].Value
        $patch = [int]$patchMatch.Groups[1].Value
        
        # Auto-increment patch version for new release
        $patch++
        $Version = "$major.$minor.$patch"
        
        Write-Success "Auto-detected and incremented version to: v$Version"
    } catch {
        Write-Error "Failed to read version: $_"
        exit 1
    }
} else {
    # Validate provided version format
    if ($Version -notmatch '^\d+\.\d+\.\d+$') {
        Write-Error "Version must be in format X.Y.Z (e.g., 0.7.5)"
        exit 1
    }
    Write-Success "Using specified version: v$Version"
}

$versionParts = $Version.Split('.')
$major = [int]$versionParts[0]
$minor = [int]$versionParts[1] 
$patch = [int]$versionParts[2]

# Step 2: Update version numbers
Write-Step "Updating version numbers..."
try {
    $updateScript = Join-Path $PSScriptRoot "update-version.ps1"
    if (-not (Test-Path $updateScript)) {
        Write-Error "update-version.ps1 not found in scripts directory"
        exit 1
    }
    
    & $updateScript -Major $major -Minor $minor -Patch $patch -Build 0
    Write-Success "Version numbers updated successfully"
} catch {
    Write-Error "Failed to update version numbers: $_"
    exit 1
}

# Step 3: Build the project
Write-Step "Building wthrr (Release configuration)..."

# Find solution file
$solutionFiles = Get-ChildItem -Path "." -Filter "*.sln" -Recurse -Depth 2
if ($solutionFiles.Count -eq 0) {
    Write-Error "No solution file (.sln) found"
    exit 1
}

$solutionFile = $solutionFiles[0].FullName
Write-Host "?? Building: $solutionFile" -ForegroundColor Gray

try {
    # Clean first
    Write-Host "?? Cleaning previous build..." -ForegroundColor Yellow
    & msbuild $solutionFile /t:Clean /p:Configuration=Release /p:Platform=x64 /verbosity:minimal /nologo | Out-Null
    
    # Build release
    Write-Host "?? Building release..." -ForegroundColor Yellow
    & msbuild $solutionFile /p:Configuration=Release /p:Platform=x64 /verbosity:minimal /nologo
    
    if ($LASTEXITCODE -ne 0) { 
        throw "Build failed with exit code $LASTEXITCODE" 
    }
    
    Write-Success "Build completed successfully"
} catch {
    Write-Error "Build failed: $_"
    exit 1
}

# Step 4: Verify build output
Write-Step "Verifying build output..."

$possiblePaths = @(
    "wthrr/x64/Release/wthrr.exe",
    "wthrr/Release/wthrr.exe",
    "x64/Release/wthrr.exe"
)

$builtBinary = $null
foreach ($path in $possiblePaths) {
    if (Test-Path $path) {
        $builtBinary = $path
        break
    }
}

if (-not $builtBinary) {
    Write-Error "Could not find wthrr.exe in expected locations"
    Write-Host "Searched paths:" -ForegroundColor Red
    $possiblePaths | ForEach-Object { Write-Host "  - $_" -ForegroundColor Gray }
    exit 1
}

$fileInfo = Get-Item $builtBinary
$sizeMB = [math]::Round($fileInfo.Length / 1MB, 2)
Write-Success "Build verified: $builtBinary ($sizeMB MB)"

# Step 5: Quick test (unless QuickRelease)
if (-not $QuickRelease) {
    Write-Step "Running quick test..."
    try {
        $process = Start-Process $builtBinary -PassThru -WindowStyle Hidden
        Start-Sleep -Seconds 2
        
        if (-not $process.HasExited) {
            Stop-Process $process -Force -ErrorAction SilentlyContinue
            Write-Success "Executable test passed"
        } else {
            Write-Warning "Executable exited immediately, but continuing with release"
        }
    } catch {
        Write-Warning "Test failed, but continuing with release: $_"
    }
}

# Step 6: Git operations
Write-Step "Preparing Git commit and tag..."

try {
    # Check for uncommitted changes
    $gitStatus = git status --porcelain 2>$null
    if ($gitStatus) {
        Write-Host "?? Committing changes..." -ForegroundColor Yellow
        git add .
        git commit -m "Release v$Version - Update version and build"
        Write-Success "Changes committed"
    }
    
    # Create or update tag
    $tagName = "v$Version"
    $existingTag = git tag -l $tagName 2>$null
    
    if ($existingTag -and $Force) {
        Write-Host "???  Removing existing tag..." -ForegroundColor Yellow
        git tag -d $tagName
        git push origin :refs/tags/$tagName 2>$null
    } elseif ($existingTag) {
        Write-Warning "Tag $tagName already exists. Use -Force to overwrite."
        if (-not $QuickRelease) {
            $response = Read-Host "Continue anyway? (y/N)"
            if ($response -ne 'y' -and $response -ne 'Y') {
                Write-Error "Release cancelled"
                exit 1
            }
        }
    }
    
    # Create new tag
    git tag -a $tagName -m "Release v$Version"
    Write-Success "Git tag created: $tagName"
    
} catch {
    Write-Error "Git operations failed: $_"
    exit 1
}

# Step 7: Create GitHub release
Write-Step "Creating GitHub release..."

try {
    # Check if GitHub CLI is available
    $ghVersion = gh --version 2>$null
    if (-not $ghVersion) {
        Write-Error "GitHub CLI (gh) is not installed. Download from: https://cli.github.com/"
        exit 1
    }
    
    # Check authentication
    $authStatus = gh auth status 2>$null
    if ($LASTEXITCODE -ne 0) {
        Write-Error "GitHub CLI not authenticated. Run: gh auth login"
        exit 1
    }
    
    # Build release command
    $releaseArgs = @(
        "release", "create", $tagName,
        $builtBinary,
        "--title", "wthrr v$Version",
        "--generate-notes"
    )
    
    if ($PreRelease) { $releaseArgs += "--prerelease" }
    if ($Draft) { $releaseArgs += "--draft" }
    
    # Create release
    & gh @releaseArgs
    
    if ($LASTEXITCODE -eq 0) {
        Write-Success "GitHub release created successfully!"
    } else {
        throw "GitHub release creation failed"
    }
    
} catch {
    Write-Error "Failed to create GitHub release: $_"
    Write-Host "You can create it manually at: https://github.com/todddube/wthrr/releases" -ForegroundColor Yellow
    exit 1
}

# Step 8: Push changes
Write-Step "Pushing to GitHub..."
try {
    git push origin main
    git push origin --tags
    Write-Success "Changes pushed to GitHub"
} catch {
    Write-Warning "Failed to push changes: $_"
    Write-Host "Please push manually: git push origin main --tags" -ForegroundColor Yellow
}

# Final summary
Write-Host @"
?? Release Complete!
?????????????????????????????????????????

?? Version: v$Version
?? Type: $(if ($PreRelease) { 'Pre-release' } elseif ($Draft) { 'Draft' } else { 'Release' })
?? Binary: $builtBinary
?? Release: https://github.com/todddube/wthrr/releases/tag/v$Version

? What's next:
  1. Test the release download
  2. Announce to your community
  3. Monitor feedback and downloads

"@ -ForegroundColor Green

Write-Host "?? Thanks for using wthrr release workflow!" -ForegroundColor Magenta