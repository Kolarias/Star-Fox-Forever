#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include "Common.h"
#include <SceneTree.hpp>
#include <Node.hpp>
#include <string>
#include <KinematicBody.hpp>
#include "player.h"
#include <Spatial.hpp>
#include <StaticBody.hpp>

namespace LevelObject {

class LevelObject : public StaticBody {
    GODOT_CLASS(LevelObject, StaticBody);

private:
    KinematicBody* player;

public:

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);
    void _physics_process(float delta);
};
}

#endif
