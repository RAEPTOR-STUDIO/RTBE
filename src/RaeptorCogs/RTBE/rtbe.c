#include <RaeptorCogs/Cog/cog.h>
#include <RaeptorCogs/Cog/function_registry.h>
#include <RaeptorCogs/Cog/info.h>
#include <RaeptorCogs/Domain/PannelCog/dock.h>
#include <RaeptorCogs/Domain/PrimeMover/cog.h>
#include <RaeptorCogs/Domain/PrimeMover/job.h>
#include <RaeptorCogs/Domain/PrimeMover/logger.h>
#include <RaeptorCogs/Domain/WindowCog/window.h>
#include <stdio.h>

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

const char *text(void) { return "This is a text from RTBE!"; }

void window_destroy_callback() { RaeptorWindowCog_window_destroy_api()(); }

void loop() {
  if (RaeptorPannelCog_draw_docks_api()) {
    RaeptorPannelCog_draw_docks_api()();
  }
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
  BindFunction(text);
}

void cog_on_start() {
  RaeptorPrimeMover_load_cog_api()("ExampleCog");
  RaeptorPrimeMover_unload_cog_api()("ExampleCog");

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
  UnbindFunction(RaeptorPrimeMover_logger_log);
  UnbindFunction(text);
}
