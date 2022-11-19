This project is a tribute to Star Fox 64, an absolute classic of a game that shaped my childhood in a big way. It's an on-rails shooter with an infinite, procedurally-generated level, much like mobile infinite-runner games.

Controls:

WASD - Move the reticle

Shift/Space - Roll left/right

Enter - Shoot laser

Esc - Pause

**Build Process:**
1) Clone the repository with: 
```
$ git clone --recurse-submodules -j8 https://github.com/Kolarias/Star-Fox-Forever.git
```
This clones the repository, the submodule godot-cpp, and that submodule's submodule godot-headers.

2) Run: 
```
$ cd godot-cpp
```
3) From within godot-cpp, we will need to generate the godot-cpp bindings. Run:
```
$ python3 /usr/bin/scons platform=linux generate_bindings=yes -j4 use_custom_api_file=yes custom_api_file=../api.json
```
If you have the scons alias, replace 'python 3 /usr/bin/scons' with just 'scons'. Replace 'linux' with whatever your platform is. This step will take a while; it generates the godot-cpp bindings necessary for GDNative. You can also copy in your pre-generated godot-cpp bindings if you have them. 

4) Run:
```
$ cd ..
$ python3 /usr/bin/scons platform=linux
```
Again, replace 'linux' if needed. Again, use the alias if you have it.

5) Import Project/project.godot into Godot and run the project!
