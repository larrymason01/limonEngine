//
// Created by Engin Manap on 10.02.2016.
//

#include "SDL2Helper.h"

SDL2Helper::SDL2Helper(const char* title, int height, int width) {



    if (SDL_Init(SDL_INIT_VIDEO) < 0) { /* Initialize SDL's Video subsystem */
        std::cout << "Unable to initialize SDL";
        throw;
    }

    /* Request opengl 4.4 context. */
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Create our window centered at 512x512 resolution */
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  height, width, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window){ /* Die if creation failed */
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw;
    }

    /* Create our opengl context and attach it to our window */
    context = SDL_GL_CreateContext(window);

    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);

}

void SDL2Helper::mapInput() {
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                inputStates["quit"] = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        inputStates["quit"] = true;
                        break;
                }
                break;
        }
    }
}

SDL2Helper::~SDL2Helper() {
    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}