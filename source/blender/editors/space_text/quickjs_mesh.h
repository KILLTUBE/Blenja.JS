#include <quickjs.h>

JSValue quickjsfunc_addmesh                (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_vertid_xyz_val(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_vertid_xyz_val(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_edgeid_v1     (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_edgeid_v1     (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_edgeid_v2     (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_edgeid_v2     (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_update            (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totvert           (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totedge           (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totface           (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totselect         (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totpoly           (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totloop           (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
