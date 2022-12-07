#include "player.h"
#include "enemy.h"
#include "level.h"

#define M_PI 3.14159265358979323846

namespace Player {

void Player::_register_methods(){
    register_method("_ready", &Player::_ready);
    register_method("_process", &Player::_process);
    register_method("_physics_process", &Player::_physics_process);
    register_method("collision_handler", &Player::collision_handler);
    
    register_property<Player, float>("Forward Velocity", &Player::forward_velocity, 15.0);
    register_property<Player, float>("Movement Velocity", &Player::movement_velocity, 10.0);
    register_property<Player, int>("Collision Damage", &Player::collision_damage, 10);
    register_property<Player, int>("Enemy Damage", &Player::enemy_damage, 5);
}

void Player::_init() {
    forward_velocity = 20.0;
    movement_velocity = 10.0;
    movement = Vector3();
    mute = false;
    flipped_left = false;
    flipped_right = false;
    collision_damage = 10;
    enemy_damage = 5;
    game_ended = false;
}

void Player::_ready() {
    bgm_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/BackgroundAudio"));
    end_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/EndAudio"));
    damage_audio = Object::cast_to<AudioStreamPlayer>(Node::get_node("/root/Level/Player/DamageAudio"));
    hp_gauge = Object::cast_to<TextureProgress>(Node::get_node("/root/Level/Player/HealthBar/Bar/Gauge"));
    end_menu = Object::cast_to<Control>(Node::get_node("/root/Level/CanvasLayer2/EndMenu"));
    start_pos = get_global_transform();
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
    reticle = (Sprite3D*)(player->get_node("Reticle"));
    player_area = (Area*)(player->get_node("PlayerArea"));
    player_area->connect("body_entered", player, "collision_handler");
    laser_start = (RayCast*)(player->get_node("LaserStart"));
    ResourceLoader* resourceLoader = ResourceLoader::get_singleton();
    laser_scene = resourceLoader->load("res://Laser.tscn");
    animation_player = (AnimationPlayer*)(player_area->get_node("AnimationPlayer"));
    bgm_audio->play();
    bgm_audio->set_stream_paused(mute);
}

void Player::_process(float delta) {
    // If game is over, dont' do anything, wait for animation to play, then go
    // to end screen
    if (game_ended) {
        movement.z = forward_velocity / 2;
        Level::Level* level = Object::cast_to<Level::Level>(Node::get_node("/root/Level"));
        if (!level->animation_player->is_playing()) {
            end_menu->set_visible(true);
            bgm_audio->set_stream_paused(true);
            bgm_audio->queue_free();
            end_audio->play();
            get_tree()->set_pause(true);
        }
        return;
    }

    // Get input
    input = Input::get_singleton();

    // Always move forward
    movement.z = forward_velocity;

    // Always play idle animation (if another thing isn't already playing)
    if (!animation_player->is_playing()){
        animation_player->play("idle");
    }

    // check for other inputs
    other_inputs();

    // check for movement inputs
    wasd_movement();
}

void Player::_physics_process(float delta) 
{
    // Move player forward
    player->translate(Vector3(0,0,movement.z * delta));

    // Move reticle according to inputs
    reticle_movement.x *= float(movement_velocity * delta * 1.1);
    reticle_movement.y *= float(movement_velocity * delta * 1.1);
    reticle_movement.z = 0;
    // make sure reticle stays in viewable area
    Vector3 old_pos = reticle->get_translation();
    Vector3 new_pos = Vector3(old_pos.x + reticle_movement.x, old_pos.y + reticle_movement.y, 0);
    if (new_pos.x > 39 && reticle_movement.x > 0) {
        reticle_movement.x = 0;
    }
    if (new_pos.x < -39 && reticle_movement.x < 0) {
        reticle_movement.x = 0;
    }
    if (new_pos.y > 20 && reticle_movement.y > 0) {
        reticle_movement.y = 0;
    }
    if (new_pos.y < -20 && reticle_movement.y < 0) {
        reticle_movement.y = 0;
    }
    reticle->translate(reticle_movement);
    
    // Store current rotation
    Vector3 new_rotation = player_area->get_rotation();

    // want player_area to follow the reticle, but lag behind a little bit
    Vector3 reticle_pos = reticle->get_transform().origin;
    Vector3 area_pos = player_area->get_transform().origin;
    Vector3 difference_vec = Vector3((reticle_pos.x / 2) - area_pos.x, (reticle_pos.y / 2) - area_pos.y, 0);
    Vector3 player_area_movement = Vector3(difference_vec.x / 15, difference_vec.y / 15, 0);
    player_area->global_translate(player_area_movement);
    // use look at to find x and y rotation that looks at reticle
    player_area->look_at(reticle->get_global_transform().origin, Vector3::UP);

    // Updated saved rotation with x and y rotation
    new_rotation.x = player_area->get_rotation().x;
    new_rotation.y = player_area->get_rotation().y;

    // Set rotation to that rotation (without look_at's z, we'll do our own z)
    player_area->set_rotation(new_rotation);

    // rotate the player area's z axis (over time) if flip inputs are pressed
    if (flipped_left && !flipped_right) {
        if (new_rotation.z < 90*(M_PI/180)) {
            player_area->rotate_z(-6*(M_PI/180));
        }
    } else if (flipped_right && !flipped_left) {
        if (new_rotation.z > -90*(M_PI/180)) {
            player_area->rotate_z(6*(M_PI/180));
        }
    } else {
        if (new_rotation.z != 0 && new_rotation.z > 0) {
            player_area->rotate_z(3*(M_PI/180));
        } else if (new_rotation.z != 0 && new_rotation.z < 0) {
            player_area->rotate_z(-3*(M_PI/180));
        }
        if (new_rotation.z < 3*(M_PI/180) && new_rotation.z > -3*(M_PI/180)) {
            player_area->set_rotation(Vector3(new_rotation.x, new_rotation.y, 0));
        }
    }
}

// If collide into walls, enemies, or enemy projectiles, take damage, remove health,
// become invulnerable temporarily
void Player::collision_handler(Area* area) {
    if (game_ended) {
        return;
    }
    Laser::Laser* laser = Object::cast_to<Laser::Laser>(area);
    Enemy::Enemy* enemy = Object::cast_to<Enemy::Enemy>(area);
    if(!laser && enemy) {
        int curr_count = hp_gauge->get_value();
        curr_count -= enemy_damage;
        hp_gauge->set_value(curr_count);
        damage_audio->play();
        animation_player->play("Collision");
        if (curr_count <= 0) {
            // died; reset scene
            end_game();
        }
    } else if (!laser) {
        int curr_count = hp_gauge->get_value();
        curr_count -= collision_damage;
        hp_gauge->set_value(curr_count);
        damage_audio->play();
        animation_player->play("Collision");
        if (curr_count <= 0) {
            // died; reset scene
            end_game();
        }
    }
}

// Helper function that handles all cases of WASD player movement
void Player::wasd_movement() {
    // Up/Down
    if (input->is_action_pressed("w")) {
        movement.y = 1;
        reticle_movement.y = 1;
    } else if (input->is_action_pressed("s")) {
        movement.y = -1;
        reticle_movement.y = -1;
    } else {
        movement.y = 0;
        reticle_movement.y = 0;
    }

    // Left/Right - give bonus speed if flipped in same direction!
    if (input->is_action_pressed("a")) {
        movement.x = 1;
        if (flipped_left) {
            reticle_movement.x = 1.5;
        } else {
            reticle_movement.x = 1;
        }
    } else if (input->is_action_pressed("d")) {
        movement.x = -1;
        if (flipped_right) {
            reticle_movement.x = -1.5;
        } else {
            reticle_movement.x = -1;
        }
    } else {
        movement.x = 0;
        reticle_movement.x = 0;
    }
}

// Helper function for inputs other than WASD
void Player::other_inputs() {
    // Flipping left/right
    if (input->is_action_pressed("flip_left")) {
        flipped_left = true;
    } else {
        flipped_left = false;
    }

    if (input->is_action_pressed("flip_right")) {
        flipped_right = true;
    } else {
        flipped_right = false;
    }

    // If both are pressed, neither are true
    if (input->is_action_pressed("flip_right") && input->is_action_pressed("flip_left")) {
        flipped_left = false;
        flipped_right = false;
    }

    // fire lasers
    if (input->is_action_just_pressed("fire")) {
        handle_fire();
    }
}

// Helper function for firing lasers
void Player::handle_fire() {
    // Spawn a laser into the current level tree. Laser will handle aiming
    KinematicBody* laser = Object::cast_to<KinematicBody>(laser_scene->instance());
    get_node("/root/Level/Player")->add_child(laser, true);
}

void Player::end_game() {
    game_ended = true;
    Level::Level* level = Object::cast_to<Level::Level>(Node::get_node("/root/Level"));
    level->animation_player->play("End");
}


}
