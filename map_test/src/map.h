#ifndef MAP_H_
#define MAP_H_

#define CELL_SIZE 32

#include <stdbool.h>

#include "SDL.h"
#include "SDL_image.h"

typedef struct{
  int x;
  int y;
} pos_t;

typedef struct{
  SDL_Texture *sheet;
  SDL_Rect location; //pixels
} sprite_t;

typedef struct{
  sprite_t *sprite;
  SDL_Rect dims; //cells
} map_tile_t;

SDL_Texture *load_texture_from_file(SDL_Renderer *renderer, char *src);
sprite_t *sprite_from_tex(SDL_Texture *texture, SDL_Rect location);
map_tile_t *map_tile_from_sprite(sprite_t *sprite, SDL_Rect pos);
void destroy_sprite(sprite_t *sprite);
void destroy_map_tile(map_tile_t *map_texture);

bool render_tile(SDL_Renderer *renderer, map_tile_t *tile, pos_t cam_pos);

#endif /* !MAP_H_ */
