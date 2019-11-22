#include <stdlib.h>
#include <stdbool.h>

#include "SDL.h"
#include "SDL_image.h"
#include "map.h"


int main(int argc, char* argv[]){

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;
  int loop_start;
  int w ,h;
  int spare1, spare2;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
    return 3;
  }

  SDL_RenderSetLogicalSize(renderer, 640,480);

  SDL_Texture *background = load_texture_from_file(renderer, "assets/snowdin_map.png");
  SDL_QueryTexture(background,&spare1,&spare2,&w,&h);
  SDL_Rect location = {.x=0, .y=0, .w=w, .h=h};
  sprite_t *big_sprite = sprite_from_tex(background, location);
  SDL_Rect dims = {.x=0, .y=0, .w=w/32, .h=h/32};
  map_tile_t *big_tile = map_tile_from_sprite(big_sprite, dims);
  pos_t cam = {.x=0, .y=0};

  bool quit = false;
  while (!quit) {
    loop_start = SDL_GetTicks();
    while(SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT) {
        quit=true;
      }
    }
    if(state[SDL_SCANCODE_LEFT] && cam.x >-10){
      cam.x-=1;
    }
    if(state[SDL_SCANCODE_RIGHT] && cam.x < w/32 - 10){
      cam.x+=1;
    }
    if(state[SDL_SCANCODE_UP] && cam.y > -7){
      cam.y-=1;
    }
    if(state[SDL_SCANCODE_DOWN] && cam.y < h/32 - 7){
      cam.y+=1;
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    render_tile(renderer, big_tile, cam);
    SDL_RenderPresent(renderer);
    SDL_Delay(34-(SDL_GetTicks()-loop_start));
  }
  destroy_map_tile(big_tile);
  destroy_sprite(big_sprite);
  SDL_DestroyTexture(background);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
