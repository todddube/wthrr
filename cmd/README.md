# Command Line Tools

[![Release](https://github.com/todddube/wthrr/actions/workflows/release.yml/badge.svg)](https://github.com/todddube/wthrr/actions/workflows/release.yml)
[![Go Version](https://img.shields.io/github/go-mod/go-version/todddube/wthrr)](https://go.dev/)
[![License](https://img.shields.io/github/license/todddube/wthrr)](https://github.com/todddube/wthrr/blob/main/LICENSE)

This directory contains the command-line applications for the wthrr weather simulation project.

## Contents

### `wthrr/`
The main command-line application for the wthrr weather simulation system.

**Purpose**: Provides a cross-platform Go implementation of the weather effects simulation, complementing the existing Windows C++ application.

**Features**:
- Cross-platform compatibility (Linux, macOS, Windows)
- Lightweight command-line interface
- Version information and basic weather simulation logic
- Built for distribution via GitHub Releases

## Usage

### Installation

Download the latest release for your platform from the [Releases page](https://github.com/todddube/wthrr/releases).

**Linux/macOS**:
```bash
# Extract the archive
tar -xzf wthrr_v1.0.0_linux_amd64.tar.gz

# Run the application
./wthrr
```

**Windows**:
```cmd
# Extract the zip file
# Run the application
wthrr.exe
```

### Building from Source

```bash
# Install Go 1.21 or later
# Clone the repository
git clone https://github.com/todddube/wthrr.git
cd wthrr

# Build the application
go build -o wthrr ./cmd/wthrr

# Run
./wthrr
```

### Commands

```bash
# Run the application
./wthrr

# Show version information
./wthrr version
```

## Development

### Project Structure
- `main.go` - Entry point and command-line interface
- Built with Go 1.21+
- Cross-platform support for Linux, macOS, and Windows (amd64)

### Release Process
Releases are automated via GitHub Actions when tags matching `v*` are pushed:

1. Builds binaries for all supported platforms
2. Creates GitHub Release with downloadable assets
3. Archives are created in platform-appropriate formats (tar.gz for Unix, zip for Windows)

### Adding New Commands
To add new command-line functionality:

1. Extend the argument parsing in `main.go`
2. Add new functions for command logic
3. Update this README with new usage examples
4. Test across all supported platforms

## Related Projects

- **Windows C++ Application**: Located in `Src/wthrr/` - Full-featured Windows desktop application with DirectX rendering
- **Installers**: Located in `Src/WthrrInstallerMSI*` - Windows installer packages

This Go application serves as a lightweight, cross-platform companion to the main Windows application, enabling wthrr functionality on additional operating systems.