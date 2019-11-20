#include <stdlib.h>
#include <stdbool.h>

#include "SDL.h"
#include "SDL_image.h"

#define CELL_SIZE 32

typedef enum {LEFT, RIGHT, NONE} direction_t;

typdef struct{
  direction_t direction;
  float speed;
} movement_t;

typdef struct{
  int pos;
  movement_t movement;
  SDL_Texture *texture;
  SDL_Rect sprite_rect;
  SDL_Rect pos_rect;
} character_t;


void move(character_t &dude, direction_t dir, float speed){
  dude->movement.direction = dir;
  dude->movement.speed = speed;
  switch(dir){
    case LEFT:
      dude->pos_rect.x-=1;
      dude->pos = pos_rect.x / CELL_SIZE;
      break;
    case RIGHT:
      dude->pos_rect.x+=1;
      break;
    default:
      break;
  }
}

bool is_moving(character_t dude){
  return !(dude.pos * CELL_SIZE == dude.pos_rect.x);
}

void process_movement(character_t &dude){
  if(is_moving(&dude)){
    dude->movement.direction = NONE;
    dude->movement.speed = 0;
    return;
  }
  else{
    switch(dude->movement.direction){
      case LEFT:
        dude->pos_rect.x -=1;
        break;
      case RIGHT:
        dude->pos_rect.x +=1;
        break;
      default:
        break;
    }
  }
}

int main(int argc, char* argv[]){

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *surface;
  SDL_Texture *background_texture;
  SDL_Texture *char_texture;
  SDL_Event event;
  SDL_Rect srcrect, sprite_rect, char_rect;
  const Uint8 *states = SDL_GetKeyboardState(NULL);
  const int floor=401;
  bool at_left_end=true;
  bool at_right_end=false;
  bool quit=false;
  srcrect.x=0;
  srcrect.y=260;
  srcrect.w=346;
  srcrect.h=260;
  const Uint32 movement_duration = 300;
  long move_start;
  long loop_start;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    return 3;
  }

  if (SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
    return 3;
  }

  SDL_RenderSetLogicalSize(renderer, 640,480);

  surface = IMG_Load("assets/background.bmp");
  if (!surface) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    return 3;
  }
  background_texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!background_texture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    return 3;
  }
  SDL_FreeSurface(surface);

  surface = IMG_Load("assets/character_sprites.png");
  if (!surface) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    return 3;
  }
  char_texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!char_texture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    return 3;
  }
  SDL_FreeSurface(surface);

  sprite_rect.x=80;
  sprite_rect.y=1;
  sprite_rect.w=16;
  sprite_rect.h=32;

  char_rect.w=32;
  char_rect.h=64;
  char_rect.x=309;
  char_rect.y=floor-char_rect.h;

  character_t mario;
  mario.texture = char_texture;
  mario.sprite_rect = sprite_rect;
  mario.pos_rect = char_rect;
  mario.pos = mario.pos_rect/32;

  while (!quit) {
    loop_start= SDL_GetTicks();
    while(SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT) {
        quit=true;
      }
    }
    if(! is_moving(mario)){
      if( states[SDL_SCANCODE_LEFT]){
        if(!at_left_end){
          move.direction=1;
          at_right_end=false;
          if(srcrect.x==0)
          at_left_end=true;
        }
        else{

        }
      }

      if(states[SDL_SCANCODE_RIGHT] && !at_right_end){
        srcrect.x+=10;
        at_left_end=false;
        if(srcrect.x+srcrect.w==3440)
        at_right_end=true;
      }
    }



    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background_texture,&srcrect , NULL);
    SDL_RenderCopy(renderer, char_texture,&sprite_rect,&char_rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(34-(SDL_GetTicks()-loop_start));
  }

  SDL_DestroyTexture(background_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
