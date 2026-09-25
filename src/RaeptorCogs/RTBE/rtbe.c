#define RAEPTOR_CONTAINERS_NO_PREFIX
#include "RaeptorCogs/Domain/PannelCog/pannel.h"
#include <RaeptorCogs/Cog/cog.h>
#include <RaeptorCogs/Cog/function_registry.h>
#include <RaeptorCogs/Cog/info.h>
#include <RaeptorCogs/Domain/PannelCog/dock.h>
#include <RaeptorCogs/Domain/PannelCog/dock_space.h>
#include <RaeptorCogs/Domain/PrimeMover/cog.h>
#include <RaeptorCogs/Domain/PrimeMover/job.h>
#include <RaeptorCogs/Domain/PrimeMover/logger.h>
#include <RaeptorCogs/Domain/RHICog/draw.h>
#include <RaeptorCogs/Domain/TextCog/text.h>
#include <RaeptorCogs/Domain/WindowCog/window.h>
#include <RaeptorContainers/dynamic_array.h>
#include <stdio.h>

RaeptorPannelCog_dock_space_t *dock_space;
dynamic_array(RaeptorPannelCog_dock_t *) docks = {0};
RaeptorPannelCog_dock_space_t *RTBE_get_dock_space() { return dock_space; }

void RTBE_add_dock(RaeptorPannelCog_dock_t *dock) {
  dynamic_array_push(&docks, dock);
  if (dock_space) {
    RaeptorPannelCog_dock_connect_api()(dock_space, dock);
  }
}

void RTBE_remove_dock(RaeptorPannelCog_dock_t *dock) {
  for (size_t i = 0; i < docks.count; i++) {
    if (docks.items[i] == dock) {
      dynamic_array_remove(&docks, i);
      break;
    }
  }
  if (dock_space) {
    RaeptorPannelCog_dock_disconnect_api()(dock_space, dock);
  }
}

void test_job_callback() {
  RaeptorPrimeMover_logger_log_api()(stdout, "Test job executed.");
}

void RaeptorPrimeMover_logger_log(FILE *out, const char *message) {
  fputc('[', out);
  fputs(cog_name(), out);
  fputs("] ", out);
  fputs(message, out);
  fputc('\n', out);
}

void window_destroy_callback() { RaeptorWindowCog_window_destroy_api()(); }

void loop() {
  RaeptorPannelCog_dock_space_t *space = RTBE_get_dock_space();
  RaeptorPannelCog_pannel_update_api()((RaeptorPannelCog_pannel_t *)space);
  RaeptorRHICog_begin_draw_api()();
  RaeptorRHICog_clear_api()(
      (RaeptorContainers_color_rgba_t){255, 255, 255, 255});
  int w_width = RaeptorWindowCog_window_get_width_api()();
  int w_height = RaeptorWindowCog_window_get_height_api()();
  if (RaeptorTextCog_draw_text_api()) {
    RaeptorTextCog_draw_text_api()(
        "Welcome to RTBE", w_width / 2, w_height / 2, 20,
        (RaeptorContainers_color_rgba_t){100, 100, 100, 255},
        RAEPTOR_TEXT_COG_TEXT_ALIGNMENT_CENTER);
  }
  RaeptorPannelCog_pannel_draw_api()((RaeptorPannelCog_pannel_t *)space);
  RaeptorRHICog_end_draw_api()();
}

RaeptorParallel_job_t *window_job;
void main_loop() {
  if (RaeptorWindowCog_window_loop_api()(loop)) {
    RaeptorParallel_job_t *destroy_job = RaeptorParallel_job_create();
    RaeptorParallel_job_set_execute(destroy_job, window_destroy_callback);
    RaeptorPrimeMover_add_job_api()(destroy_job, -1);
    RaeptorPrimeMover_remove_job_api()(window_job);
  }
}
void cog_on_attach(void) {
  BindFunction(RaeptorPrimeMover_logger_log);
  BindFunction(RTBE_get_dock_space);
  BindFunction(RTBE_add_dock);
  BindFunction(RTBE_remove_dock);
}

void cog_on_start() {
  RaeptorPrimeMover_load_cog_api()("ExampleCog");
  RaeptorPrimeMover_unload_cog_api()("ExampleCog");

  dock_space = RaeptorPannelCog_dock_space_create_api()(0, 0, 800, 600);

  RaeptorParallel_job_t *test_job = RaeptorParallel_job_create();
  RaeptorParallel_job_set_execute(test_job, test_job_callback);
  RaeptorPrimeMover_add_job_api()(test_job, 0);

  RaeptorWindowCog_window_create_api()();
  RaeptorWindowCog_window_set_target_fps_api()(60);
  window_job = RaeptorParallel_job_create();
  RaeptorParallel_job_set_execute(window_job, main_loop);
  RaeptorParallel_job_set_repeat_count(window_job, -1);
  RaeptorPrimeMover_add_job_api()(window_job, 100);
}

void cog_on_detach(void) {
  RaeptorPannelCog_pannel_destroy_api()(
      (RaeptorPannelCog_pannel_t **)&dock_space);
  UnbindFunction(RaeptorPrimeMover_logger_log);
  UnbindFunction(RTBE_get_dock_space);
  UnbindFunction(RTBE_add_dock);
  UnbindFunction(RTBE_remove_dock);
}
