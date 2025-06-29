# wthrr

A Windows desktop rain and snow simulation application that creates an ASMR-inspired visual experience with realistic weather effects falling across your desktop.

## Features

- **Realistic Physics**: Raindrops and snowflakes with natural falling patterns and wind effects
- **Multi-Monitor Support**: Seamlessly works across multiple displays
- **Customizable Weather**: Adjustable particle count, wind direction, lightning effects, and colors
- **Desktop Integration**: Particles interact with taskbar and window boundaries
- **High Performance**: Hardware-accelerated DirectX rendering with efficient particle systems

## Requirements

- Windows 10/11
- DirectX 11.2 compatible graphics card

## Installation

Download the latest installer from the [Releases](../../releases) page and run the MSI package.

## Usage

1. Launch wthrr from the Start Menu or desktop shortcut
2. Right-click the system tray icon to access settings
3. Configure rain/snow intensity, wind effects, lightning, and particle colors
4. The application runs in the background, creating weather effects on your desktop

## Building from Source

### Prerequisites
- Visual Studio 2022 with C++20 support
- Windows SDK 10.0.19041.0 or later

### Build Steps
```bash
# Clone the repository
git clone https://github.com/yourusername/wthrr.git
cd wthrr

# Open solution in Visual Studio
start wthrr.sln

# Or build from command line
msbuild wthrr.sln /p:Configuration=Release /p:Platform=x64
```

The built executable will be located at `Src/wthrr/x64/Release/wthrr.exe`

## Architecture

Built with modern C++20 featuring:
- **Template-based particle systems** with concept-driven design
- **RAII resource management** for DirectX objects
- **Multi-threaded rendering** with parallel particle updates
- **DirectComposition integration** for seamless desktop layering

## Contributing

Contributions are welcome! Please see the development documentation in `Src/wthrr/CLAUDE.md` for technical details and coding standards.

## License

Copyright (C) 2024 RYF TOOLS. All rights reserved.

---

*Creating immersive desktop weather experiences since 2024* ☔❄️
