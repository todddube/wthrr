#include "DisplayWindow.h"
#include "Global.h"
#include "VersionRC.h"  // Single source of truth for version information
#include <chrono>
#include <thread> // Add thread header for sleep_for

// Suppress warnings for Win32 API parameters that may be unused in release builds
#pragma warning(push)
#pragma warning(disable: 4100)  // unreferenced formal parameter (common in WinMain for some params)

// Modern C++20 frame rate limiting function
[[nodiscard]] std::chrono::microseconds calculateSleepTime(
    const std::chrono::high_resolution_clock::time_point& lastFrameTime,
    const std::chrono::microseconds& targetFrameTime) noexcept {
    
    const auto now = std::chrono::high_resolution_clock::now();
    const auto frameTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrameTime);
    
    return (frameTime < targetFrameTime) ? (targetFrameTime - frameTime) : std::chrono::microseconds{0};
}

void sleepFor(const std::chrono::high_resolution_clock::time_point& lastFrameTime, 
              const std::chrono::microseconds& targetFrameTime) noexcept {
    const auto sleepTime = calculateSleepTime(lastFrameTime, targetFrameTime);
    if (sleepTime > std::chrono::microseconds{0}) {
        std::this_thread::sleep_for(sleepTime);
    }
}

//
// Provides the entry point to the application.
//
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE /*hPrevInstance*/,
    _In_ LPSTR /*lpCmdLine*/,
    _In_ int /*nCmdShow*/)
{
    // Ignore the return value because we want to continue running even in the
    // unlikely event that HeapSetInformation fails.
    static_cast<void>(HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0));

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    // Optional: Log version information for debugging
#ifdef _DEBUG
    OutputDebugStringA(("wthrr " WTHRR_VERSION_STRING " (built " __DATE__ " " __TIME__ ") starting...\n"));
#endif

    std::vector<MonitorData> monitorDataList;
    EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitorDataList));

    if (SUCCEEDED(CoInitialize(nullptr))) {
        std::vector<std::unique_ptr<DisplayWindow>> rainWindows;
        rainWindows.reserve(monitorDataList.size());
        
        for (const auto& monitorData : monitorDataList) {
            auto rainWindow = std::make_unique<DisplayWindow>();
            if (SUCCEEDED(rainWindow->Initialize(hInstance, monitorData))) {
                rainWindows.push_back(std::move(rainWindow));
            }
        }

        if (!rainWindows.empty()) {
            MSG msg = {};
            
            // Modern C++20 constants - Target 60 FPS
            static constexpr auto TARGET_FPS = 60;
            static constexpr auto TARGET_FRAME_TIME = std::chrono::microseconds{1000000 / TARGET_FPS};
            
            auto lastFrameTime = std::chrono::high_resolution_clock::now();
            
            while (msg.message != WM_QUIT) {
                if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                } else {
                    for (const auto& rainWindow : rainWindows) {
                        rainWindow->Animate();
                    }
                    
                    sleepFor(lastFrameTime, TARGET_FRAME_TIME);
                    lastFrameTime = std::chrono::high_resolution_clock::now();
                }
            }
        }
        CoUninitialize();
    }
    return 0;
}

#pragma warning(pop)  // Restore warning settings
