#define NOMINMAX

#include "Puddle.h"
#include "MathUtil.h"
#include "RandomGenerator.h"

#include <d2d1.h>
#include <wrl/client.h>  // For Microsoft::WRL::ComPtr
#include <algorithm>
#include <cmath>

// Local min/max helpers to avoid macro issues
namespace {
    template<typename T>
    constexpr T my_min(const T& a, const T& b) noexcept { return (a < b) ? a : b; }
    template<typename T>
    constexpr T my_max(const T& a, const T& b) noexcept { return (a > b) ? a : b; }
}

// Puddle implementation
Puddle::Puddle(DisplayData* pDispData, Vector2 pos, float initialSize) noexcept
    : pDisplayData(pDispData),
      Pos(pos),
      CurrentSize(initialSize),
      CurrentLifetime(0.0f),
      TimeSinceLastRipple(0.0f),
      HasRipple(false),
      RippleProgress(0.0f)
{
}

void Puddle::Update(float deltaSeconds) noexcept
{
    // Update lifetime
    CurrentLifetime += deltaSeconds;
    
    // Handle evaporation
    const float evaporationAmount = EVAPORATION_RATE * deltaSeconds;
    CurrentSize = my_max(0.0f, CurrentSize - evaporationAmount);
    
    // Update ripple effect if active
    if (HasRipple)
    {
        RippleProgress += deltaSeconds / RIPPLE_DURATION;
        if (RippleProgress >= 1.0f)
        {
            HasRipple = false;
            RippleProgress = 0.0f;
        }
    }
    
    // Update ripple timer
    TimeSinceLastRipple += deltaSeconds;
}

void Puddle::Draw(ID2D1DeviceContext* dc) const noexcept
{
    // Only draw if we have a valid size
    if (CurrentSize <= 0.0f)
        return;
        
    // Create a semi-transparent brush based on the drop color
    const float alpha = my_min(0.4f, CurrentSize / MAX_PUDDLE_SIZE * 0.4f);
    const D2D1_COLOR_F puddleColor = D2D1::ColorF(D2D1::ColorF::White, alpha);
    
    // Create a slightly darker color for the puddle
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> puddleBrush;
    if (FAILED(dc->CreateSolidColorBrush(puddleColor, puddleBrush.GetAddressOf())))
        return;
    
    // Draw the main puddle as an ellipse that's wider than tall
    const D2D1_ELLIPSE ellipse = D2D1::Ellipse(
        D2D1::Point2F(Pos.x, Pos.y), 
        CurrentSize * 1.5f,  // Wider
        CurrentSize * 0.7f   // Less tall
    );
    
    // Draw the puddle with the drop color brush
    dc->FillEllipse(ellipse, pDisplayData->DropColorBrush.Get());
    
    // Draw ripple effect if active
    if (HasRipple)
    {
        // Calculate ripple size and opacity based on animation progress
        const float rippleSize = CurrentSize * RIPPLE_SIZE_FACTOR * (0.5f + RippleProgress * 0.5f);
        const float rippleOpacity = my_max(0.0f, 0.4f - RippleProgress * 0.4f);
        
        // Create ripple brush with fading opacity
        const D2D1_COLOR_F rippleColor = D2D1::ColorF(D2D1::ColorF::White, rippleOpacity);
        
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> rippleBrush;
        if (SUCCEEDED(dc->CreateSolidColorBrush(rippleColor, rippleBrush.GetAddressOf())))
        {
            // Draw the ripple as a ring using stroke
            const D2D1_ELLIPSE rippleEllipse = D2D1::Ellipse(
                D2D1::Point2F(Pos.x, Pos.y), 
                rippleSize * 1.5f,  // Wider
                rippleSize * 0.7f   // Less tall
            );
            
            // Draw the ripple with the drop color brush
            dc->DrawEllipse(rippleEllipse, pDisplayData->DropColorBrush.Get(), 1.0f);
        }
    }
}

void Puddle::AddWater(float amount) noexcept
{
    // Add to the size, capped at maximum
    CurrentSize = my_min(MAX_PUDDLE_SIZE, CurrentSize + amount);
    
    // Create a ripple effect if enough time has passed
    if (TimeSinceLastRipple > 1.0f / RIPPLE_FREQUENCY)
    {
        HasRipple = true;
        RippleProgress = 0.0f;
        TimeSinceLastRipple = 0.0f;
    }
}

bool Puddle::IsReadyForRemoval() const noexcept
{
    // Remove when fully evaporated or exceeding maximum lifetime
    return (CurrentSize <= 0.0f || CurrentLifetime > MAX_LIFETIME);
}

// Helper function to calculate distance between two points
[[nodiscard]] static float CalculateDistance(const Vector2& p1, const Vector2& p2) noexcept
{
    const float dx = p2.x - p1.x;
    const float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

// PuddleManager implementation
PuddleManager::PuddleManager(DisplayData* pDispData) noexcept
    : pDisplayData(pDispData), HasTaskbarData(false)
{
    CalculateTaskbarRegion();
    Puddles.reserve(MAX_PUDDLES);
}

void PuddleManager::Update(float deltaSeconds) noexcept
{
    // Update all puddles
    for (auto& puddle : Puddles)
    {
        puddle->Update(deltaSeconds);
    }
    
    // Remove puddles that have evaporated or expired
    Puddles.erase(
        std::remove_if(Puddles.begin(), Puddles.end(),
            [](const std::unique_ptr<Puddle>& p) noexcept { return p->IsReadyForRemoval(); }
        ),
        Puddles.end()
    );
}

void PuddleManager::Draw(ID2D1DeviceContext* dc) const noexcept
{
    // Draw all puddles
    for (const auto& puddle : Puddles)
    {
        puddle->Draw(dc);
    }
}

void PuddleManager::CreateOrAddToPuddle(const Vector2& pos) noexcept
{
    // Ignore if not on taskbar
    if (!IsOnTaskbar(pos))
        return;
        
    // Try to find a nearby puddle to add to
    Puddle* nearbyPuddle = FindNearbyPuddle(pos);
    
    if (nearbyPuddle)
    {
        // Add water to existing puddle
        nearbyPuddle->AddWater(ADD_WATER_AMOUNT);
    }
    else
    {
        // Create a new puddle if we have room
        if (Puddles.size() < MAX_PUDDLES)
        {
            // Add slight randomness to position
            Vector2 adjustedPos = pos;
            adjustedPos.x += RandomGenerator::GetInstance().GenerateFloat(-2.0f, 2.0f);
            
            Puddles.emplace_back(std::make_unique<Puddle>(
                pDisplayData, adjustedPos, INITIAL_SIZE));
        }
        else
        {
            // Find the smallest puddle and add water to it
            const auto smallestIt = std::min_element(Puddles.begin(), Puddles.end(),
                [](const std::unique_ptr<Puddle>& a, const std::unique_ptr<Puddle>& b) noexcept {
                    return a->GetRadius() < b->GetRadius();
                });
                
            if (smallestIt != Puddles.end())
            {
                (*smallestIt)->AddWater(ADD_WATER_AMOUNT);
            }
        }
    }
}

void PuddleManager::Reset() noexcept
{
    Puddles.clear();
    CalculateTaskbarRegion();
}

bool PuddleManager::IsOnTaskbar(const Vector2& pos) const noexcept
{
    if (!HasTaskbarData)
        return false;
        
    // Check if the point is on the taskbar
    // We specifically focus on the top surface of the taskbar
    const int taskbarTop = NormalizedRect.top;
    const int taskbarHeight = 10; // Only use the top portion of taskbar
    
    return (pos.x >= static_cast<float>(NormalizedRect.left) && 
            pos.x <= static_cast<float>(NormalizedRect.right) && 
            pos.y >= static_cast<float>(taskbarTop) && 
            pos.y <= static_cast<float>(taskbarTop + taskbarHeight));
}

Puddle* PuddleManager::FindNearbyPuddle(const Vector2& pos) const noexcept
{
    for (const auto& puddle : Puddles)
    {
        if (CalculateDistance(pos, puddle->GetPosition()) < MERGE_DISTANCE)
        {
            return puddle.get();
        }
    }
    return nullptr;
}

void PuddleManager::CalculateTaskbarRegion() noexcept
{
    // Find the taskbar window
    HWND hTaskbarWnd = nullptr;
    if (pDisplayData)
    {
        const RECT& sceneRect = pDisplayData->SceneRect;
        const RECT displayRect = {
            sceneRect.left,
            sceneRect.top,
            sceneRect.right,
            pDisplayData->Height // Use full height to find the taskbar
        };
        
        // Try to find primary taskbar first (use explicit W suffix for Unicode)
        hTaskbarWnd = FindWindowW(L"Shell_traywnd", nullptr);
        
        if (!hTaskbarWnd)
        {
            // Try to find secondary taskbar if primary not found (use explicit W suffix for Unicode)
            hTaskbarWnd = FindWindowW(L"Shell_SecondaryTrayWnd", nullptr);
        }
        
        if (hTaskbarWnd)
        {
            if (GetWindowRect(hTaskbarWnd, &TaskbarRect))
            {
                // Check if taskbar is at the bottom (most common case)
                if (TaskbarRect.top > displayRect.bottom - 100)
                {
                    NormalizedRect.left = TaskbarRect.left;
                    NormalizedRect.right = TaskbarRect.right;
                    NormalizedRect.top = TaskbarRect.top;
                    NormalizedRect.bottom = TaskbarRect.bottom;
                    
                    // Normalize coordinates relative to scene rect
                    NormalizedRect = MathUtil::NormalizeRect(NormalizedRect, 
                                                            pDisplayData->SceneRect.top, 
                                                            pDisplayData->SceneRect.left);
                    
                    HasTaskbarData = true;
                }
            }
        }
    }
}