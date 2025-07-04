# wthrr Release Process

This document outlines the complete process for creating and publishing new releases of wthrr.

## ?? Version Management

### Semantic Versioning
wthrr follows [Semantic Versioning](https://semver.org/):
- **MAJOR.MINOR.PATCH** (e.g., 2.0.4)
- **MAJOR**: Breaking changes or major new features
- **MINOR**: New features that are backward compatible
- **PATCH**: Bug fixes and small improvements

### Centralized Version Control
All version information is managed in `/wthrr/Version.h`:
- Update only this file for version changes
- All other files automatically reflect the new version
- Ensures consistency across the entire application

## ?? Release Workflow

### 1. Prepare Release
```powershell
# Update version (example: releasing 2.0.4)
.\scripts\update-version.ps1 -Major 2 -Minor 0 -Patch 4

# For pre-release versions:
.\scripts\update-version.ps1 -Major 2 -Minor 1 -Patch 0 -PreRelease
```

### 2. Build and Test
```powershell
# Build release version
msbuild Src/wthrr.sln /p:Configuration=Release /p:Platform=x64

# Test the application
Src/wthrr/x64/Release/wthrr.exe

# Verify version shows correctly in:
# - About dialog
# - File properties
# - Options dialog caption
```

### 3. Commit and Tag
```powershell
# Commit version changes
git add .
git commit -m "Release v2.0.4"

# Create annotated tag
git tag -a v2.0.4 -m "Release v2.0.4"

# Push to GitHub
git push origin main
git push origin v2.0.4
```

### 4. Create GitHub Release
```powershell
# Automated release creation
.\scripts\create-release.ps1 -Version "2.0.4"

# For pre-release:
.\scripts\create-release.ps1 -Version "2.0.4" -PreRelease

# Manual release (if needed)
gh release create v2.0.4 --title "wthrr v2.0.4" --generate-notes
```

## ?? Automated Releases

### GitHub Actions
The repository includes automated workflows:
- **Trigger**: Push tag matching `v*.*.*`
- **Process**: Build ? Test ? Package ? Release
- **Artifacts**: Portable ZIP + executable

### Manual Trigger
```bash
# Trigger release workflow manually
gh workflow run release.yml -f version=2.0.4 -f prerelease=false
```

## ?? Release Artifacts

### Standard Release
- `wthrr.exe` - Portable executable
- `wthrr-v2.0.4-portable.zip` - Complete package with docs

### Future Enhancements
- **MSI Installer** (using WiX Toolset)
- **Microsoft Store Package** (MSIX)
- **Chocolatey Package**
- **Winget Package**

## ?? Release Checklist

### Pre-Release
- [ ] Update version in `Version.h`
- [ ] Build and test thoroughly
- [ ] Update README.md if needed
- [ ] Check all features work correctly
- [ ] Verify multi-monitor support
- [ ] Test on different Windows versions

### Release
- [ ] Commit version changes
- [ ] Create and push git tag
- [ ] Build release artifacts
- [ ] Create GitHub release
- [ ] Update documentation

### Post-Release
- [ ] Announce on social media
- [ ] Update project website
- [ ] Monitor for bug reports
- [ ] Plan next version features

## ??? Tag Management

### Naming Convention
- **Stable releases**: `v2.0.4`
- **Pre-releases**: `v2.1.0-beta.1`
- **Release candidates**: `v2.1.0-rc.1`

### Tag Operations
```bash
# List all tags
git tag -l

# Delete local tag
git tag -d v2.0.4

# Delete remote tag
git push origin --delete v2.0.4

# Create lightweight tag
git tag v2.0.4

# Create annotated tag (recommended)
git tag -a v2.0.4 -m "Release version 2.0.4"
```

## ?? Tools Required

### Development
- Visual Studio 2022 (C++20 support)
- Windows SDK 10.0.19041.0+
- Git for version control

### Release Automation
- [GitHub CLI](https://cli.github.com/) (`gh`)
- PowerShell 5.1+ or PowerShell Core
- MSBuild (included with Visual Studio)

### Optional
- [WiX Toolset](https://wixtoolset.org/) for MSI creation
- [Advanced Installer](https://www.advancedinstaller.com/) alternative
- [Inno Setup](https://jrsoftware.org/isinfo.php) for installers

## ?? Troubleshooting

### Common Issues
1. **Version mismatch**: Ensure `Version.h` is updated before building
2. **Build failures**: Check Windows SDK version compatibility
3. **Tag conflicts**: Delete and recreate tags if needed
4. **Release upload fails**: Verify GitHub CLI authentication

### Recovery
```bash
# Rollback release
gh release delete v2.0.4 --yes
git tag -d v2.0.4
git push origin --delete v2.0.4

# Fix issues and re-release
# ... make fixes ...
git tag -a v2.0.4 -m "Release v2.0.4 (fixed)"
git push origin v2.0.4
```

## ?? Support
For release process questions, contact Todd@thedubes.com or create an issue in the GitHub repository.