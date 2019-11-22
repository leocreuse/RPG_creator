#include "map.h"
#include <stdlib.h>

SDL_Texture *load_texture_from_file(SDL_Renderer *renderer, char *src){
  SDL_Surface *surf = IMG_Load(src);
  if (!surf) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    SDL_FreeSurface(surf);
    return NULL;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
  if (!texture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    SDL_FreeSurface(surf);
    return NULL;
  }
  SDL_FreeSurface(surf);
  return texture;
}


sprite_t *sprite_from_tex(SDL_Texture *texture, SDL_Rect location){
  sprite_t *res = malloc(sizeof(sprite_t));
  res->sheet = texture;
  res->location = location;
  return res;
}

map_tile_t *map_tile_from_sprite(sprite_t *sprite, SDL_Rect dims){
  map_tile_t *res = malloc(sizeof(map_tile_t));
  res->sprite = sprite;
  res->dims = dims;
  return res;
}

void destroy_sprite(sprite_t *sprite){
    sprite->sheet = NULL;
    free(sprite);
}

void destroy_map_tile(map_tile_t *map_texture){
  map_texture->sprite = NULL;
  free(map_texture);
}

bool render_tile(SDL_Renderer *renderer, map_tile_t *tile, pos_t cam_pos){
  SDL_Rect src_rect, dest_rect;
  int wl, wr, ht, hb;
  wl = tile->dims.x > cam_pos.x ? tile->dims.x : cam_pos.x;
  ht = tile->dims.y > cam_pos.y ? tile->dims.y : cam_pos.y;
  wr = tile->dims.x + tile->dims.w > cam_pos.x + 20 ? tile->dims.w + 20 : tile->dims.x + tile->dims.h;
  hb = tile->dims.y + tile->dims.h > cam_pos.y + 15 ? tile->dims.h + 15 : tile->dims.y + tile->dims.h;
  dest_rect.x = CELL_SIZE * (wl- cam_pos.x);
  dest_rect.y = CELL_SIZE * (ht - cam_pos.y);
  dest_rect.w = CELL_SIZE * (wr - wl);
  dest_rect.h = CELL_SIZE * (hb - ht);
  if(dest_rect.w <= 0 || dest_rect.h <= 0)
    return false;
#ifdef DEBUG
  printf("dst: x: %d, y: %d, w: %d h:%d\n",dest_rect.x, dest_rect.y, dest_rect.w, dest_rect.h);
#endif
  src_rect.x = (wl - tile->dims.x) * tile->sprite->location.w / tile->dims.w;
  src_rect.y = (ht - tile->dims.y) * tile->sprite->location.h / tile->dims.h;
  src_rect.w = (wr - wl) * tile->sprite->location.w / tile->dims.w;
  src_rect.h = (hb - ht) * tile->sprite->location.h / tile->dims.h;
#ifdef DEBUG
  printf("src: x: %d, y: %d, w: %d h:%d\n\n",src_rect.x, src_rect.y, src_rect.w, src_rect.h);
#endif
  return SDL_RenderCopy(renderer, tile->sprite->sheet, &src_rect, &dest_rect) == 0;

}
