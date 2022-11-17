#ifndef ENEMY_H
#define ENEMY_H

#include "Common.h"
#include <KinematicBody.hpp>
#include <StaticBody.hpp>
#include <Transform.hpp>
#include <Area.hpp>
#include <SceneTree.hpp>
#include <Node.hpp>
#include <string>
#include "RandomNumberGenerator.hpp"
#include "player.h"

namespace Enemy {

class Enemy : public KinematicBody {
    GODOT_CLASS(Enemy, Area);

private:
    KinematicBody* player;
    int hp;

public:
    int difficulty;

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
    void collision_handler(Area* area);
};
}

#endif
