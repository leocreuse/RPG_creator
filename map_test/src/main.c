#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
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
  // SDL_Texture *background = load_sprite_from_file(renderer, "assets/background.png", NULL);
  // SDL_QueryTexture(background,&spare1,&spare2,&w,&h);
  // SDL_Rect dims = {.x=0, .y=0, .w=w/32, .h=h/32};
  // pos_t big_real_dims = {.x=w, .y=h};
  // map_tile_t *big_tile = map_tile_from_sprite(background, dims, big_real_dims);
   pos_t cam = {.x=0, .y=0};
  // SDL_Rect smol_dims = {.x=w/64, .y=-h/64, .w=w/64, .h=h/64};
  // map_tile_t *smol_tile = map_tile_from_sprite(background,smol_dims, big_real_dims);
  //
  // map_tile_t *tile_list[2];
  // tile_list[0] = big_tile;
  // tile_list[1] = smol_tile;
  map_tile_t **map = load_from_csv(renderer, "assets/background.csv", "assets/map.csv");
  if(map==NULL){
    return -1;
  }
  bool quit = false;
  while (!quit) {
    loop_start = SDL_GetTicks();
    while(SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT) {
        quit=true;
      }
    }
    if(state[SDL_SCANCODE_LEFT] ){
      cam.x-=1;
    }
    if(state[SDL_SCANCODE_RIGHT] ){
      cam.x+=1;
    }
    if(state[SDL_SCANCODE_UP] ){
      cam.y-=1;
    }
    if(state[SDL_SCANCODE_DOWN] ){
      cam.y+=1;
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    //render_tile(renderer, smol_tile, cam);
    render_map(renderer, map, 2, cam);
    SDL_RenderPresent(renderer);
    SDL_Delay(34-(SDL_GetTicks()-loop_start));
  }
  //destroy_map_tile(big_tile);
  //SDL_DestroyTexture(background);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
