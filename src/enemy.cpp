#include "enemy.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"
#include "level.h"
#include <cstdlib>

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
}

void Enemy::_ready() {
    if (difficulty = 1) {
        hp = 2;
    } else if (difficulty = 2) {
        hp = 3;
    } else if (difficulty = 3) {
        hp = 4;
    }
    enemy = Node::get_node(".");
    enemy->get_node("Area")->connect("body_entered", enemy, "collision_handler");
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
    animation_player = (AnimationPlayer*)(enemy->get_node("AnimationPlayer"));
    Object::cast_to<KinematicBody>(enemy)->set_translation(Vector3(player->get_translation().x, player->get_translation().y, player->get_translation().z + 90));
}

void Enemy::_process(float delta) {
    // If dead, queue free the moment death animation is done
    if (is_dead) {
        if (!animation_player->is_playing()) {
            queue_free();
        }
        return;
    }

    // Always play idle animation (if another thing isn't already playing)
    if (!animation_player->is_playing()) {
        animation_player->play("idle");
    }

    // If we're too far away from player, delete self
    if ((get_translation().distance_to(player->get_translation())) > 100) {
        queue_free();
    }
}

void Enemy::_physics_process(float delta) {
}

void Enemy::collision_handler(Area* area) {
    if (is_dead) {
        return;
    }

    if (Object::cast_to<Enemy>(area)) { 
        return;
    }

    hp -= 1;
    if (hp <= 0) {
        Level::Level* level = Object::cast_to<Level::Level>(Node::get_node("/root/Level"));
        level->update_hits();
        is_dead = true;
        enemy->get_node("Area")->queue_free();
        enemy->get_node("CollisionShape")->queue_free();
        animation_player->play("death");
    } else {
        animation_player->play("hurt");
    }
}

}
