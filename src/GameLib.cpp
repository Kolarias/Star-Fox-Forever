#include <player.h>
#include <menu.h>
#include <pausemenu.h>
#include <enemy.h>
#include <laser.h>
#include <level.h>
#include <levelobject.h>
#include <pausemenu.h>
#include <endmenu.h>


extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);

    godot::register_class<Player::Player>();
    godot::register_class<Menu::Menu>();
    godot::register_class<PauseMenu::PauseMenu>();
    godot::register_class<Enemy::Enemy>();
    godot::register_class<Laser::Laser>();
    godot::register_class<Level::Level>();
    godot::register_class<LevelObject::LevelObject>();
    godot::register_class<PauseMenu::PauseMenu>();
    godot::register_class<EndMenu::EndMenu>();
}