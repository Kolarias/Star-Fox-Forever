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
    area->connect("body_entered", laser, "collision_handler");
    laser->set_translation(Vector3(player_area->get_translation().x, player_area->get_translation().y, player_area->get_translation().z + 1));
    //laser->look_at(reticle->get_translation(), Vector3::UP);
    normal = (reticle->get_translation() - laser->get_translation()).normalized();
    laser->set_rotation(player_area->get_rotation());
    laser_audio->play();
}

void Laser::_process(float delta) {
    // If we're too far away from player, delete self
    if ((get_translation().distance_to(player_area->get_translation())) > 100) {
        queue_free();
    }

    // Move forward just like Player
    Player::Player* player = Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"));
    laser->translate(Vector3(0,0,player->forward_velocity * delta));

}

void Laser::_physics_process(float delta) {
    // Move in direction that was given when laser was created
    //move_and_slide(normal * velocity * delta, Vector3::UP, false, 4, 0.785398, true);
    move_and_collide(normal * velocity * delta);

    // Also stretch laser as is goes further away
    if (laser) {
        Vector3 scale = laser->get_scale(); 
        laser->set_scale(Vector3(scale.x, scale.y, scale.z + (3 * delta)));
    }
}

void Laser::collision_handler(Area* area) {
    // Upon colliding with ANYTHING except player, delete laser (but keep audio)
    Player::Player* player = Object::cast_to<Player::Player>(area->get_parent());
    if (player) {
        return;
    }
    this->area->queue_free();
    laser->get_node("CollisionShape")->queue_free();

}
}
