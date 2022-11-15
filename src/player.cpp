#include "player.h"
#include "enemy.h"

#define M_PI 3.14159265358979323846

namespace Player {

void Player::_register_methods(){
    register_method("_ready", &Player::_ready);
    register_method("_process", &Player::_process);
    register_method("_physics_process", &Player::_physics_process);
    register_method("collision_handler", &Player::collision_handler);
    
    register_property<Player, float>("Forward Velocity", &Player::forward_velocity, 15.0);
    register_property<Player, float>("Movement Velocity", &Player::movement_velocity, 10.0);
}

void Player::_init() {
    forward_velocity = 15.0;
    movement_velocity = 10.0;
    movement = Vector3();
    mute = false;
    flipped_left = false;
    flipped_right = false;
}

void Player::_ready() {
    start_pos = get_global_transform();
    player = Object::cast_to<KinematicBody>(Node::get_node("/root/Level/Player"));
    player_area = (Area*)(player->get_node("PlayerArea"));
    reticle = (Sprite3D*)(player_area->get_node("Reticle"));
    player_area->connect("area_entered", player, "collision_handler");
    raycast = (RayCast*)(player_area->get_node("RayCast"));
    up_or_down = none;
    left_or_right = neither;
}

void Player::_process(float delta) {
    // Get input
    input = Input::get_singleton();

    // check for movement inputs
    wasd_movement();

    // check for other inputs
    other_inputs();
}

void Player::_physics_process(float delta) {
    // Always move entire player node forward
    movement.z = 20 * forward_velocity * delta;
    move_and_slide(movement, Vector3::UP, false, 4, 0.785398, true);

    // Move player_area specifically up/down left/right according to inputs
    player_movement.x = (player_movement.x * delta * movement_velocity);
    player_movement.y = (player_movement.y * delta * movement_velocity);
    player_movement.z = 0;
    Vector3 old_pos = player_area->get_translation();
    // Only allow player to move within certain box
    if (old_pos.x + player_movement.x >= 16 || old_pos.x + player_movement.x <= -16) {
        player_movement.x = 0;
    }
    if (old_pos.y + player_movement.y >= 10 || old_pos.y + player_movement.y <= -10) {
        player_movement.y = 0;
    }
    player_area->global_translate(player_movement);

    // Also rotate the player_area in direction that we're moving (max 45 degrees);
    // left/right
    Vector3 current_rotation = player_area->get_rotation();
    if (left_or_right == left) {
        Godot::print("here1");
        if (current_rotation.y < 46*(M_PI/180)) {
            Godot::print("here12");
            player_area->rotate_y(2*(M_PI/180));
        }
    } else if (left_or_right == right) {
        Godot::print("here2");
        if (current_rotation.y > -46*(M_PI/180)) {
            Godot::print("here22");
            player_area->rotate_y(-2*(M_PI/180));
        }
    } else if (left_or_right == neither) {
        Godot::print("here3");
        if (current_rotation.y > 0) {
            player_area->rotate_y(-1*(M_PI/180));
        } else if (current_rotation.y < 0) {
            player_area->rotate_y(1*(M_PI/180));
        }
        if (current_rotation.y > (-1*(M_PI/180)) &&  current_rotation.y < (1*(M_PI/180))) {
            player_area->set_rotation(Vector3(player_area->get_rotation().x, 0, player_area->get_rotation().z));
        }
    }
    // up/down
    if (up_or_down == up) {
        if (current_rotation.x > -46*(M_PI/180)) {
            player_area->rotate_x(-2*(M_PI/180));
        }
    } else if (up_or_down == down) {
        if (current_rotation.x < 46*(M_PI/180)) {
            player_area->rotate_x(2*(M_PI/180));
        }
    } else if (up_or_down == none) {
        if (current_rotation.x > 0) {
            player_area->rotate_x(-1*(M_PI/180));
        } else if (current_rotation.x < 0) {
            player_area->rotate_x(1*(M_PI/180));
        }
        if (current_rotation.x > (-1*(M_PI/180)) &&  current_rotation.x < (1*(M_PI/180))) {
            player_area->set_rotation(Vector3(0, player_area->get_rotation().y, player_area->get_rotation().z));
        }
    }

    // Set z rotation to zero
    player_area->set_rotation(Vector3(player_area->get_rotation().x, player_area->get_rotation().y, 0));

    // Place reticle at end of raycast
    reticle->set_translation(raycast->get_cast_to());
}


void Player::collision_handler(Area* area) {
    
}

// Helper function that handles all cases of WASD player movement
void Player::wasd_movement() {
    // Up/Down
    if (input->is_action_pressed("w")) {
        player_movement.y = 3;
        up_or_down = up;
    } else if (input->is_action_pressed("s")) {
        player_movement.y = -3;
        up_or_down = down;
    } else {
        player_movement.y = 0;
        up_or_down = none;
    }

    // Left/Right
    if (input->is_action_pressed("a")) {
        player_movement.x = 3;
        left_or_right = left;
    } else if (input->is_action_pressed("d")) {
        player_movement.x = -3;
        left_or_right = right;
    } else {
        player_movement.x = 0;
        left_or_right = neither;
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

}


}
