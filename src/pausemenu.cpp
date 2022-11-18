#include "pausemenu.h"
#include <Node.hpp>
#include <Viewport.hpp>
#include <NetworkedMultiplayerENet.hpp>

namespace PauseMenu
{
void PauseMenu::_register_methods()
{
    register_method("_ready", &PauseMenu::_ready);
    register_method("_process", &PauseMenu::_process);
    register_method("_input", &PauseMenu::_input);
    register_method("_on_resume_pressed", &PauseMenu::_on_resume_pressed);
    register_method("_on_main_menu_pressed", &PauseMenu::_on_main_menu_pressed);
    register_method("_on_quit_pressed", &PauseMenu::_on_quit_pressed);
}

void PauseMenu::_init() 
{
    isPaused = false;
    input = Input::get_singleton();
}

void PauseMenu::_ready() 
{
    pauseMenu = Object::cast_to<Control>(Node::get_node("."));

    Button* resume_button = Object::cast_to<Button>(Node::get_node("CenterContainer/VBoxContainer/Resume"));
    Button* main_menu_button = Object::cast_to<Button>(Node::get_node("CenterContainer/VBoxContainer/MainMenu"));
    Button* quit_button = Object::cast_to<Button>(Node::get_node("CenterContainer/VBoxContainer/Quit"));
    resume_button->connect("pressed", pauseMenu, "_on_resume_pressed");
    main_menu_button->connect("pressed", pauseMenu, "_on_main_menu_pressed");
    quit_button->connect("pressed", pauseMenu, "_on_quit_pressed");

    player = Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"));
}

void PauseMenu::_process(float delta){
}

void PauseMenu::_input(InputEvent* event)
{
    if (event->is_action_pressed("pause") && !player->game_ended) {
        isPaused = !isPaused;
        get_tree()->set_pause(isPaused);
        pauseMenu->set_visible(isPaused);
        if(!isPaused){
            handle_music();
        }
    }
}

void PauseMenu::_on_resume_pressed() 
{
    isPaused = false;
    get_tree()->set_pause(isPaused);
    pauseMenu->set_visible(isPaused);
    if (!isPaused){
        handle_music();
    }
}

void PauseMenu::handle_music(){
    AudioStreamPlayer* bgm_audio = player->bgm_audio;
    if (bgm_audio && player) {
        bgm_audio->set_stream_paused(player->mute);
    }
}

void PauseMenu::_on_main_menu_pressed() 
{
    isPaused = !isPaused;
    get_tree()->set_pause(isPaused);
    get_tree()->change_scene("res://Menu.tscn");
}

void PauseMenu::_on_quit_pressed() 
{
    get_tree()->quit();
}

}
