#include "level.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"
#include <ctime>
#include <random>

namespace Level {

void Level::_register_methods()
{
    register_method("_ready", &Level::_ready);
    register_method("_process", &Level::_process);
    register_method("_physics_process", &Level::_physics_process);
    register_method("_on_timeout", &Level::_on_timeout);
}

void Level::_init() {
    // Can't figure out how to get seeding to work
	srand(6);
    time = 0.0;
    hits = 0;
}

void Level::_ready() {
    level = Object::cast_to<Level>(Node::get_node("/root/Level"));
    time_counter = Object::cast_to<RichTextLabel>(Node::get_node("/root/Level/TimeCounter/Time"));
    hit_counter = Object::cast_to<RichTextLabel>(Node::get_node("/root/Level/HitCounter/Hits"));
    level_timer = Object::cast_to<Timer>(Node::get_node("/root/Level/LevelTimer"));
    level_timer->connect("timeout", level, "_on_timeout");
    ResourceLoader* resourceLoader = ResourceLoader::get_singleton();
    asteroid_scene = resourceLoader->load("res://level_objects/Asteroid.tscn");
    asteroid2_scene = resourceLoader->load("res://level_objects/Asteroid2.tscn");
    horizontal_bar_scene = resourceLoader->load("res://level_objects/HorizontalBar.tscn");
    vertical_bar_scene = resourceLoader->load("res://level_objects/VerticalBar.tscn");
    diagonal_bar1_scene = resourceLoader->load("res://level_objects/DiagonalBar1.tscn");
    diagonal_bar2_scene = resourceLoader->load("res://level_objects/DiagonalBar2.tscn");
    wall_scene = resourceLoader->load("res://level_objects/Wall.tscn");;
    easy_enemy_scene = resourceLoader->load("res://enemies/EasyEnemy.tscn");
    medium_enemy_scene = resourceLoader->load("res://enemies/MediumEnemy.tscn");
    hard_enemy_scene = resourceLoader->load("res://enemies/HardEnemy.tscn");
    animation_player = (AnimationPlayer*)(level->get_node("AnimationPlayer"));
    animation_player->play("Start");
}

void Level::_process(float delta) {
    // Update time
    time_helper(delta);
}

void Level::_physics_process(float delta) {
}

void Level::time_helper(float delta) {
    // LOTS OF UNECESSARY MATH HERE!
    time += delta;
    int ten_minutes = time / 600;
    int minutes = (time / 60) - (ten_minutes * 10);
    int ten_seconds = (time / 10) - (minutes * 6) - (ten_minutes * 60);
    int seconds = (time / 1) - (ten_seconds * 10) - (minutes * 60) - (ten_minutes * 600);
    int milliseconds = (time / 0.1) - (seconds * 10) - (ten_seconds * 100) - (minutes * 600) - (ten_minutes * 6000);
    int centiseconds = (time / 0.01) - (milliseconds * 10)- (seconds * 100) - (ten_seconds * 1000) - (minutes * 6000) - (ten_minutes * 60000);
    string new_time = "[right]" + std::to_string(ten_minutes) + std::to_string(minutes) +
        ":" + std::to_string(ten_seconds) + std::to_string(seconds) + ":" 
        + std::to_string(milliseconds) + std::to_string(centiseconds) +"[/right]";
    time_counter->set_bbcode(new_time.c_str());
}

void Level::update_hits() {
    hits += 1;
    hit_counter->set_bbcode(std::to_string(hits).c_str());
}

// When timer pops, decide what enemy or terrain thing to spawn
void Level::_on_timeout() {
    if (Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"))->game_ended) {
        return;
    }

    // Get a random number between 0 and 100
	int random = rand() % 100;

    // Get time in minutes
    int minutes = (time / 60);

    // Decide what to spawn based on random number and time passed
    // Start game
    if (minutes < 1) {
        if (random <= 40) {
            return;
        } else if (random <= 70) {
            spawn_easy_terrain();
        } else if (random <= 100) {
            spawn_easy_enemy();
        }
    // Early game
    } else if (minutes < 2) {
        if (random <= 20) {
            return;
        } else if (random <= 50) {
            spawn_easy_terrain();
        } else if (random <= 80) {
            spawn_easy_enemy();
        } else if (random <= 90) {
            spawn_medium_terrain();
        } else if (random <= 100) {
            spawn_medium_enemy();
        }
    // Mid game
    } else if (minutes < 4) {
        if (random <= 10) {
            return;
        } else if (random <= 25) {
            spawn_easy_terrain();
        } else if (random <= 40) {
            spawn_easy_enemy();
        } else if (random <= 70) {
            spawn_medium_terrain();
        } else if (random <= 100) {
            spawn_medium_enemy();
        }
    // Late game
    } else if (minutes < 7) {
        if (random <= 5) {
            spawn_easy_terrain();
        } else if (random <= 10) {
            spawn_easy_enemy();
        } else if (random <= 25) {
            spawn_medium_terrain();
        } else if (random <= 40) {
            spawn_medium_enemy();
        } else if (random <= 70) {
            spawn_hard_terrain();
        } else if (random <= 100) {
            spawn_hard_enemy();
        }
    // End game
    } else {
         if (random <= 5) {
            spawn_medium_terrain();
        } else if (random <= 10) {
            spawn_medium_enemy();
        } else if (random <= 55) {
            spawn_hard_terrain();
        } else if (random <= 100) {
            spawn_hard_enemy();
        }
    }
}

void Level::spawn_easy_terrain() {
    StaticBody* asteroid;
    int random = rand() % 3;
    if (random <= 1) {
        asteroid = Object::cast_to<StaticBody>(asteroid_scene->instance());
    } else {
        asteroid = Object::cast_to<StaticBody>(asteroid2_scene->instance());
    }
    int rand_x = -15 + (rand() % 30);
    int rand_y = -10 + (rand() % 20);
    level->add_child(asteroid, true);
    asteroid->global_translate(Vector3(rand_x, rand_y, 0));
}

void Level::spawn_easy_enemy() {
    KinematicBody* easy_enemy = Object::cast_to<KinematicBody>(easy_enemy_scene->instance());
    int rand_x = -15 + (rand() % 30);
    int rand_y = -10 + (rand() % 20);
    level->add_child(easy_enemy, true);
    easy_enemy->global_translate(Vector3(rand_x, rand_y, 0));
}

void Level::spawn_medium_terrain() {
    StaticBody* bar;
    int random = rand() % 4;
    if (random <= 1) {
        bar = Object::cast_to<StaticBody>(horizontal_bar_scene->instance());
        int rand_y = -10 + (rand() % 20);
        level->add_child(bar, true);
        bar->global_translate(Vector3(0, rand_y, 0));
    } else if (random <= 2) {
        bar = Object::cast_to<StaticBody>(vertical_bar_scene->instance());
        int rand_x = -15 + (rand() % 30);
        level->add_child(bar, true);
        bar->global_translate(Vector3(rand_x, 0, 0));
    } else if (random <= 3) {
        bar = Object::cast_to<StaticBody>(diagonal_bar1_scene->instance());
        level->add_child(bar, true);
    } else {
        bar = Object::cast_to<StaticBody>(diagonal_bar2_scene->instance());
        level->add_child(bar, true);
    }
}

void Level::spawn_medium_enemy() {
    KinematicBody* medium_enemy = Object::cast_to<KinematicBody>(medium_enemy_scene->instance());
    int rand_x = -10 + (rand() % 20);
    int rand_y = -15 + (rand() % 30);
    level->add_child(medium_enemy, true);
    medium_enemy->global_translate(Vector3(rand_x, rand_y, 0));
}

void Level::spawn_hard_terrain() {
    StaticBody* wall = Object::cast_to<StaticBody>(wall_scene->instance());
    level->add_child(wall, true);
    int random = rand() % 4;
    if (random <= 1) {
        wall->global_translate(Vector3(13, 0, 0));
    } else if (random <= 2) {
        wall->global_translate(Vector3(-13, 0, 0));
    } else if (random <= 3) {
        wall->global_translate(Vector3(0, 7, 0));
    } else {
        wall->global_translate(Vector3(0, -7, 0));
    }
}

void Level::spawn_hard_enemy() {
    KinematicBody* hard_enemy = Object::cast_to<KinematicBody>(hard_enemy_scene->instance());
    int rand_x = -10 + (rand() % 20);
    int rand_y = -15 + (rand() % 30);
    level->add_child(hard_enemy, true);
    hard_enemy->global_translate(Vector3(rand_x, rand_y, 0));
    
}

}
