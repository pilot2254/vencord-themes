#include "pch.h"
#include "esp.h"
#include "menu.h"

#include <crymore_filelog.h>
#include <crymore_overlay.h>

namespace {

crymore::overlay::Callbacks make_overlay()
{
    crymore::overlay::Callbacks cb;
    cb.open_on_start = true;
    cb.wants_overlay = [] { return esp::active(); };
    cb.draw_overlay = [](ImDrawList* dl, float w, float h) { esp::draw(dl, w, h); };
    cb.draw_menu = [] { return menu::draw(); };
    return cb;
}

DWORD WINAPI start(LPVOID param)
{
    const auto self = static_cast<HMODULE>(param);
    crymore::filelog::init("crymore-dbd-internal.log");
    CRYMORE_LOG_INFO("---- dbd-internal loaded; game-data provider not configured ----");

    if (!crymore::overlay::install(make_overlay())) {
        CRYMORE_LOG_ERROR("overlay install failed; no compatible DX11 swap chain was found");
        FreeLibraryAndExitThread(self, 1);
    }
    CRYMORE_LOG_INFO("overlay installed; ESP is waiting for validated game data");

    while (!crymore::overlay::unload_requested()) Sleep(100);
    crymore::overlay::uninstall();
    FreeLibraryAndExitThread(self, 0);
    return 0;
}

} // namespace

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        if (HANDLE thread = CreateThread(nullptr, 0, start, hModule, 0, nullptr)) CloseHandle(thread);
        break;
    }
    return TRUE;
}

