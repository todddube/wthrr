# wthrr ☔❄️

**An immersive Windows desktop weather simulation** that brings realistic rain and snow effects to your screen with stunning visual fidelity and ASMR-inspired atmospheric ambiance.

![wthrr Demo](https://img.shields.io/badge/Platform-Windows%2011+-blue?style=flat-square)
![Version](https://img.shields.io/badge/Version-2.0.3-green?style=flat-square)
![License](https://img.shields.io/badge/License-Copyright%20RYF%20TOOLS-red?style=flat-square)

## ✨ Features

### 🌧️ **Advanced Weather Simulation**
- **Realistic Rain Physics**: Dynamic raindrops with authentic falling patterns, wind interaction, and ground collision
- **Snow Dynamics**: Sophisticated snowflake simulation with realistic settling, wind gusts, and accumulation effects
- **Puddle Formation**: NEW! Raindrops create interactive puddles on the taskbar that evaporate naturally over time
- **Multi-Monitor Support**: Seamless weather effects across all displays with individual configuration

### ⚡ **Dynamic Environmental Effects**
- **Lightning System**: Configurable lightning strikes with adjustable frequency and intensity during rain
- **Advanced Wind Physics**: Realistic wind patterns affecting both rain direction and snow drift
- **Snow Wind Variability**: Dynamic wind gusts with customizable intensity and stability patterns
- **Interactive Ground Collision**: Particles respond naturally to desktop boundaries and taskbar surfaces

### 🎨 **Customization & Visual Polish**
- **Color Customization**: Full RGB color selection for raindrops and snowflakes
- **Particle Density Control**: Adjust from light drizzle to heavy downpour/blizzard
- **Performance Optimization**: Hardware-accelerated DirectX 11.2 rendering with 60 FPS targeting
- **Desktop Integration**: Non-intrusive overlay that respects window z-ordering and system interactions

### 🖥️ **Modern Windows Integration**
- **System Tray Management**: Clean, minimal interface with context menu access
- **DPI Awareness**: Per-monitor DPI scaling support for crisp visuals on high-resolution displays
- **Composition Engine**: Leverages Windows Desktop Composition for smooth, tear-free rendering
- **Resource Efficiency**: Intelligent CPU usage management with adaptive frame limiting

## 🔧 Technical Excellence

Built with **modern C++20** standards and cutting-edge Windows APIs:

| Component | Technology |
|-----------|------------|
| **Rendering** | DirectX 11.2 + Direct2D + DirectComposition |
| **Physics** | Custom particle system with FastNoiseLite integration |
| **Performance** | Multi-threaded updates with fixed timestep simulation |
| **Architecture** | RAII resource management with template-driven design |
| **Standards** | C++20 with concepts, constexpr, and modern STL features |

## 📋 System Requirements

| Requirement | Specification |
|-------------|---------------|
| **OS** | Windows 11 or higher |
| **Graphics** | DirectX 11.2 compatible GPU |
| **Memory** | 50MB RAM typical usage |
| **CPU** | Any modern processor (Intel/AMD) |
| **Display** | Any resolution, multi-monitor supported |

## 🚀 Quick Start

### Option 1: Download Release (Recommended)
1. Visit our [Releases Page](../../releases)
2. Download the latest `wthrr-v2.0.3-setup.msi`
3. Run installer with administrator privileges
4. Launch from Start Menu or desktop shortcut

### Option 2: Build from Source
# Prerequisites: Visual Studio 2022 with C++20 support and Windows 11 SDK (22000.0+)
git clone https://github.com/yourusername/wthrr.git
cd wthrr/Src
msbuild wthrr.sln /p:Configuration=Release /p:Platform=x64
## 🎮 Usage Guide

### **Basic Operation**
1. **Launch** wthrr from Start Menu
2. **Configure** via system tray icon right-click → "Configure"
3. **Customize** weather type, intensity, and visual effects
4. **Enjoy** immersive desktop weather simulation

### **Configuration Options**

| Setting | Description | Range |
|---------|-------------|-------|
| **Weather Type** | Rain or Snow mode | Toggle |
| **Particle Count** | Density of weather effects | Low → High |
| **Wind Direction** | Horizontal weather movement | Left ← → Right |
| **Lightning Frequency** | Storm intensity (Rain only) | Rare → Frequent |
| **Lightning Intensity** | Flash brightness | Dim → Bright |
| **Snow Wind System** | Dynamic wind for snow | Enable/Disable |
| **Wind Intensity** | Snow drift strength | Mild → Strong |
| **Wind Variability** | Gust frequency | Stable → Gusty |
| **Color Selection** | Custom particle colors | Full RGB Spectrum |

## 🏗️ Architecture Deep Dive

### **Core Systems**┌─ DisplayWindow ─────────┐
│  ├─ Particle Manager   │  ← Rain/Snow Systems
│  ├─ PuddleManager      │  ← NEW! Puddle Physics  
│  ├─ Lightning System   │  ← Environmental Effects
│  └─ Wind Physics       │  ← Dynamic Weather
└─────────────────────────┘
         │
    DirectX Pipeline
         │
┌─ Hardware Acceleration ─┐
│  ├─ D3D11 Device       │
│  ├─ D2D Context        │
│  └─ DComposition       │
└─────────────────────────┘
### **Key Innovations**
- **Puddle System**: Real-time water accumulation with ripple effects and natural evaporation
- **Wind Dynamics**: Procedural wind generation using FastNoiseLite for organic movement patterns  
- **Z-Order Management**: Intelligent window layering that keeps effects behind active applications
- **Multi-Monitor Awareness**: Independent weather simulation per display with shared configuration

## 🛠️ Development

### **Project Structure**wthrr/
├── Src/wthrr/           # Main application source
│   ├── Puddle.cpp       # NEW! Puddle physics system
│   ├── DisplayWindow.cpp # Core rendering and window management
│   ├── RainDrop.cpp     # Rain particle physics
│   ├── SnowFlake.cpp    # Snow particle system
│   ├── MathUtil.cpp     # Mathematical utilities
│   └── ...
├── Resources/           # Icons, version info
└── Build/              # Output directory
### **Coding Standards**
- **Modern C++20** with concepts and constexpr
- **RAII** for all resource management
- **Template-driven** particle systems
- **noexcept** specifications for performance-critical paths
- **[[nodiscard]]** attributes for error handling

## 🤝 Contributing

We welcome contributions! Please:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/amazing-enhancement`)
3. **Follow** our coding standards (see `Src/wthrr/` for examples)
4. **Test** on multiple monitor configurations
5. **Submit** a Pull Request with detailed description

### **Areas for Contribution**
- 🌈 Additional weather effects (hail, sleet, fog)
- 🎵 Audio integration for ambient soundscapes  
- 🎨 Seasonal themes and particle variations
- ⚡ Performance optimizations
- 🖥️ Additional platform support

## 📞 Support

- **GitHub Issues**: [Report bugs or request features](../../issues)
- **Email**: Todd@thedubes.com  
- **Discord**: Join our weather simulation community

## 📄 License
Copyright (C) 2024 RYF TOOLS
All rights reserved.

This software is proprietary and confidential.
Unauthorized copying, distribution, or modification 
is strictly prohibited.
## 🙏 Acknowledgments

- **FastNoiseLite** - Procedural noise generation
- **Microsoft DirectX** - Hardware acceleration
- **Windows Desktop Composition** - Seamless overlay rendering
- **Community Feedback** - Feature inspiration and testing

---

<div align="center">

**Creating immersive desktop weather experiences since 2024**

*Transform your desktop into a living, breathing weather simulation*

[⬇️ Download Latest Release](../../releases) | [📖 Documentation](../../wiki) | [🐛 Report Bug](../../issues)

*Made with ❤️ and lots of ☕ by the RYF TOOLS team*

</div>
