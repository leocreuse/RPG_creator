#include "map.h"
#include <stdlib.h>

SDL_Texture *load_sprite_from_file(SDL_Renderer *renderer, char *src, SDL_Rect *location){
  SDL_Surface *surf = IMG_Load(src);
  if (!surf) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    SDL_FreeSurface(surf);
    return NULL;
  }
  SDL_Surface *interm;
  if(!location){
    interm = surf;
  }
  else{
    interm = SDL_CreateRGBSurfaceWithFormat(0, location->w, location->h, 32, SDL_PIXELFORMAT_RGBA32);
    if (!interm) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
      SDL_FreeSurface(interm);
      return NULL;
    }
    SDL_BlitSurface(surf, location, interm, NULL);
    SDL_FreeSurface(surf);
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, interm);
  if (!texture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    SDL_FreeSurface(interm);
    return NULL;
  }
  SDL_FreeSurface(interm);
  return texture;
}




map_tile_t *map_tile_from_sprite(SDL_Texture *sprite, SDL_Rect dims, pos_t real_dims){
  map_tile_t *res = malloc(sizeof(map_tile_t));
  res->sprite = sprite;
  res->dims = dims;
  res->real_dims = real_dims;
  return res;
}

void destroy_map_tile(map_tile_t *map_texture){
  map_texture->sprite = NULL;
  free(map_texture);
}

bool render_tile(SDL_Renderer *renderer, map_tile_t *tile, pos_t cam_pos){
#ifdef PERF
  long start = SDL_GetPerformanceCounter();
#endif
  SDL_Rect src_rect, dest_rect;
  int wl, wr, ht, hb;
  wl = tile->dims.x > cam_pos.x ? tile->dims.x : cam_pos.x;
  ht = tile->dims.y > cam_pos.y ? tile->dims.y : cam_pos.y;
  wr = tile->dims.x + tile->dims.w > cam_pos.x + 20 ? cam_pos.x + 20 : tile->dims.x + tile->dims.w;
  hb = tile->dims.y + tile->dims.h > cam_pos.y + 15 ? cam_pos.y + 15 : tile->dims.y + tile->dims.h;
  dest_rect.x = CELL_SIZE * (wl - cam_pos.x);
  dest_rect.y = CELL_SIZE * (ht - cam_pos.y);
  dest_rect.w = CELL_SIZE * (wr - wl);
  dest_rect.h = CELL_SIZE * (hb - ht);
  if(dest_rect.w <= 0 || dest_rect.h <= 0){
#ifdef PERF
    printf("Perf_Counter : render_tile : %f ms\n",(double)((SDL_GetPerformanceCounter() - start)*1000)/SDL_GetPerformanceFrequency());
#endif
    //printf("not ok for rendering:\n");
    //printf("dst: x: %d, y: %d, w: %d h:%d\n",dest_rect.x, dest_rect.y, dest_rect.w, dest_rect.h);
    return false;
  }
  src_rect.x = (wl - tile->dims.x) * tile->real_dims.x / tile->dims.w;
  src_rect.y = (ht - tile->dims.y) * tile->real_dims.y / tile->dims.h;
  src_rect.w = (wr - wl) * tile->real_dims.x / tile->dims.w;
  src_rect.h = (hb - ht) * tile->real_dims.y / tile->dims.h;
  //printf("src: x: %d, y: %d, w: %d h:%d\n\n",src_rect.x, src_rect.y, src_rect.w, src_rect.h);
  bool res = SDL_RenderCopy(renderer, tile->sprite, &src_rect, &dest_rect) == 0;
#ifdef PERF
  printf("Perf_Counter : render_tile : %f ms\n",(double)((SDL_GetPerformanceCounter() - start)*1000)/SDL_GetPerformanceFrequency());
#endif
  return res;
}

bool render_map(SDL_Renderer *renderer, map_tile_t *tile_list[], int n_tiles, pos_t cam_pos){
#ifdef PERF
  long start = SDL_GetPerformanceCounter();
#endif
  bool res = true;
  for(int i=0; i<n_tiles; i++){
    res &= render_tile(renderer, tile_list[i], cam_pos);
  }
#ifdef PERF
printf("Perf_Counter : render_map : %f ms\n",(double)((SDL_GetPerformanceCounter() - start)*1000)/SDL_GetPerformanceFrequency());
#endif
  return res;
}
