#include "esp.h"

#include <mutex>
#include <utility>

namespace esp {
namespace {
Settings g_settings;
Snapshot g_snapshot;
std::mutex g_snapshot_mutex;
}

Settings& settings() { return g_settings; }

void publish(Snapshot snapshot)
{
    std::lock_guard lock(g_snapshot_mutex);
    g_snapshot = std::move(snapshot);
}

bool active() { return g_settings.enabled; }

void draw(ImDrawList* dl, float width, float height)
{
    if (!dl || !g_settings.enabled) return;

    std::lock_guard lock(g_snapshot_mutex);
    const Snapshot& snapshot = g_snapshot;

    if (!snapshot.ready || !snapshot.view.valid) {
        dl->AddText(ImVec2(18.0f, 18.0f), IM_COL32(255, 220, 110, 235),
                    "DBD ESP: waiting for validated game data");
        return;
    }

    crymore::esp::View view = snapshot.view;
    view.width = width;
    view.height = height;
    for (const Actor& actor : snapshot.actors) {
        if (actor.kind == ActorKind::Objective) {
            if (g_settings.objectives.enabled)
                crymore::esp::draw_marker(dl, view, actor.x, actor.y, actor.z, actor.label.c_str(),
                                          actor.distance, actor.color, g_settings.objectives.max_distance,
                                          g_settings.objectives.distance);
            continue;
        }

        crymore::esp::Target target;
        target.x = actor.x;
        target.y = actor.y;
        target.z = actor.z;
        target.height = actor.height;
        target.up_axis = 2; // Unreal world Z is vertical.
        target.label = actor.label.c_str();
        target.distance = actor.distance;
        target.color = actor.color;
        const auto& config = actor.kind == ActorKind::Killer ? g_settings.killer : g_settings.survivors;
        crymore::esp::draw(dl, view, config, target);
    }
}

} // namespace esp
