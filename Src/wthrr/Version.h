#pragma once

// wthrr Application Version Information
// This header provides C++ access to version information defined in VersionRC.h

#ifndef WTHRR_VERSION_H
#define WTHRR_VERSION_H

#include <string>
#include <string_view>

// Include the single source of truth for version numbers
#include "VersionRC.h"

namespace wthrr {
    namespace version {
        
        // Version Components (sourced from VersionRC.h)
        constexpr int MAJOR = WTHRR_VERSION_MAJOR;
        constexpr int MINOR = WTHRR_VERSION_MINOR;
        constexpr int PATCH = WTHRR_VERSION_PATCH;
        constexpr int BUILD = WTHRR_VERSION_BUILD;
        
        // Derived version strings (sourced from VersionRC.h)
        constexpr std::string_view VERSION_STRING = WTHRR_VERSION_STRING;
        constexpr std::string_view VERSION_STRING_FULL = WTHRR_VERSION_FULL;
        
        // Build information
        constexpr std::string_view BUILD_DATE = __DATE__;
        constexpr std::string_view BUILD_TIME = __TIME__;
        
        // Application metadata (sourced from VersionRC.h)
        constexpr std::string_view APP_NAME = WTHRR_APP_NAME;
        constexpr std::string_view APP_DESCRIPTION = WTHRR_DESCRIPTION;
        constexpr std::string_view COMPANY_NAME = WTHRR_COMPANY_NAME;
        constexpr std::string_view COPYRIGHT = WTHRR_COPYRIGHT;
        constexpr std::string_view GITHUB_URL = "https://github.com/todddube/wthrr";
        
        // Version comparison helpers
        constexpr int VERSION_NUMBER = (MAJOR << 24) | (MINOR << 16) | (PATCH << 8) | BUILD;
        
        // Runtime version functions
        [[nodiscard]] inline std::string GetVersionString() {
            return std::string{VERSION_STRING};
        }
        
        [[nodiscard]] inline std::string GetFullVersionString() {
            return std::string{VERSION_STRING_FULL};
        }
        
        [[nodiscard]] inline std::string GetVersionWithBuildInfo() {
            return std::string{VERSION_STRING} + " (built " + std::string{BUILD_DATE} + " " + std::string{BUILD_TIME} + ")";
        }
        
        [[nodiscard]] inline std::string GetDialogCaption() {
            return std::string{WTHRR_DIALOG_CAPTION};
        }
        
        // Semantic version checking
        [[nodiscard]] constexpr bool IsPreRelease() noexcept {
            return false; // Set to true for beta/alpha versions
        }
        
        [[nodiscard]] constexpr bool IsDebugBuild() noexcept {
#ifdef _DEBUG
            return true;
#else
            return false;
#endif
        }
    }
}

#endif // WTHRR_VERSION_H