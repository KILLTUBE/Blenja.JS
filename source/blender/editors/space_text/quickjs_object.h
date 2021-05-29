#include <quickjs.h>

JSValue quickjsfunc_object_position        (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_object_update          (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_object_children        (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_object_name_get        (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_object_mesh_get        (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_object_reference_set   (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_selectedObjects        (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
