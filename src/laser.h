#ifndef LASER_H
#define LASER_H

#include "Common.h"
#include <KinematicBody.hpp>
#include <AudioStreamPlayer.hpp>
#include <Area.hpp>
#include <string>
#include <Sprite3D.hpp>
#include <RayCast.hpp>


namespace Laser {

class Laser : public KinematicBody {
    GODOT_CLASS(Laser, KinematicBody);

private:
    float velocity;
    AudioStreamPlayer* laser_audio;
    Sprite3D* reticle;
    Area* player_area;
    RayCast* raycast;
    KinematicBody* laser;
    Area* area;
    Vector3 normal;

public:
    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
    void collision_handler(Area* area);
};
}

#endif
