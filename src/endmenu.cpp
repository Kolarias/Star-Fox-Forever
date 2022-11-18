#include "endmenu.h"
#include <Node.hpp>
#include <Viewport.hpp>
#include <NetworkedMultiplayerENet.hpp>

namespace EndMenu
{
void EndMenu::_register_methods()
{
    register_method("_ready", &EndMenu::_ready);
    register_method("_process", &EndMenu::_process);
    register_method("_input", &EndMenu::_input);
    register_method("_on_main_menu_pressed", &EndMenu::_on_main_menu_pressed);
    register_method("_on_quit_pressed", &EndMenu::_on_quit_pressed);
}

void EndMenu::_init() 
{
    isPaused = false;
}

void EndMenu::_ready() 
{
    endMenu = Object::cast_to<Control>(Node::get_node("."));

    Button* main_menu_button = Object::cast_to<Button>(Node::get_node("CenterContainer/VBoxContainer/MainMenu"));
    Button* quit_button = Object::cast_to<Button>(Node::get_node("CenterContainer/VBoxContainer/Quit"));
    main_menu_button->connect("pressed", endMenu, "_on_main_menu_pressed");
    quit_button->connect("pressed", endMenu, "_on_quit_pressed");

    player = Object::cast_to<Player::Player>(Node::get_node("/root/Level/Player"));
    level = Object::cast_to<Level::Level>(Node::get_node("/root/Level"));
    score = Object::cast_to<Label>(endMenu->get_node("CenterContainer/VBoxContainer/Score"));
}

void EndMenu::_process(float delta){
    score->set_text(("Score: " + std::to_string(level->hits * static_cast<int>(level->time / 10))).c_str());
}

void EndMenu::_input(InputEvent* event) {
}

void EndMenu::_on_main_menu_pressed() 
{
    isPaused = !isPaused;
    get_tree()->set_pause(false);
    get_tree()->change_scene("res://Menu.tscn");
}

void EndMenu::_on_quit_pressed() 
{
    get_tree()->quit();
}

}
