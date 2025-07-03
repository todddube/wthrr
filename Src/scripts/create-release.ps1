# GitHub Release Automation Script
# This script creates a GitHub release with proper tagging
# Requires GitHub CLI (gh) to be installed and authenticated

param(
    [Parameter(Mandatory=$true)]
    [string]$Version,
    
    [string]$ReleaseNotes = "",
    
    [switch]$PreRelease,
    
    [switch]$Draft,
    
    [string]$BinaryPath = "Src/wthrr/x64/Release/wthrr.exe"
)

$ErrorActionPreference = "Stop"

# Validate version format
if ($Version -notmatch '^\d+\.\d+\.\d+$') {
    Write-Error "Version must be in format X.Y.Z (e.g., 2.0.4)"
    exit 1
}

$tagName = "v$Version"
$releaseName = "wthrr v$Version"

Write-Host "Creating GitHub release for $releaseName" -ForegroundColor Green

# Check if gh CLI is available
try {
    gh --version | Out-Null
} catch {
    Write-Error "GitHub CLI (gh) is not installed. Please install it from https://cli.github.com/"
    exit 1
}

# Check if we're in a git repository
if (-not (Test-Path ".git")) {
    Write-Error "Not in a git repository"
    exit 1
}

# Generate release notes if not provided
if (-not $ReleaseNotes) {
    Write-Host "Generating release notes from recent commits..." -ForegroundColor Yellow
    
    $ReleaseNotes = @"
## ?? What's New in v$Version

### ? Features & Improvements
$(git log --oneline --since="30 days ago" --grep="feat:" --grep="add:" --grep="new:" | ForEach-Object { "- $_" } | Out-String)

### ?? Bug Fixes
$(git log --oneline --since="30 days ago" --grep="fix:" --grep="bug:" | ForEach-Object { "- $_" } | Out-String)

### ?? Technical Changes
$(git log --oneline --since="30 days ago" --grep="refactor:" --grep="improve:" --grep="update:" | ForEach-Object { "- $_" } | Out-String)

### ?? Full Changelog
See all changes: [v$Version...main](https://github.com/todddube/wthrr/compare/v$Version...main)

---

## ?? Installation

### Windows Installer
Download the MSI installer from the assets below for the easiest installation experience.

### Portable Executable
Download `wthrr.exe` for a portable version that doesn't require installation.

### System Requirements
- Windows 10 (1903+) / Windows 11
- DirectX 11.2 compatible graphics card
- 50MB available disk space

## ?? Quick Start
1. Install or extract wthrr
2. Launch from Start Menu or run `wthrr.exe`
3. Right-click the system tray icon to configure settings
4. Enjoy immersive desktop weather effects!

## ?? Support
- **Report Issues**: [GitHub Issues](https://github.com/todddube/wthrr/issues)
- **Documentation**: [Project Wiki](https://github.com/todddube/wthrr/wiki)
- **Contact**: Todd@thedubes.com
"@
}

# Check if binary exists
if ($BinaryPath -and (Test-Path $BinaryPath)) {
    $binarySize = (Get-Item $BinaryPath).Length
    $binarySizeMB = [math]::Round($binarySize / 1MB, 2)
    Write-Host "? Found binary at $BinaryPath ($binarySizeMB MB)" -ForegroundColor Green
    $assetArgs = @("--attach", $BinaryPath)
} else {
    Write-Warning "Binary not found at $BinaryPath - creating release without assets"
    $assetArgs = @()
}

# Build command arguments
$ghArgs = @(
    "release", "create", $tagName,
    "--title", $releaseName,
    "--notes", $ReleaseNotes
)

if ($PreRelease) {
    $ghArgs += "--prerelease"
}

if ($Draft) {
    $ghArgs += "--draft"
}

$ghArgs += $assetArgs

try {
    # Create the release
    Write-Host "Creating release..." -ForegroundColor Yellow
    & gh @ghArgs
    
    Write-Host "`n?? Release created successfully!" -ForegroundColor Green
    Write-Host "View your release: https://github.com/todddube/wthrr/releases/tag/$tagName" -ForegroundColor Cyan
    
    # Additional MSI creation suggestion
    Write-Host "`n?? Next Steps:" -ForegroundColor Yellow
    Write-Host "1. Consider creating an MSI installer using WiX Toolset" -ForegroundColor White
    Write-Host "2. Add release to Microsoft Store (if desired)" -ForegroundColor White
    Write-Host "3. Update project documentation" -ForegroundColor White
    Write-Host "4. Announce on social media/forums" -ForegroundColor White
    
} catch {
    Write-Error "Failed to create release: $_"
    exit 1
}