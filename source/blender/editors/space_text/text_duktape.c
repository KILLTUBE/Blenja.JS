#include "text_duktape.h"

duk_context *ctx = NULL;
void *lines = NULL;

void text_duktape_eval(const char *code)
{
  duk_eval_string(ctx, code);
  printf("text_duktape_eval> %s\n", duk_get_string(ctx, -1));
  duk_pop(ctx);
}

void text_duktape_init() {
  char *what;
  if (ctx == NULL) {
    ctx = duk_create_heap_default();

    duk_push_array(ctx);
    lines = duk_get_heapptr(ctx, -1);
    //duk_
    duk_put_global_string(ctx, "lines");

    duk_eval_string_noresult(ctx, "lines.push(1, 2, 3)");
  }

}

void text_duktape_lines_each(text_duktape_lines_each_callback cb)
{
  //duk_push_heapptr(ctx, lines);
  //duk_get_length(ctx, -1);
  //duk_get_prop_index

  cb(0, "first");
  cb(1, "second");
  cb(2, "third");
}
