#ifndef MAP_H_
#define MAP_H_

#define CELL_SIZE 32
#define PERF
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct{
  int x;
  int y;
} pos_t;


typedef struct{
  SDL_Texture *sprite;
  pos_t real_dims;
  SDL_Rect dims; //cells
} map_tile_t;


SDL_Texture *load_sprite_from_file(SDL_Renderer *renderer, char *src, SDL_Rect *location);
map_tile_t *map_tile_from_sprite(SDL_Texture *sprite, SDL_Rect pos, pos_t real_dims;);
void destroy_map_tile(map_tile_t *map_texture);

bool render_tile(SDL_Renderer *renderer, map_tile_t *tile, pos_t cam_pos);
bool render_map(SDL_Renderer *renderer, map_tile_t *tile_list[], int n_tiles, pos_t cam_pos);

#endif /* !MAP_H_ */
