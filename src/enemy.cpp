#include "enemy.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"
#include "level.h"

namespace Enemy {

void Enemy::_register_methods()
{
    register_method("_ready", &Enemy::_ready);
    register_method("_process", &Enemy::_process);
    register_method("_physics_process", &Enemy::_physics_process);
    register_method("collision_handler", &Enemy::collision_handler);
    register_property<Enemy, int>("Difficulty", &Enemy::difficulty, 1);
}

void Enemy::_init() {
    if (difficulty = 1) {
        hp = 3;
    } else if (difficulty = 2) {
        hp = 4;
    } else if (difficulty = 3) {
        hp = 6;
    }
}

void Enemy::_ready() {
    Node* enemy = Node::get_node(".");
    enemy->get_node("Area")->connect("body_entered", enemy, "collision_handler");
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
    // spawn 100 units in front of player, random spot
    Object::cast_to<KinematicBody>(enemy)->set_translation(Vector3(player->get_translation().x + 5, player->get_translation().y + 5, player->get_translation().z + 90));
}

void Enemy::_process(float delta) {
    // If we're too far away from player, delete self
    if ((get_translation().distance_to(player->get_translation())) > 100) {
        queue_free();
    }
}

void Enemy::_physics_process(float delta) {
}

void Enemy::collision_handler(Area* area) {
    hp -= 1;
    if (hp <= 0) {
        Level::Level* level = Object::cast_to<Level::Level>(Node::get_node("/root/Level"));
        level->update_hits();
        queue_free();
    }
}

}
