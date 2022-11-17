#include "levelobject.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"

namespace LevelObject {

void LevelObject::_register_methods() {
    register_method("_ready", &LevelObject::_ready);
    register_method("_process", &LevelObject::_process);
    register_method("_physics_process", &LevelObject::_physics_process);
}

void LevelObject::_init() {
}

void LevelObject::_ready() {
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
    // spawn 100 units in front of player
    Object::cast_to<StaticBody>(Node::get_node("."))->set_translation(Vector3(player->get_translation().x - 5, player->get_translation().y - 5, player->get_translation().z + 90));
}

void LevelObject::_process(float delta) {
    // If we're too far away from player, delete self
    if ((get_translation().distance_to(player->get_translation())) > 100) {
        queue_free();
    }
}

void LevelObject::_physics_process(float delta) {
}

}
