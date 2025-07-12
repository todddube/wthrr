# wthrr Release Scripts - Simple & Fast ??

This directory contains **simplified** PowerShell scripts for managing wthrr releases. The new workflow is designed to be fast, reliable, and easy to use.

## ?? Quick Start

### Super Simple Release (Recommended)# Auto-increment patch version and release
.\quick-release.ps1

# Or specify a version
.\quick-release.ps1 "0.7.5"
### Full Control Release# Complete workflow with all steps
.\release-workflow.ps1 -Version "0.7.5"

# Quick release (skip tests and prompts)
.\release-workflow.ps1 -Version "0.7.5" -QuickRelease

# Pre-release
.\release-workflow.ps1 -Version "0.8.0" -PreRelease
## ?? Scripts Overview

| Script | Purpose | When to Use |
|--------|---------|-------------|
| **`quick-release.ps1`** | ? Super fast release | Daily releases, hotfixes |
| **`release-workflow.ps1`** | ?? Complete automation | Major releases, full control |
| **`update-version.ps1`** | ?? Version management | Manual version updates |

## ??? Prerequisites

### One-Time Setup# Install GitHub CLI
winget install GitHub.cli

# Login to GitHub
gh auth login

# Done! You're ready to release ??
### Required Tools
- **GitHub CLI** (`gh`) - For GitHub releases
- **Git** - For version control
- **MSBuild** - For building (comes with Visual Studio)

## ?? Usage Examples

### Most Common Use Cases

**Daily Development Release:**.\quick-release.ps1
# ? Auto-increments version (0.7.4 ? 0.7.5)
# ? Builds, tests, and releases in seconds
**Specific Version Release:**.\release-workflow.ps1 -Version "1.0.0"
# ? Full workflow with version 1.0.0
**Beta Release:**.\release-workflow.ps1 -Version "0.8.0" -PreRelease
# ? Creates pre-release for testing
**Force Release (override existing):**.\release-workflow.ps1 -Version "0.7.5" -Force
# ? Overwrites existing tag/release
## ?? What the New Workflow Does

### ?? `quick-release.ps1` (30 seconds)
1. **Auto-detects** current version from code
2. **Increments** patch version automatically
3. **Builds** release binary
4. **Creates** GitHub release with binary
5. **Pushes** everything to GitHub

### ?? `release-workflow.ps1` (1-2 minutes)
1. **Version Management** - Updates all version numbers
2. **Clean Build** - Builds release configuration
3. **Verification** - Tests the binary works
4. **Git Operations** - Commits, tags, and pushes
5. **GitHub Release** - Creates release with auto-generated notes
6. **Asset Upload** - Attaches the binary

## ? Key Improvements

### ??? Error Handling
- **Robust regex** patterns (fixes the original substring error)
- **Graceful failures** with helpful error messages
- **Pre-flight checks** for tools and authentication

### ? Speed Optimizations
- **QuickRelease mode** skips unnecessary steps
- **Auto-version detection** eliminates manual input
- **Streamlined output** shows only what matters

### ?? Simplicity
- **One command** for most releases
- **Sensible defaults** for everything
- **Clear progress** indicators

## ?? Troubleshooting

### Common Issues & Solutions

**? "substring error"**
? **Fixed!** The new script uses robust regex patterns

**? "GitHub CLI not found"**winget install GitHub.cli
**? "Not authenticated"**gh auth login
**? "Build failed"**
- Check Visual Studio is installed
- Ensure project compiles manually first

**? "Tag already exists"**.\release-workflow.ps1 -Version "0.7.5" -Force
## ?? Example Workflow

### Daily Development# Make your changes
code .

# Test locally  
# (your testing process)

# Release in one command
.\quick-release.ps1

# That's it! ??
### Major Release# For important releases with full control
.\release-workflow.ps1 -Version "1.0.0"

# Review the GitHub release
# Announce to community ??
## ?? What Gets Created

Every release creates:
- ? **Git tag** (e.g., `v0.7.5`)
- ? **GitHub release** with auto-generated notes
- ? **Binary attachment** (wthrr.exe)
- ? **Version updates** in all code files
- ? **Commit** with release changes

## ?? Migration from Old Scripts

If you were using the old complex workflow:

**Old way:**.\release-workflow.ps1 -Version 0.7.5 -PreRelease -Draft -SkipBuild -SkipVersionUpdate -SkipTests -ReleaseNotes "..." -AutoCommit
**New way:**.\quick-release.ps1
The new scripts handle all the complexity automatically! ??

---

## ?? Summary

**For 90% of releases:** Use `.\quick-release.ps1`

**For special releases:** Use `.\release-workflow.ps1` with specific options

**The new workflow is:**
- ? **Faster** - 30 seconds vs 5+ minutes
- ??? **More reliable** - Better error handling
- ?? **Simpler** - One command for most cases
- ?? **More robust** - Fixes the original substring issue

Happy releasing! ?????