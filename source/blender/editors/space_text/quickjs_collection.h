#include "text_duktape.h"
#include <quickjs.h>

// From text_duktape.c
// TODO: Proper header
// Duktape
extern duk_context *ctx;
extern void *lines;
// QuickJS
extern JSRuntime *quickjs_runtime;
extern JSContext *quickjs_ctx;
// Blender
extern bContext *globalC;

void quickjs_funcs_collection();
JSValue quickjsfunc_collection_selected    (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_collection_main        (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_collection_get_name    (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_collection_get_children(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_collection_get_objects (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
