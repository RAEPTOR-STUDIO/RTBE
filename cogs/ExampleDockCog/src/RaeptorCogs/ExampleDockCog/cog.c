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

void update_cb(RaeptorPannelCog_pannel_t *pannel) {}

void draw_cb(RaeptorPannelCog_pannel_t *pannel) {
  // Scissor the dock area to prevent drawing outside of it
  RaeptorRHICog_begin_scissor_api()(pannel->x, pannel->y, pannel->width,
                                    pannel->height);
  RaeptorGraphicsCog_draw_rectangle_api()(
      pannel->x, pannel->y, pannel->width, pannel->height,
      (RaeptorContainers_color_rgba_t){100, 100, 100, 255});
  RaeptorGraphicsCog_draw_rectangle_api()(
      pannel->x + 2, pannel->y + 2, pannel->width - 4, pannel->height - 4,
      (RaeptorContainers_color_rgba_t){255, 255, 255, 255});
  char string[256];
  snprintf(string, sizeof(string), "Dock [%dx%d] at (%d,%d)", pannel->width,
           pannel->height, pannel->x, pannel->y);
  RaeptorTextCog_draw_text_api()(string, pannel->x + pannel->width / 2,
                                 pannel->y + pannel->height / 2, 20,
                                 (RaeptorContainers_color_rgba_t){0, 0, 0, 255},
                                 RAEPTOR_TEXT_COG_TEXT_ALIGNMENT_CENTER);
  RaeptorRHICog_end_scissor_api()();
}

dynamic_array(RaeptorPannelCog_dock_t *) docks = {0};

void cog_on_start() {
  if (RaeptorPannelCog_dock_connect_api() && RTBE_add_dock_api()) {
    RaeptorPannelCog_dock_t *dock =
        RaeptorPannelCog_dock_create_api()(50, 50, 300, 200);
    RaeptorPannelCog_pannel_set_user_update_callback_api()(
        (RaeptorPannelCog_pannel_t *)dock, update_cb);
    RaeptorPannelCog_pannel_set_user_draw_callback_api()(
        (RaeptorPannelCog_pannel_t *)dock, draw_cb);
    RTBE_add_dock_api()(dock);
    dynamic_array_push(&docks, dock);
  } else
    printf("Dock API not available.\n");
}

void cog_on_detach(void) {
  dynamic_array_foreach(&docks, i, dock, {
    RTBE_remove_dock_api()(dock);
    RaeptorPannelCog_pannel_destroy_api()((RaeptorPannelCog_pannel_t **)&dock);
  });
  dynamic_array_free(&docks);
}
