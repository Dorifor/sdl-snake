#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window *window;
SDL_Renderer *renderer;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_CreateWindowAndRenderer("Snake: The Game", 800, 600,
                              SDL_WINDOW_KEYBOARD_GRABBED, &window, &renderer);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) { return SDL_APP_CONTINUE; }

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
    break;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
