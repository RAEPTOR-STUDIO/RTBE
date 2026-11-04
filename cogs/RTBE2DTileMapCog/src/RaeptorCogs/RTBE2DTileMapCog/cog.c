#define RAEPTOR_CONTAINERS_NO_PREFIX
#include <RaeptorCogs/Domain/GraphicsCog/shapes.h>
#include <RaeptorCogs/Domain/InputCog/mouse.h>
#include <RaeptorCogs/Domain/PannelCog/dock.h>
#include <RaeptorCogs/Domain/PannelCog/pannel.h>
#include <RaeptorCogs/Domain/RHICog/scissor.h>
#include <RaeptorCogs/Domain/RTBE/rtbe.h>
#include <RaeptorCogs/Domain/TextCog/text.h>
#include <RaeptorCogs/Domain/WindowCog/window.h>
#include <RaeptorContainers/dynamic_array.h>
#include <stdbool.h>
#include <stdio.h>

typedef uint32_t RTBE2DTileMapCog_tile_id_t;
typedef struct RTBE2DTileMapCog_tile {
  RaeptorContainers_color_rgba_t color;
} RTBE2DTileMapCog_tile_t;

typedef struct RTBE2DTileMapCog_tileset {
  dynamic_array(RTBE2DTileMapCog_tile_t) tiles;
} RTBE2DTileMapCog_tileset_t;

typedef struct RTBE2DTileMapCog_tilemap {
  RTBE2DTileMapCog_tileset_t *tileset;
  RTBE2DTileMapCog_tile_id_t *tiles;
  int width;
  int height;
} RTBE2DTileMapCog_tilemap_t;

RTBE2DTileMapCog_tilemap_t *RTBE2DTileMapCog_tilemap_create(int width,
                                                            int height) {
  RTBE2DTileMapCog_tilemap_t *tilemap =
      (RTBE2DTileMapCog_tilemap_t *)malloc(sizeof(RTBE2DTileMapCog_tilemap_t));
  tilemap->width = width;
  tilemap->height = height;
  tilemap->tiles = (RTBE2DTileMapCog_tile_id_t *)calloc(
      width * height, sizeof(RTBE2DTileMapCog_tile_id_t));
  return tilemap;
}

void RTBE2DTileMapCog_tilemap_set_tileset(RTBE2DTileMapCog_tilemap_t *tilemap,
                                          RTBE2DTileMapCog_tileset_t *tileset) {
  tilemap->tileset = tileset;
}

void RTBE2DTileMapCog_tilemap_set_tile(RTBE2DTileMapCog_tilemap_t *tilemap,
                                       int x, int y,
                                       RTBE2DTileMapCog_tile_id_t tile_id) {
  if (x < 0 || x >= tilemap->width || y < 0 || y >= tilemap->height) {
    return;
  }
  tilemap->tiles[y * tilemap->width + x] = tile_id;
}

void RTBE2DTileMapCog_tilemap_destroy(RTBE2DTileMapCog_tilemap_t **tilemap) {
  free((*tilemap)->tiles);
  free(*tilemap);
  *tilemap = NULL;
}

RTBE2DTileMapCog_tileset_t *RTBE2DTileMapCog_tileset_create() {
  RTBE2DTileMapCog_tileset_t *tileset =
      (RTBE2DTileMapCog_tileset_t *)malloc(sizeof(RTBE2DTileMapCog_tileset_t));
  dynamic_array_init(&tileset->tiles);
  return tileset;
}

void RTBE2DTileMapCog_tileset_add_tile(RTBE2DTileMapCog_tileset_t *tileset,
                                       RTBE2DTileMapCog_tile_t tile) {
  dynamic_array_push(&tileset->tiles, tile);
}

void RTBE2DTileMapCog_tileset_remove_tile(RTBE2DTileMapCog_tileset_t *tileset,
                                          int index) {
  if (index < 0 || index >= tileset->tiles.count) {
    return;
  }
  dynamic_array_remove(&tileset->tiles, index);
}

void RTBE2DTileMapCog_tileset_destroy(RTBE2DTileMapCog_tileset_t **tileset) {
  dynamic_array_free(&(*tileset)->tiles);
  free(*tileset);
  *tileset = NULL;
}

void RTBE2DTileMapCog_tilemap_draw(RTBE2DTileMapCog_tilemap_t *tilemap,
                                   RTBE2DTileMapCog_tileset_t *tileset, int x,
                                   int y, int tile_size) {
  for (int i = 0; i < tilemap->height; i++) {
    for (int j = 0; j < tilemap->width; j++) {
      RTBE2DTileMapCog_tile_id_t tile_id =
          tilemap->tiles[i * tilemap->width + j];
      if (tile_id < tileset->tiles.count) {
        RTBE2DTileMapCog_tile_t *tile = &tileset->tiles.items[tile_id];
        RaeptorGraphicsCog_draw_rectangle_api()(x + j * tile_size,
                                                y + i * tile_size, tile_size,
                                                tile_size, tile->color);
      }
    }
  }
}

RTBE2DTileMapCog_tilemap_t *tilemap;
RTBE2DTileMapCog_tileset_t *tileset;

void update_cb(RaeptorPannelCog_pannel_t *pannel) {
  pannel->flags |= RAEPTOR_PANNEL_COG_PANNEL_FLAG_DRAGGABLE;
  if (RaeptorInputCog_mouse_is_button_pressed_api()(
          RAEPTOR_INPUT_COG_MOUSE_BUTTON_LEFT)) {
    int mouse_x = RaeptorInputCog_mouse_get_x_api()();
    int mouse_y = RaeptorInputCog_mouse_get_y_api()();
    if (mouse_x >= pannel->x + 10 &&
        mouse_x <= pannel->x + pannel->width - 10 &&
        mouse_y >= pannel->y + 40 &&
        mouse_y <= pannel->y + pannel->height - 10) {
      pannel->flags &= ~RAEPTOR_PANNEL_COG_PANNEL_FLAG_DRAGGABLE;
    }
  }
}

void draw_cb(RaeptorPannelCog_pannel_t *pannel) {
  // Scissor the dock area to prevent drawing outside of it
  RaeptorRHICog_begin_scissor_api()(pannel->x, pannel->y, pannel->width,
                                    pannel->height);
  RaeptorGraphicsCog_draw_rectangle_outline_api()(
      pannel->x, pannel->y, pannel->width, pannel->height, 5,
      (RaeptorContainers_color_rgba_t){100, 100, 100, 255});
  RaeptorGraphicsCog_draw_rectangle_api()(
      pannel->x + 2, pannel->y + 2, pannel->width - 4, pannel->height - 4,
      (RaeptorContainers_color_rgba_t){150, 150, 255, 100});
  RaeptorRHICog_begin_scissor_api()(pannel->x + 10, pannel->y + 40,
                                    pannel->width - 20, pannel->height - 50);
  RaeptorGraphicsCog_draw_rectangle_api()(
      pannel->x + 10, pannel->y + 40, pannel->width - 20, pannel->height - 50,
      (RaeptorContainers_color_rgba_t){200, 200, 200, 100});
  RTBE2DTileMapCog_tilemap_draw(tilemap, tileset, pannel->x + 10,
                                pannel->y + 40, 20);
  const char *string = "RTBE2DTileMapChanged";
  RaeptorTextCog_draw_text_api()(string, pannel->x + pannel->width / 2,
                                 pannel->y + pannel->height / 2, 20,
                                 (RaeptorContainers_color_rgba_t){0, 0, 0, 255},
                                 RAEPTOR_TEXT_COG_TEXT_ALIGNMENT_CENTER);
  RaeptorRHICog_end_scissor_api()();
  RaeptorRHICog_end_scissor_api()();
}

RaeptorPannelCog_dock_t *dock;
void cog_on_start(void) {
  tileset = RTBE2DTileMapCog_tileset_create();
  RTBE2DTileMapCog_tile_t tile1 = {.color = {255, 0, 0, 255}};
  RTBE2DTileMapCog_tile_t tile2 = {.color = {0, 255, 0, 255}};
  RTBE2DTileMapCog_tileset_add_tile(tileset, tile1);
  RTBE2DTileMapCog_tileset_add_tile(tileset, tile2);
  tilemap = RTBE2DTileMapCog_tilemap_create(10, 10);
  RTBE2DTileMapCog_tilemap_set_tileset(tilemap, tileset);
  RTBE2DTileMapCog_tilemap_set_tile(tilemap, 0, 0, 0);
  RTBE2DTileMapCog_tilemap_set_tile(tilemap, 1, 0, 1);
  RTBE2DTileMapCog_tilemap_set_tile(tilemap, 2, 0, 0);
  RTBE2DTileMapCog_tilemap_set_tile(tilemap, 3, 0, 1);
  RTBE2DTileMapCog_tilemap_set_tile(tilemap, 4, 0, 0);
  RTBE2DTileMapCog_tilemap_set_tile(tilemap, 5, 0, 1);
  if (RaeptorPannelCog_dock_connect_api() && RTBE_add_dock_api()) {
    dock = RaeptorPannelCog_dock_create_api()(100, 100, 400, 300);
    RaeptorPannelCog_pannel_set_user_update_callback_api()(
        (RaeptorPannelCog_pannel_t *)dock, update_cb);
    RaeptorPannelCog_pannel_set_user_draw_callback_api()(
        (RaeptorPannelCog_pannel_t *)dock, draw_cb);
    RTBE_add_dock_api()(dock);
  } else
    printf("Dock API not available.\n");
}

void cog_on_detach(void) {
  RTBE_remove_dock_api()(dock);
  RaeptorPannelCog_pannel_destroy_api()((RaeptorPannelCog_pannel_t **)&dock);
  RTBE2DTileMapCog_tilemap_destroy(&tilemap);
  RTBE2DTileMapCog_tileset_destroy(&tileset);
}
