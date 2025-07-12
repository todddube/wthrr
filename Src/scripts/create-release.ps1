# GitHub Release Automation Script for wthrr
# This script creates a GitHub release with proper tagging and asset uploading
# Requires GitHub CLI (gh) to be installed and authenticated

[CmdletBinding()]
param(
    [Parameter(Mandatory=$false)]
    [string]$Version = "",
    
    [string]$ReleaseNotes = "",
    
    [switch]$PreRelease,
    
    [switch]$Draft,
    
    [string]$BinaryPath = "wthrr/x64/Release/wthrr.exe",
    
    [string]$AlternateBinaryPath = "wthrr/Release/wthrr.exe",
    
    [switch]$AutoDetectVersion,
    
    [switch]$Force,
    
    [string]$MSIPath = "",
    
    [switch]$CreateMSI,
    
    [switch]$SkipBuild
)

$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

# ASCII Art Header
Write-Host @"
????????????????????????????????????????????????????????????????
?                    wthrr Release Manager                     ?
?        Creating immersive desktop weather experiences        ?
????????????????????????????????????????????????????????????????
"@ -ForegroundColor Cyan

function Write-Status {
    param([string]$Message, [string]$Color = "White")
    Write-Host "?? $Message" -ForegroundColor $Color
}

function Write-Success {
    param([string]$Message)
    Write-Host "? $Message" -ForegroundColor Green
}

function Write-Warning {
    param([string]$Message)
    Write-Host "??  $Message" -ForegroundColor Yellow
}

function Write-Error {
    param([string]$Message)
    Write-Host "? $Message" -ForegroundColor Red
}

# Auto-detect version from VersionRC.h if not provided
if ($AutoDetectVersion -or [string]::IsNullOrEmpty($Version)) {
    Write-Status "Auto-detecting version from VersionRC.h..."
    
    $versionFile = "wthrr/VersionRC.h"
    if (Test-Path $versionFile) {
        $content = Get-Content $versionFile
        $major = ($content | Select-String "#define WTHRR_VERSION_MAJOR\s+(\d+)").Matches[0].Groups[1].Value
        $minor = ($content | Select-String "#define WTHRR_VERSION_MINOR\s+(\d+)").Matches[0].Groups[1].Value
        $patch = ($content | Select-String "#define WTHRR_VERSION_PATCH\s+(\d+)").Matches[0].Groups[1].Value
        
        if ($major -and $minor -and $patch) {
            $Version = "$major.$minor.$patch"
            Write-Success "Detected version: v$Version"
        } else {
            Write-Error "Could not parse version from VersionRC.h"
            exit 1
        }
    } else {
        Write-Error "VersionRC.h not found at $versionFile"
        exit 1
    }
}

# Validate version format
if ($Version -notmatch '^\d+\.\d+\.\d+$') {
    Write-Error "Version must be in format X.Y.Z (e.g., 0.7.4)"
    exit 1
}

$tagName = "v$Version"
$releaseName = "wthrr v$Version"

Write-Status "Preparing GitHub release for $releaseName" "Cyan"

# Check prerequisites
Write-Status "Checking prerequisites..."

# Check if gh CLI is available
try {
    $ghVersion = gh --version 2>$null
    Write-Success "GitHub CLI found: $($ghVersion[0])"
} catch {
    Write-Error "GitHub CLI (gh) is not installed. Please install it from https://cli.github.com/"
    exit 1
}

# Check if we're in a git repository
if (-not (Test-Path ".git")) {
    Write-Error "Not in a git repository"
    exit 1
}

# Check authentication
try {
    $user =