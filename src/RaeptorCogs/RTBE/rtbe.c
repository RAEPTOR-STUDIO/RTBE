#include <RaeptorCogs/Cog/cog.h>
#include <RaeptorCogs/Cog/function_registry.h>
#include <RaeptorCogs/Cog/info.h>
#include <stdio.h>

void test_job_callback() {
  ((void (*)(FILE *, const char *))shared_context->fn_registry.get(
      "logger_log"))(stdout, "Test job executed.");
  ((void (*)(const char *))shared_context->fn_registry.get("unload_cog"))(
      "ExampleCog");
}

void logger_log(FILE *out, const char *message) {
  fputc('[', out);
  fputs(cog_name(), out);
  fputs("] ", out);
  fputs(message, out);
  fputc('\n', out);
}

const char *text(void) { return "This is a text from RTBE!"; }

void lazy_load_job_callback() {
  ((void (*)(const char *))shared_context->fn_registry.get("load_cog"))(
      "ExampleCog");
}

void cog_on_attach(void) {
  shared_context->fn_registry.bind("logger_log", logger_log);
  shared_context->fn_registry.bind("text", text);

  job_t *lazy_load_job = job_create();
  job_set_execute(lazy_load_job, lazy_load_job_callback);
  worker_add_job(&shared_context->workers.main_worker, lazy_load_job, -1);

  job_t *test_job = job_create();
  job_set_execute(test_job, test_job_callback);
  worker_add_job(&shared_context->workers.main_worker, test_job, 0);
}

void cog_on_detach(void) {
  shared_context->fn_registry.unbind("logger_log", logger_log);
  shared_context->fn_registry.unbind("text", text);
}
