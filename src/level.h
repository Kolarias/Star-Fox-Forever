#ifndef LEVEL_H
#define LEVEL_H

#include "Common.h"
#include <Spatial.hpp>
#include <KinematicBody.hpp>
#include <Transform.hpp>
#include <Area.hpp>
#include <SceneTree.hpp>
#include <Node.hpp>
#include <string>
#include "RandomNumberGenerator.hpp"
#include "player.h"
#include <RichTextLabel.hpp>
#include <Timer.hpp>
#include <PackedScene.hpp>
#include <StaticBody.hpp>

namespace Level {

class Level : public Spatial {
    GODOT_CLASS(Level, Spatial);

private:
    Spatial* level;
    RichTextLabel* time_counter;
    RichTextLabel* hit_counter;
    Timer* level_timer;
    float time;
    Ref<PackedScene> center_ball_scene;

public:
    int hits;

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
    void time_helper(float delta);
    void update_hits();
    void _on_timeout();
};
}

#endif
