#include "map.h"
##include <stdlib>
map_texture_t *create_map_texture(char* src, pos_t pos){
  texture_t *background = malloc(sizeof(texture_t));
  background->texture = NULL;
  background->loaded = false;
  background->size = { .x=-1, .y=-1};
  background->src = src;


  map_texture_t *res = malloc(sizeof(map_texture_t));
  res->background = background;
  res->cell_pos = pos;
  return res;
}

void destroy_map_texture(map_texture_t *map_texture){
  SDL_DestroyTexture(map_texture->background->texture);
  free(map_texture->background);
  free(map_texture);
}

bool load_texture_in_map(map_texture_t *map_texture, map_texture_t ***map){
  SDL_Surface surf = IMG_Load(map_texture->background.src);
  if (!surf) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    return false;
  }
  map_texture->background->texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!*(map_texture->background->texture)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    return false;
  }
  map_texture->background->size.x = surf.x
  map_texture->background->size.y = surf.y;
  int min_x, min_y, max_x, max_y;
  min_x = map_texture->pos.x;
  min_y = map_texture->pos.y;
  max_x = min_x + surf.x / 32;
  max_y = min_y + surf.y / 32;
  for(int i=min_x; i < max_x; i++){
    for(int j=min_y; j < max_y; j++){
      map[i][j] = map_texture;
    }
  }
  map_texture->background.loaded = true;

  SDL_FreeSurface(surf);
  return true;
}

bool unload_texture_in_map(map_texture_t *map_texture, map_texture_t **map){
  map_texture->background.loaded = false;
  min_x = map_texture->pos.x;
  min_y = map_texture->pos.y;
  max_x = min_x + map_texture->background.size.x / 32;
  max_y = min_y + map_texture->background.size.y / 32;
  for(int i=min_x; i < max_x; i++){
    for(int j=min_y; j < max_y; j++){
      if(map[i][j]== map_texture){
        map[i][j] = NULL;
      }
    }
  }
  SDL_DestroyTexture(map_texture->background->texture);
  map_texture->background->texture = NULL;
  map_texture->background->loaded = false;
  return true;
}
