#ifndef MAP_H_
#define MAP_H_

#include "SDL.h"
##include "SDL_image.h"

typedef struct{
  int x;
  int y;
} pos_t;

typedef struct{
  SDL_Texture *texture;
  bool loaded;
  char *src;
  pos_t size;
} texture_t;

typedef struct{
  texture_t *background;
  pos_t cell_pos;
} map_texture_t;

map_texture_t *create_map_texture(char* src, pos_t pos);
void destroy_map_texture(map_texture_t *map_texture);
bool load_texture_in_map(map_texture_t *map_texture, map_texture_t **map);
bool unload_texture_in_map(map_texture_t *map_texture, map_texture_t **map);
#endif /* !MAP_H_ */
