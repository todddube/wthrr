#pragma once

#include <vector>
#include <memory>
#include <d2d1.h>
#include "Vector2.h"
#include "DisplayData.h"

// Forward declarations to reduce compilation dependencies
struct ID2D1DeviceContext;

// Puddle class - represents small water accumulations on the taskbar
class Puddle final
{
public:
    explicit Puddle(DisplayData* pDispData, Vector2 pos, float initialSize) noexcept;
    ~Puddle() noexcept = default;

    // Delete copy operations to prevent accidental copying
    Puddle(const Puddle&) = delete;
    Puddle& operator=(const Puddle&) = delete;
    
    // Default move operations
    Puddle(Puddle&&) noexcept = default;
    Puddle& operator=(Puddle&&) noexcept = default;

    // Main interface functions
    void Update(float deltaSeconds) noexcept;
    void Draw(ID2D1DeviceContext* dc) const noexcept;
    void AddWater(float amount) noexcept;
    [[nodiscard]] bool IsReadyForRemoval() const noexcept;
    [[nodiscard]] const Vector2& GetPosition() const noexcept { return Pos; }
    [[nodiscard]] float GetRadius() const noexcept { return CurrentSize; }

private:
    static constexpr float MAX_PUDDLE_SIZE = 4.0f;       // Maximum radius in pixels
    static constexpr float EVAPORATION_RATE = 0.10f;     // How quickly puddles shrink per second
    static constexpr float MAX_LIFETIME = 10.0f;         // Maximum lifetime in seconds
    static constexpr float RIPPLE_FREQUENCY = 0.5f;      // Ripples per second when adding water
    static constexpr float RIPPLE_DURATION = 0.7f;       // How long a ripple lasts in seconds
    static constexpr float RIPPLE_SIZE_FACTOR = 1.2f;    // How much larger ripples are than puddle

    DisplayData* pDisplayData;  // Non-owning pointer
    Vector2 Pos;
    float CurrentSize;          // Current radius of puddle
    float CurrentLifetime;      // How long this puddle has existed in seconds
    float TimeSinceLastRipple;  // Time since the last ripple effect
    bool HasRipple;             // Whether there's an active ripple
    float RippleProgress;       // 0.0 to 1.0 tracking ripple animation progress
};

// PuddleManager class - manages collection of puddles and their interactions
class PuddleManager final
{
public:
    explicit PuddleManager(DisplayData* pDispData) noexcept;
    ~PuddleManager() noexcept = default;

    // Delete copy operations to prevent accidental copying
    PuddleManager(const PuddleManager&) = delete;
    PuddleManager& operator=(const PuddleManager&) = delete;
    
    // Default move operations
    PuddleManager(PuddleManager&&) noexcept = default;
    PuddleManager& operator=(PuddleManager&&) noexcept = default;

    void Update(float deltaSeconds) noexcept;
    void Draw(ID2D1DeviceContext* dc) const noexcept;
    void CreateOrAddToPuddle(const Vector2& pos) noexcept;
    void Reset() noexcept;
    
    // Is this point on the taskbar?
    [[nodiscard]] bool IsOnTaskbar(const Vector2& pos) const noexcept;

private:
    static constexpr int MAX_PUDDLES = 50;               // Maximum puddles to avoid overdrawing
    static constexpr float MERGE_DISTANCE = 15.0f;      // Distance for puddle merging
    static constexpr float INITIAL_SIZE = 3.0f;         // Initial radius for new puddles
    static constexpr float ADD_WATER_AMOUNT = 1.0f;     // Amount of water added per drop
    
    DisplayData* pDisplayData;  // Non-owning pointer
    std::vector<std::unique_ptr<Puddle>> Puddles;
    
    // Find a nearby puddle, return nullptr if none found within MERGE_DISTANCE
    [[nodiscard]] Puddle* FindNearbyPuddle(const Vector2& pos) const noexcept;
    
    // Calculate taskbar region for puddle placement
    void CalculateTaskbarRegion() noexcept;
    
    // Taskbar region data
    RECT TaskbarRect{};         // The global taskbar position
    RECT NormalizedRect{};      // Normalized for puddle placement
    bool HasTaskbarData;        // Whether taskbar data is valid
};