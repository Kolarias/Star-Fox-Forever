#include "laser.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"

namespace Laser {

void Laser::_register_methods()
{
    register_method("_ready", &Laser::_ready);
    register_method("_process", &Laser::_process);
    register_method("_physics_process", &Laser::_physics_process);
    register_method("collision_handler", &Laser::collision_handler);
    register_property<Laser, float>("Velocity", &Laser::velocity, 180.0);
}

void Laser::_init() {
    velocity = 180.0;
}

void Laser::_ready() {
    player_area = Object::cast_to<Area>(Node::get_node("/root/Level/Player/PlayerArea"));
    reticle = Object::cast_to<Sprite3D>(Node::get_node("/root/Level/Player/Reticle"));
    raycast = Object::cast_to<RayCast>(Node::get_node("/root/Level/Player/LaserStart"));
    laser = Object::cast_to<KinematicBody>(Node::get_node("."));
    laser_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("LaserAudio"));
    area = (Area*)laser->get_node("Area");
    area->connect("area_entered", laser, "collision_handler");
    laser->set_translation(Vector3(player_area->get_translation().x, player_area->get_translation().y, player_area->get_translation().z + 1));
    //laser->look_at(reticle->get_translation(), Vector3::UP);
    normal = (reticle->get_translation() - laser->get_translation()).normalized();
    laser->set_rotation(player_area->get_rotation());
    laser_audio->play();
}

void Laser::_process(float delta) {
    // If we're too far away from player, delete self
    if ((laser->get_translation().distance_to(player_area->get_translation())) > 100) {
        laser->queue_free();
    }

}

void Laser::_physics_process(float delta) {
    // Move in direction that was given when laser was created
    move_and_slide(normal * velocity, Vector3::UP, false, 4, 0.785398, true);

    // Also stretch laser as is goes further away
    Vector3 scale = laser->get_scale(); 
    laser->set_scale(Vector3(scale.x, scale.y, scale.z + (15 * delta)));
}

void Laser::collision_handler(Area* area) {
    // Upon colliding with ANYTHING except player, delete self
    Player::Player* player = Object::cast_to<Player::Player>(area->get_parent());
    if (player) {
        return;
    }
    laser->queue_free();
}

}
