# PowerShell script to update version numbers - Single Source System
# Usage: .\update-version.ps1 -Major 2 -Minor 1 -Patch 0 [-Build 0]

param(
    [Parameter(Mandatory=$true)]
    [int]$Major,
    
    [Parameter(Mandatory=$true)]
    [int]$Minor,
    
    [Parameter(Mandatory=$true)]
    [int]$Patch,
    
    [int]$Build = 0,
    
    [switch]$PreRelease
)

$ErrorActionPreference = "Stop"

# File paths - Single source of truth
$versionRCPath = "wthrr/VersionRC.h"
$versionHeaderPath = "wthrr/Version.h"  # Only for PreRelease flag
$readmePath = "../README.md"

# Validate version numbers
if ($Major -lt 0 -or $Minor -lt 0 -or $Patch -lt 0 -or $Build -lt 0) {
    Write-Error "Version numbers must be non-negative"
    exit 1
}

$versionString = "$Major.$Minor.$Patch"
$fullVersionString = "$Major.$Minor.$Patch.$Build"
$dialogCaption = "wthrr [v $versionString] - options"

Write-Host "?? Updating wthrr to version $versionString" -ForegroundColor Green
Write-Host "   Single-source versioning system" -ForegroundColor Cyan

# Update VersionRC.h (Single Source of Truth)
if (Test-Path $versionRCPath) {
    $content = Get-Content $versionRCPath -Raw
    
    # Update version number components
    $content = $content -replace '#define WTHRR_VERSION_MAJOR\s+\d+', "#define WTHRR_VERSION_MAJOR     $Major"
    $content = $content -replace '#define WTHRR_VERSION_MINOR\s+\d+', "#define WTHRR_VERSION_MINOR     $Minor"
    $content = $content -replace '#define WTHRR_VERSION_PATCH\s+\d+', "#define WTHRR_VERSION_PATCH     $Patch"
    $content = $content -replace '#define WTHRR_VERSION_BUILD\s+\d+', "#define WTHRR_VERSION_BUILD     $Build"
    
    # Update derived version strings
    $content = $content -replace '#define WTHRR_VERSION_STRING\s+"[^"]+"', "#define WTHRR_VERSION_STRING    `"$versionString`""
    $content = $content -replace '#define WTHRR_VERSION_FULL\s+"[^"]+"', "#define WTHRR_VERSION_FULL      `"$fullVersionString`""
    
    # Update dialog caption
    $content = $content -replace '#define WTHRR_DIALOG_CAPTION\s+"[^"]+"', "#define WTHRR_DIALOG_CAPTION    `"$dialogCaption`""
    
    Set-Content $versionRCPath $content -NoNewline
    Write-Host "? Updated $versionRCPath (single source of truth)" -ForegroundColor Green
} else {
    Write-Error "VersionRC.h not found at $versionRCPath"
    exit 1
}

# Update PreRelease flag in Version.h only (if it exists)
if ((Test-Path $versionHeaderPath) -and ($PreRelease.IsPresent -or $PSBoundParameters.ContainsKey('PreRelease'))) {
    $versionContent = Get-Content $versionHeaderPath -Raw
    
    if ($PreRelease) {
        $versionContent = $versionContent -replace 'return false; // Set to true for beta/alpha versions', 'return true; // Set to true for beta/alpha versions'
        Write-Host "? Marked as pre-release version" -ForegroundColor Yellow
    } else {
        $versionContent = $versionContent -replace 'return true; // Set to true for beta/alpha versions', 'return false; // Set to true for beta/alpha versions'
        Write-Host "? Marked as stable release" -ForegroundColor Green
    }
    
    Set-Content $versionHeaderPath $versionContent -NoNewline
    Write-Host "? Updated $versionHeaderPath (PreRelease flag)" -ForegroundColor Green
}

# Update README.md version badge
if (Test-Path $readmePath) {
    $readmeContent = Get-Content $readmePath -Raw
    $readmeContent = $readmeContent -replace '\!\[Version\]\(https://img\.shields\.io/badge/Version-[^-]+-green\?style=flat-square\)', "![Version](https://img.shields.io/badge/Version-$versionString-green?style=flat-square)"
    Set-Content $readmePath $readmeContent -NoNewline
    Write-Host "? Updated README.md version badge" -ForegroundColor Green
} else {
    Write-Warning "README.md not found at $readmePath"
}

# Verify the changes
Write-Host "`n?? Version Summary:" -ForegroundColor Cyan
Write-Host "   Version: $versionString" -ForegroundColor White
Write-Host "   Full Version: $fullVersionString" -ForegroundColor White
Write-Host "   Dialog Caption: $dialogCaption" -ForegroundColor White
Write-Host "   Pre-release: $(if ($PreRelease) { 'Yes' } else { 'No' })" -ForegroundColor White

Write-Host "`n?? Version update complete!" -ForegroundColor Green
Write-Host "?? Files updated:" -ForegroundColor Yellow
Write-Host "   • VersionRC.h (single source of truth)" -ForegroundColor White
if (Test-Path $versionHeaderPath) { Write-Host "   • Version.h (PreRelease flag only)" -ForegroundColor White }
if (Test-Path $readmePath) { Write-Host "   • README.md (version badge)" -ForegroundColor White }

Write-Host "`n?? Next steps:" -ForegroundColor Yellow
Write-Host "1. Build and test: msbuild wthrr.sln /p:Configuration=Release" -ForegroundColor White
Write-Host "2. Verify version appears correctly in application" -ForegroundColor White
Write-Host "3. Commit changes: git add . && git commit -m `"Release v$versionString`"" -ForegroundColor White
Write-Host "4. Create tag: git tag -a v$versionString -m `"Release v$versionString`"" -ForegroundColor White
Write-Host "5. Push changes: git push origin main --tags" -ForegroundColor White