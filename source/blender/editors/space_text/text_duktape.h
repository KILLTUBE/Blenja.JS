#ifndef TEXT_DUKTAPE
#define TEXT_DUKTAPE

#include "duktape.h"

extern duk_context *ctx;

typedef void (*text_duktape_lines_each_callback)(int i, char *str);

void text_duktape_eval(const char *code);
void text_duktape_init();
void text_duktape_lines_each(text_duktape_lines_each_callback cb);

#endif
