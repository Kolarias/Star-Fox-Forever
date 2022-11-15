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

enum UP_OR_DOWN {up, down, none};
enum LEFT_OR_RIGHT {left, right, neither};

namespace Player  {

class Player : public KinematicBody  {
    GODOT_CLASS(Player, KinematicBody);

private:
    // Editable in Godot editor
    float forward_velocity;
    float movement_velocity;

    // Non-editable in Godot editor
    Transform start_pos;
    KinematicBody* player;
    Area* player_area;
    Vector3 movement;
    Input* input;
    Sprite3D* reticle;
    Vector3 player_movement;
    RayCast* raycast;
    UP_OR_DOWN up_or_down;
    LEFT_OR_RIGHT left_or_right;
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
};
}

#endif
