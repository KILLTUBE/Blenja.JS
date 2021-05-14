#ifndef TEXT_DUKTAPE
#define TEXT_DUKTAPE

#include "duktape.h"

extern duk_context *ctx;

typedef void (*text_duktape_lines_each_callback)(int i, char *str);

duk_bool_t js_push_global_by_name(char *name);
void text_duktape_eval(const char *code);
void js_add_function(char* name, duk_c_function func, duk_idx_t nargs);
void text_duktape_init();
void text_duktape_lines_each(text_duktape_lines_each_callback cb);
int js_eval_file_safe(char* filename);
int js_call(char* function, char* params, ...);
int jsfunc_file_get_contents(duk_context* ctx);
int jsfunc_file_put_contents(duk_context* ctx);
int jsfunc_get_global(duk_context* ctx);
int js_printf(char* msg, ...);
void js_reload();
int jsfunc_reload(duk_context *ctx);
int jsfunc_include(duk_context* ctx);
int jsfunc_exe(duk_context *ctx);
int jsfunc_exedir(duk_context *ctx);

#endif
