# PowerShell script to update version numbers - Single Source System (Option 2)
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

# File paths - Single VersionRC.h approach
$versionRCPath = "../wthrr/VersionRC.h"
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
Write-Host "   Using Option 2: Single VersionRC.h system" -ForegroundColor Cyan

# Create VersionRC.h content directly with proper UTF-8 encoding and CRLF line endings
$versionRCContent = @"
// VersionRC.h - Single Source of Truth for All Version Information
// This file contains only C preprocessor definitions compatible with both 
// the resource compiler and C++ code

#ifndef WTHRR_VERSION_RC_H
#define WTHRR_VERSION_RC_H

// Version number components - UPDATE THESE FOR NEW RELEASES
#define WTHRR_VERSION_MAJOR     $Major
#define WTHRR_VERSION_MINOR     $Minor
#define WTHRR_VERSION_PATCH     $Patch
#define WTHRR_VERSION_BUILD     $Build

// Derived version strings - AUTOMATICALLY MAINTAINED BY SCRIPTS
#define WTHRR_VERSION_STRING    "$versionString"
#define WTHRR_VERSION_FULL      "$fullVersionString"

// Application information
#define WTHRR_APP_NAME          "wthrr"
#define WTHRR_COMPANY_NAME      "Todd Dube"
#define WTHRR_DESCRIPTION       "Weather simulation for Windows"
#define WTHRR_COPYRIGHT         "Copyright (C) 2025 Todd Dube"

// Dialog caption with version - AUTOMATICALLY MAINTAINED BY SCRIPTS
#define WTHRR_DIALOG_CAPTION    "$dialogCaption"

#endif // WTHRR_VERSION_RC_H
"@

# Ensure proper CRLF line endings for Windows RC compiler compatibility
$versionRCContent = $versionRCContent -replace "`r`n", "`n" -replace "`n", "`r`n"
# Add final CRLF to prevent RC1004 "unexpected end of file" error
$versionRCContent += "`r`n"

# Write the VersionRC.h file with UTF-8 encoding (no BOM for RC compiler compatibility)
[System.IO.File]::WriteAllText((Resolve-Path $versionRCPath), $versionRCContent, [System.Text.UTF8Encoding]::new($false))
Write-Host "? Updated $versionRCPath (single source of truth)" -ForegroundColor Green

# Update README.md version badge
if (Test-Path $readmePath) {
    $readmeContent = Get-Content $readmePath -Raw
    $readmeContent = $readmeContent -replace '\!\[Version\]\(https://img\.shields\.io/badge/Version-[^-]+-green\?style=flat-square\)', "![Version](https://img.shields.io/badge/Version-$versionString-green?style=flat-square)"
    Set-Content $readmePath $readmeContent -NoNewline
    Write-Host "? Updated README.md version badge" -ForegroundColor Green
} else {
    Write-Warning "README.md not found at $readmePath"
}

# Check if Version.h exists and recommend removal
$versionHeaderPath = "../wthrr/Version.h"
if (Test-Path $versionHeaderPath) {
    Write-Host "`n??  Option 2 Implementation Note:" -ForegroundColor Yellow
    Write-Host "   Version.h still exists but is no longer needed" -ForegroundColor Yellow
    Write-Host "   Your code now uses VersionRC.h directly for true single-source versioning" -ForegroundColor Yellow
    Write-Host "   You can safely delete Version.h to complete the migration" -ForegroundColor Yellow
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
if (Test-Path $readmePath) { Write-Host "   • README.md (version badge)" -ForegroundColor White }

Write-Host "`n?? Next steps:" -ForegroundColor Yellow
Write-Host "1. Build and test: msbuild wthrr.sln /p:Configuration=Release" -ForegroundColor White
Write-Host "2. Verify version appears correctly in:" -ForegroundColor White
Write-Host "   - Application file properties (right-click .exe)" -ForegroundColor Gray
Write-Host "   - Options dialog title bar" -ForegroundColor Gray
Write-Host "   - Debug output (if enabled)" -ForegroundColor Gray
Write-Host "3. Optional: Remove Version.h to complete single-source migration" -ForegroundColor White
Write-Host "4. Commit changes: git add . && git commit -m `"Release v$versionString`"" -ForegroundColor White
Write-Host "5. Create tag: git tag -a v$versionString -m `"Release v$versionString`"" -ForegroundColor White
Write-Host "6. Push changes: git push origin main --tags" -ForegroundColor White

Write-Host "`n? Single-Source Benefits:" -ForegroundColor Cyan
Write-Host "   ? No duplicate version definitions" -ForegroundColor White
Write-Host "   ? Works with both C++ code and Resource Compiler" -ForegroundColor White
Write-Host "   ? Automatic dialog caption updates" -ForegroundColor White
Write-Host "   ? Simplified maintenance with this script" -ForegroundColor White
Write-Host "   ? No file encoding issues" -ForegroundColor White
Write-Host "   ? Proper CRLF line endings for RC compiler" -ForegroundColor White