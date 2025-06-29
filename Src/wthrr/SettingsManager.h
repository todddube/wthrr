#pragma once

#include <string>
#include <memory>
#include <windows.h>

namespace RainEngine {

// Modern C++20 constants
inline constexpr int MAX_PARTICLES = 75;

// Modern enum class for type safety
enum class ParticleType : int {
    Rain = 0,
    Snow = 1
};

// Modern settings class with C++20 features
class Setting {
public:
    bool loaded = false;
    int MaxParticles;
    int WindSpeed;
    COLORREF ParticleColor;
    ParticleType PartType;
    
    // Lightning settings
    int LightningFrequency;  // 0-100 scale, 50 = default
    int LightningIntensity;  // 0-100 scale, 50 = default

    // Snow wind randomness settings
    bool EnableSnowWind;      // Whether to enable random wind for snow
    int SnowWindIntensity;    // 0-100 scale, how strong the wind is
    int SnowWindVariability;  // 0-100 scale, how frequently the wind changes

    // Modern constructor with designated initializers support
    explicit constexpr Setting(
        int maxParticles = 10, 
        int windSpeed = 3, 
        COLORREF particleColor = 0x00AAAAAA, 
        ParticleType partType = ParticleType::Rain,
        int lightningFrequency = 50,
        int lightningIntensity = 50,
        bool enableSnowWind = false,
        int snowWindIntensity = 25,
        int snowWindVariability = 50) noexcept
        : MaxParticles(maxParticles)
        , WindSpeed(windSpeed)
        , ParticleColor(particleColor)
        , PartType(partType)
        , LightningFrequency(lightningFrequency)
        , LightningIntensity(lightningIntensity)
        , EnableSnowWind(enableSnowWind)
        , SnowWindIntensity(snowWindIntensity)
        , SnowWindVariability(snowWindVariability) {
    }

    // Default copy/move operations
    Setting(const Setting&) = default;
    Setting& operator=(const Setting&) = default;
    Setting(Setting&&) = default;
    Setting& operator=(Setting&&) = default;
    ~Setting() = default;
};

// Modern singleton with thread safety
class SettingsManager {
public:
    // Delete copy and move operations for singleton
    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
    SettingsManager(SettingsManager&&) = delete;
    SettingsManager& operator=(SettingsManager&&) = delete;

    [[nodiscard]] static SettingsManager& GetInstance() noexcept;
    
    void ReadSettings(Setting& setting) const noexcept;
    void WriteSettings(const Setting& setting) const noexcept;

private:
    SettingsManager() noexcept;
    ~SettingsManager() = default;

    [[nodiscard]] static std::wstring GetAppDataPath();
    void CreateINIFile() const noexcept;

    std::wstring iniFilePath_;
    Setting defaultSetting_;
};

} // namespace RainEngine

// Backward compatibility aliases
using ParticleType = RainEngine::ParticleType;
using Setting = RainEngine::Setting;
using SettingsManager = RainEngine::SettingsManager;

// Enum value aliases for backward compatibility
inline constexpr auto RAIN = ParticleType::Rain;
inline constexpr auto SNOW = ParticleType::Snow;
