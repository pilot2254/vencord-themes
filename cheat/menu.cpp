#include "menu.h"

#include "esp.h"
#include <crymore_gui.h>

namespace menu {

bool draw()
{
    static int tab = 0;
    bool unload = false;
    static const char* tabs[] = { "ESP", "Info" };
    if (crymore::gui::begin_window(tabs, 2, tab, ImVec2(620.0f, 440.0f), "Internal")) {
        if (tab == 0) {
            crymore::gui::begin_group("Overlay");
            crymore::gui::toggle("Enabled", &esp::settings().enabled);
            crymore::gui::section("Survivors");
            crymore::esp::config_ui(esp::settings().survivors, false, true);
            crymore::gui::section("Killer");
            crymore::esp::config_ui(esp::settings().killer, false, true);
            crymore::gui::section("Objectives");
            crymore::gui::toggle("Show objectives", &esp::settings().objectives.enabled);
            crymore::gui::slider("Objective range", &esp::settings().objectives.max_distance, 10.0f, 500.0f, "%.0f m");
            crymore::gui::end_group();
        } else {
            crymore::gui::begin_group("Dead by Daylight");
            crymore::gui::text_muted("Internal DX11 overlay initialized.");
            crymore::gui::text_muted("Actor and camera reads remain disabled until this build is validated.");
            unload = crymore::gui::button_secondary("Unload cheat", ImVec2(160.0f, 0.0f));
            crymore::gui::end_group();
        }
    }
    crymore::gui::end_window();
    return unload;
}

} // namespace menu
