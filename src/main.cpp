#include "Game/Game.h"
#include <sol/sol.hpp>
#include <iostream>

int nativeCppCubeFunction(int n) {
    return n*n*n;
}

void TestLua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    //how to bind c++ function to lua
    lua["cube"] = nativeCppCubeFunction;

    lua.script_file("./assets/scripts/myscript.lua");

    double someVariableInCpp = lua["some_variable"];
    std::cout << "The value of some_variable in c++ is " << someVariableInCpp << std::endl;

    sol::table config = lua["config"];
    bool isFullScreen = config["fullscreen"];
    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];
    std::cout << "isFullScreen:" << isFullScreen << " width:" << width << " height:" << height << std::endl;

    // invoke lua function
    sol::function functionFactorial = lua["factorial"];
    int functionResult = functionFactorial.call(5);
    std::cout << "functionResult:" << functionResult << std::endl;
}

int main(int argc, char *argv[]) {
//    Game game;
//
//    game.Initialize();
//    game.Run();
//    game.Destroy();

    TestLua();

    return 0;
}
