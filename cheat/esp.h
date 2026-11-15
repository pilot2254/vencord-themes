#pragma once

#include <crymore_esp.h>

#include <string>
#include <vector>

namespace esp {

enum class ActorKind { Survivor, Killer, Objective };

struct Actor {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    float height = 1.8f;
    float distance = -1.0f;
    std::string label;
    ImU32 color = IM_COL32(255, 255, 255, 255);
    ActorKind kind = ActorKind::Survivor;
};

// Filled by the game-data reader once it has validated this build's world, camera, and actors.
struct Snapshot {
    crymore::esp::View view{};
    std::vector<Actor> actors;
    bool ready = false;
};

struct Settings {
    bool enabled = true;
    crymore::esp::Config survivors{};
    crymore::esp::Config killer{};
    crymore::esp::Config objectives{};
};

Settings& settings();
void publish(Snapshot snapshot);
void draw(ImDrawList* draw_list, float width, float height);
bool active();

} // namespace esp
