#include "level.h"
#include "player.h"
#include "GlobalConstants.hpp"
#include "KinematicCollision.hpp"

namespace Level {

void Level::_register_methods()
{
    register_method("_ready", &Level::_ready);
    register_method("_process", &Level::_process);
    register_method("_physics_process", &Level::_physics_process);
    register_method("_on_timeout", &Level::_on_timeout);
}

void Level::_init() {
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
    center_ball_scene = resourceLoader->load("res://level_objects/CenterBall.tscn");
    easy_enemy_scene = resourceLoader->load("res://enemies/EasyEnemy.tscn");
    animation_player = (AnimationPlayer*)(level->get_node("AnimationPlayer"));
    animation_player->play("Start");
}

void Level::_process(float delta) {
}

void Level::_physics_process(float delta) {
    // Update time
    time_helper(delta);
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
    StaticBody* center_ball = Object::cast_to<StaticBody>(center_ball_scene->instance());
    KinematicBody* easy_enemy = Object::cast_to<KinematicBody>(easy_enemy_scene->instance());
    if (!Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"))->game_ended) {
        level->add_child(center_ball, true);
        level->add_child(easy_enemy, true);
    }
}

}
