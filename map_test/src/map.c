#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int find_name(char target[],char *names[], int num_sprites){
  for(int i=0; i< num_sprites; i++){
    if(!strcmp(names[i], target)){
      return i;
    }
  }
  return num_sprites;
}

map_tile_t **load_from_csv(SDL_Renderer *renderer, char *sprite_csv, char *map_csv){

  int len = strlen(sprite_csv);
  char *sheet_src = malloc(sizeof(char)*(len+1));
  strcpy(sheet_src, sprite_csv);
  sheet_src[len-3]='p';
  sheet_src[len-2]='n';
  sheet_src[len-1]='g';
  SDL_Surface *sheet_surf = IMG_Load(sheet_src);
  if (!sheet_surf) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    SDL_FreeSurface(sheet_surf);
    return NULL;
  }
  free(sheet_src);

  FILE *sprite_sheet = fopen(sprite_csv, "r");
  if(sprite_sheet == NULL){
    printf("Could not open sprite sheet CSV\n");
    return NULL;
  }

  int num_sprites = 0;
  char tmp;
  do{
    tmp = fgetc(sprite_sheet);
    if(tmp == '\n')
      num_sprites++;
  }while(tmp !=EOF);
  char **names = malloc(num_sprites*sizeof(char*));
  for(int i=0; i< num_sprites; i++){
    names[i] = malloc(13*sizeof(char));
  }
  SDL_Texture **sprites = malloc(num_sprites*sizeof(SDL_Texture*));
  pos_t *real_sizes = malloc(num_sprites*sizeof(pos_t));

  SDL_Rect loc;
  int res;
  SDL_Surface *extracted;
  rewind(sprite_sheet);
  for(int i=0; i<num_sprites; i++){
    if(fscanf(sprite_sheet, "%s %d %d %d %d\n",names[i], &(loc.x), &(loc.y), &(loc.w), &(loc.h)) != 5){
      printf("Error while reading sprite def CSV");
      return NULL;
    }
    real_sizes[i].x=loc.w;
    real_sizes[i].y=loc.h;
    extracted = SDL_CreateRGBSurfaceWithFormat(0, loc.w, loc.h, 32, SDL_PIXELFORMAT_RGBA32);
    if (!extracted) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
      SDL_FreeSurface(extracted);
      return NULL;
    }
    SDL_BlitSurface(sheet_surf, &loc, extracted, NULL);
    sprites[i] = SDL_CreateTextureFromSurface(renderer, extracted);
    if (!sprites[i]) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
      SDL_FreeSurface(extracted);
      return NULL;
    }
    SDL_FreeSurface(extracted);
  }
  SDL_FreeSurface(sheet_surf);

  FILE *map_def = fopen(map_csv, "r");
  if(map_def == NULL){
    printf("Could not open map definition CSV\n");
    return NULL;
  }

  int num_tiles = 0;
  do{
    tmp = fgetc(map_def);
    if(tmp == '\n')
      num_tiles++;
  }while(tmp != EOF);
  rewind(map_def);
  char name[13];
  map_tile_t ** tiles = malloc(num_tiles*sizeof(map_tile_t*));
  int index = 0;
  for(int i=0; i<num_tiles; i++){
    if(fscanf(map_def, "%s %d %d %d %d\n",name, &(loc.x), &(loc.y), &(loc.w), &(loc.h)) !=5){
      printf("Error while reading map definition\n");
      return NULL;
    }
    index = find_name(name, names, num_sprites);
    if(index == num_sprites){
      printf("Could not find sprite %s in %s\n", name, sprite_csv);
      return NULL;
    }
    tiles[i] = map_tile_from_sprite(sprites[index], loc, real_sizes[index]);
  }
  for(int i=0; i<num_sprites; i++){
    free(names[i]);
  }
  free(names);
  free(real_sizes);
  return tiles;
}


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
