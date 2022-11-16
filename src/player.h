#ifndef PLAYER_H
#define PLAYER_H

#include "Common.h"
#include <KinematicBody.hpp>
#include <RayCast.hpp>
#include <Transform.hpp>
#include <Area.hpp>
#include <Label.hpp>
#include <TextureProgress.hpp>
#include <AudioStreamPlayer.hpp>
#include <SceneTree.hpp>
#include <string>
#include <sprite3d.hpp>
#include <ResourceLoader.hpp>
#include <laser.h>
#include <enemy.h>
#include <Spatial.hpp>
#include <PackedScene.hpp>
#include <TextureProgress.hpp>

namespace Player  {

class Player : public KinematicBody  {
    GODOT_CLASS(Player, KinematicBody);

private:
    // Editable in Godot editor
    float forward_velocity;
    float movement_velocity;
    int collision_damage;
    int enemy_damage;

    // Non-editable in Godot editor
    Spatial* level;
    Ref<PackedScene> laser_scene;
    TextureProgress* hp_gauge;
    AudioStreamPlayer* bgm_audio;
    AudioStreamPlayer* damage_audio;
    Transform start_pos;
    KinematicBody* player;
    Area* player_area;
    Vector3 movement;
    Input* input;
    Sprite3D* reticle;
    Vector3 reticle_movement;
    RayCast* laser_start;
    bool flipped_left;
    bool flipped_right;

public:
    // Editable in Godo editor
    bool mute;

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
    void collision_handler(Area* area);
    void wasd_movement();
    void other_inputs();
    void handle_fire();
};
}

#endif
