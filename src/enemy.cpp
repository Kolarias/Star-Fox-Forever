#include "enemy.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"

namespace Enemy {

void Enemy::_register_methods()
{
    register_method("_ready", &Enemy::_ready);
    register_method("_process", &Enemy::_process);
    register_method("_physics_process", &Enemy::_physics_process);
    register_method("collision_handler", &Enemy::collision_handler);
}

void Enemy::_init() {
}

void Enemy::_ready() {
}

void Enemy::_process(float delta) {
}

void Enemy::_physics_process(float delta) {
}

void Enemy::collision_handler(Area* area) {
}

}
