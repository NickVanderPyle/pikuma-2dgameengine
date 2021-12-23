#include "Game/Game.h"
#include <sol/sol.hpp>
#include <iostream>

void TestLua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    lua.script_file("./assets/scripts/myscript.lua");

    int some_variableInCpp = lua["some_variable"];
    std::cout << "The value of some_variable in c++ is " << some_variableInCpp << std::endl;
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
