#include "../include/main.hpp"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>

//globabl variables
SDL_Window *gWindow = NULL;

SDL_Texture *gTexture = NULL;

SDL_Renderer *gRenderer = NULL;

bool init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    
    if(gWindow == NULL)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if(gRenderer == NULL){
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("IMG could not be initialized! IMG Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void close() 
{
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

bool loadMedia() 
{
    gTexture = IMG_LoadTexture(gRenderer, "resources/samus.png");
    if(gTexture == NULL)
    {
        printf("failed to load image! IMG Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

int main(int argc, char *args[])
{
    if(!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        if(!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            bool quit = false;

            SDL_Event e;

            Uint64 last, curr;

            int alpha = 0;

            while(!quit)
            {
                last = SDL_GetTicks64();
                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    else if(e.type == SDL_KEYDOWN)
                    {
                        switch (e.key.keysym.sym) 
                        {
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                            case SDLK_UP:
                                alpha += 5;
                                break;
                            case SDLK_DOWN:
                                alpha -= 5;
                                break;
                        }
                    }
                }

                SDL_SetRenderDrawColor(gRenderer, 0xF0, 0xF0, 0xF0, 0xFF);
                SDL_RenderClear(gRenderer);

                int w,h;
                SDL_QueryTexture(gTexture, NULL, NULL, &w, &h);

                SDL_Rect src = {260, 0, 32 * 5, 43 * 5};
                SDL_Rect dst = {0, 0, 132 * 5, 160 * 5};

                SDL_SetTextureBlendMode(gTexture, SDL_BLENDMODE_BLEND);

                SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
                
                SDL_RenderCopy(gRenderer, gTexture, &src, &dst);

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, alpha);

                SDL_Rect rect = {0, 0, 132 * 5, 160 * 5};
                
                SDL_RenderFillRect(gRenderer, &rect);

                SDL_RenderPresent(gRenderer);

                curr = SDL_GetTicks64();
                // printf("frames per second: %f\n", 1.0f / ((float)(curr - last) / 1000));
            }
        }
    }

    close();

    return 0;
}

