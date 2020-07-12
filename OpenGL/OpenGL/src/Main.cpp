#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Game.h"
int main(void)
{
   // _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // windowShouldClose = window->shouldClose();
    Game* game = new Game();
    while (!game->shouldClose())
    {
        game->OnWindowUpdate();
        game->Update();
        game->Draw();
    }

    // end & clear up
    game->OnFinished();
    delete game;

    return 0;
}