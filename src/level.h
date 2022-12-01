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
#include <AnimationPlayer.hpp>

namespace Level {

class Level : public Spatial {
    GODOT_CLASS(Level, Spatial);

private:
    Spatial* level;
    RichTextLabel* time_counter;
    RichTextLabel* hit_counter;
    Timer* level_timer;
    Ref<PackedScene> asteroid_scene;
    Ref<PackedScene> asteroid2_scene;
    Ref<PackedScene> horizontal_bar_scene;
    Ref<PackedScene> vertical_bar_scene;
    Ref<PackedScene> diagonal_bar1_scene;
    Ref<PackedScene> diagonal_bar2_scene;
    Ref<PackedScene> wall_scene;
    Ref<PackedScene> easy_enemy_scene;
    Ref<PackedScene> medium_enemy_scene;
    Ref<PackedScene> hard_enemy_scene;

public:
    int hits;
    float time;
    AnimationPlayer* animation_player;

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
    void time_helper(float delta);
    void update_hits();
    void _on_timeout();
    void spawn_easy_terrain();
    void spawn_easy_enemy();
    void spawn_medium_terrain();
    void spawn_medium_enemy();
    void spawn_hard_terrain();
    void spawn_hard_enemy();
};
}

#endif
