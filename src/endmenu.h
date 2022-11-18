#ifndef ENDMENU_H
#define ENDMENU_H

#include "Common.h"
#include <SceneTree.hpp>
#include "Control.hpp"
#include "Button.hpp"
#include "Input.hpp"
#include "InputEvent.hpp"
#include "Control.hpp"
#include "player.h"
#include "level.h"
#include <AudioStreamPlayer.hpp>
#include <Label.hpp>

#include <string>

namespace EndMenu 
{
class EndMenu : public Control 
{
    GODOT_CLASS(EndMenu, Control);

private:

    Input* input;
    bool isPaused;
    Control* endMenu;
    Player::Player* player;
    Level::Level* level;
    Label* score;

public:

    static void _register_methods();

    void _init();
    void _ready();
    void _process(float delta);

    void _input(InputEvent* event);

    void _on_main_menu_pressed();
    void _on_quit_pressed();
};
}

#endif
