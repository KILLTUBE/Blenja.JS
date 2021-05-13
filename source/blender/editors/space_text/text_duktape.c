#include "text_duktape.h"

duk_context *ctx = NULL;
void *lines = NULL;

void text_duktape_eval(const char *code)
{
  duk_peval_string(ctx, code);
  printf("text_duktape_eval> %s\n", duk_safe_to_string(ctx, -1));
  duk_pop(ctx);
}

duk_ret_t text_duktape_log(duk_context *ctx)
{
  printf("%s\n", duk_to_string(ctx, 0));
  return 0; /* no return value (= undefined) */
}

void text_duktape_fatal(void *userdata, const char *msg)
{
  (void)userdata; /* ignored in this case, silence warning */

  /* Note that 'msg' may be NULL. */
  fprintf(stderr, "*** FATAL DUKTAPE ERROR: %s\n", (msg ? msg : "no message"));
  fflush(stderr);
  //abort();
}

void text_duktape_init() {
  if (ctx != NULL) {
    return;
  }
  // Create context with error handler
  ctx = duk_create_heap(NULL, NULL, NULL, NULL, text_duktape_fatal);
  // global.lines = [];
  duk_push_array(ctx);
  lines = duk_get_heapptr(ctx, -1);
  duk_put_global_string(ctx, "lines");
  // add some test data into global.lines
  duk_eval_string_noresult(ctx, "lines.push(1, 2, 3)");
  // add log function
  duk_push_c_function(ctx, text_duktape_log, 1 /*nargs*/);
  duk_put_global_string(ctx, "log");
}

void text_duktape_lines_each(text_duktape_lines_each_callback cb)
{
  duk_push_heapptr(ctx, lines);
  int n = duk_get_length(ctx, -1);
  for (int i=0; i<n; i++) {
    duk_get_prop_index(ctx, -1, i);
    const char *str = duk_to_string(ctx, -1);
    cb(i, str);
    duk_pop(ctx);
  }
  cb(1, "second");
  cb(2, "third");
}
