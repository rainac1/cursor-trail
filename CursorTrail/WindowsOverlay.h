#ifndef WINDOWS_OVERLAY_H
#define WINDOWS_OVERLAY_H

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <objbase.h>
#include <propidl.h>
#include <gdiplus.h>
#include <vector>
#include <deque>
#include <memory>
#include "TrailPart.h"
#include "Config.h"

// Windows-specific overlay implementation for guaranteed top-level transparent overlay
class WindowsOverlay
{
public:
    WindowsOverlay();
    ~WindowsOverlay();
    
    bool Initialize();
    void Update();
    void Render();
    void Cleanup();
    
    bool IsActive() const { return m_hwnd != nullptr; }
    
private:
    static constexpr size_t kMaxQueuedTrailPoints = 8192;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void DrawTrail(Gdiplus::Graphics& graphics);
    void AddTrailPart(const TrailPart& part);
    void QueueTrailPoint(float x, float y);
    void ProcessQueuedTrailPoints();
    void HandleRawMouseInput(HRAWINPUT rawInputHandle);
    void RegisterRawInput();
    
    HWND m_hwnd;
    HDC m_hdc;
    HDC m_memDC;
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;
    
    int m_screenWidth;
    int m_screenHeight;
    int m_primaryScreenWidthMinusOne;
    int m_primaryScreenHeightMinusOne;
    int m_virtualScreenLeft;
    int m_virtualScreenTop;
    int m_virtualScreenWidthMinusOne;
    int m_virtualScreenHeightMinusOne;
    
    std::vector<TrailPart> m_trailParts;
    std::deque<POINT> m_queuedTrailPoints;
    POINT m_lastRawCursorPoint;
    bool m_hasLastRawCursorPoint;
    size_t m_currentIndex;
    
    std::unique_ptr<Gdiplus::Bitmap> m_trailTexture;
    ULONG_PTR m_gdiplusToken;
};

#endif // _WIN32

#endif // WINDOWS_OVERLAY_H