# Quick Release Script for wthrr
# This is the simplest possible way to create a release
# Usage: .\quick-release.ps1 [version]
# If no version provided, auto-increments patch version

param([string]$Version = "")

$ErrorActionPreference = "Stop"

Write-Host "?? wthrr Quick Release" -ForegroundColor Cyan
Write-Host "?????????????????????????" -ForegroundColor Cyan

try {
    if ($Version) {
        & "$PSScriptRoot\release-workflow.ps1" -Version $Version -QuickRelease
    } else {
        & "$PSScriptRoot\release-workflow.ps1" -QuickRelease
    }
} catch {
    Write-Host "? Quick release failed: $_" -ForegroundColor Red
    exit 1
}

Write-Host "?? Quick release completed!" -ForegroundColor Green