#include "SettingsManager.h"

#include <windows.h>
#include <string>
#include <shlobj.h>
#include <iostream>
#include <mutex>

using namespace RainEngine;

SettingsManager::SettingsManager() noexcept 
    : defaultSetting_(MAX_PARTICLES, 3, 0x00AAAAAA, ParticleType::Rain, 50, 50, false, 25, 50) {
    const std::wstring appDataPath = GetAppDataPath();
    iniFilePath_ = appDataPath + L"\\wthrr.ini";
}

std::wstring SettingsManager::GetAppDataPath() {
    wchar_t path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_APPDATA, nullptr, 0, path))) {
        return std::wstring(path);
    }
    return L"";
}

void SettingsManager::CreateINIFile() const noexcept {
    WritePrivateProfileString(L"Settings", L"MaxParticles", 
                             std::to_wstring(defaultSetting_.MaxParticles).c_str(),
                             iniFilePath_.c_str());
    WritePrivateProfileString(L"Settings", L"WindSpeed", 
                             std::to_wstring(defaultSetting_.WindSpeed).c_str(),
                             iniFilePath_.c_str());
    
    wchar_t colorBuffer[16];
    swprintf_s(colorBuffer, 16, L"%08X", defaultSetting_.ParticleColor);
    WritePrivateProfileString(L"Settings", L"ParticleColor", colorBuffer, iniFilePath_.c_str());

    WritePrivateProfileString(L"Settings", L"ParticleType", 
                             std::to_wstring(static_cast<int>(defaultSetting_.PartType)).c_str(),
                             iniFilePath_.c_str());
    
    // Lightning settings
    WritePrivateProfileString(L"Settings", L"LightningFrequency", 
                             std::to_wstring(defaultSetting_.LightningFrequency).c_str(),
                             iniFilePath_.c_str());
    WritePrivateProfileString(L"Settings", L"LightningIntensity", 
                             std::to_wstring(defaultSetting_.LightningIntensity).c_str(),
                             iniFilePath_.c_str());
        
    // Snow wind randomness settings
    WritePrivateProfileString(L"Settings", L"EnableSnowWind", 
                             std::to_wstring(defaultSetting_.EnableSnowWind ? 1 : 0).c_str(),
                             iniFilePath_.c_str());
    WritePrivateProfileString(L"Settings", L"SnowWindIntensity", 
                             std::to_wstring(defaultSetting_.SnowWindIntensity).c_str(),
                             iniFilePath_.c_str());
    WritePrivateProfileString(L"Settings", L"SnowWindVariability", 
                             std::to_wstring(defaultSetting_.SnowWindVariability).c_str(),
                             iniFilePath_.c_str());
}

SettingsManager& SettingsManager::GetInstance() noexcept {
    static SettingsManager instance;
    return instance;
}

void SettingsManager::ReadSettings(Setting& setting) const noexcept {
    if (GetFileAttributes(iniFilePath_.c_str()) == INVALID_FILE_ATTRIBUTES) {
        CreateINIFile();
    }

    setting.MaxParticles = GetPrivateProfileInt(L"Settings", L"MaxParticles", 
                                               defaultSetting_.MaxParticles,
                                               iniFilePath_.c_str());
    setting.WindSpeed = GetPrivateProfileInt(L"Settings", L"WindSpeed", 
                                            defaultSetting_.WindSpeed,
                                            iniFilePath_.c_str());

    wchar_t colorBuffer[16];
    GetPrivateProfileString(L"Settings", L"ParticleColor", nullptr, 
                           colorBuffer, 16, iniFilePath_.c_str());
    setting.ParticleColor = colorBuffer[0] ? wcstoul(colorBuffer, nullptr, 16) 
                                          : defaultSetting_.ParticleColor;

    const int particleTypeInt = GetPrivateProfileInt(L"Settings", L"ParticleType", 
                                                    static_cast<int>(defaultSetting_.PartType),
                                                    iniFilePath_.c_str());
    setting.PartType = static_cast<ParticleType>(particleTypeInt);

    // Lightning settings
    setting.LightningFrequency = GetPrivateProfileInt(L"Settings", L"LightningFrequency", 
                                                     defaultSetting_.LightningFrequency,
                                                     iniFilePath_.c_str());
    setting.LightningIntensity = GetPrivateProfileInt(L"Settings", L"LightningIntensity", 
                                                     defaultSetting_.LightningIntensity,
                                                     iniFilePath_.c_str());
    
    // Snow wind randomness settings
    setting.EnableSnowWind = GetPrivateProfileInt(L"Settings", L"EnableSnowWind", 
                                                 defaultSetting_.EnableSnowWind ? 1 : 0,
                                                 iniFilePath_.c_str()) != 0;
    setting.SnowWindIntensity = GetPrivateProfileInt(L"Settings", L"SnowWindIntensity", 
                                                    defaultSetting_.SnowWindIntensity,
                                                    iniFilePath_.c_str());
    setting.SnowWindVariability = GetPrivateProfileInt(L"Settings", L"SnowWindVariability", 
                                                      defaultSetting_.SnowWindVariability,
                                                      iniFilePath_.c_str());

    WriteSettings(setting);
    setting.loaded = true;
}

void SettingsManager::WriteSettings(const Setting& setting) const noexcept {
    WritePrivateProfileString(L"Settings", L"MaxParticles", 
                             std::to_wstring(setting.MaxParticles).c_str(),
                             iniFilePath_.c_str());
    WritePrivateProfileString(L"Settings", L"WindSpeed", 
                             std::to_wstring(setting.WindSpeed).c_str(),
                             iniFilePath_.c_str());
    
    wchar_t colorBuffer[16];
    swprintf_s(colorBuffer, 16, L"%08X", setting.ParticleColor);
    WritePrivateProfileString(L"Settings", L"ParticleColor", colorBuffer, iniFilePath_.c_str());
    
    WritePrivateProfileString(L"Settings", L"ParticleType", 
                             std::to_wstring(static_cast<int>(setting.PartType)).c_str(),
                             iniFilePath_.c_str());
    
    // Lightning settings
    WritePrivateProfileString(L"Settings", L"LightningFrequency", 
                             std::to_wstring(setting.LightningFrequency).c_str(),
                             iniFilePath_.c_str());
    WritePrivateProfileString(L"Settings", L"LightningIntensity", 
                             std::to_wstring(setting.LightningIntensity).c_str(),
                             iniFilePath_.c_str());
        
    // Snow wind randomness settings
    WritePrivateProfileString(L"Settings", L"EnableSnowWind", 
                             std::to_wstring(setting.EnableSnowWind ? 1 : 0).c_str(),
                             iniFilePath_.c_str());
    WritePrivateProfileString(L"Settings", L"SnowWindIntensity", 
                             std::to_wstring(setting.SnowWindIntensity).c_str(),
                             iniFilePath_.c_str());
    WritePrivateProfileString(L"Settings", L"SnowWindVariability", 
                             std::to_wstring(setting.SnowWindVariability).c_str(),
                             iniFilePath_.c_str());
}
