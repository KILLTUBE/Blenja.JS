#include <quickjs.h>

JSValue quickjsfunc_addmesh                  (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_from_buffers        (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_vertid_xyz      (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_vertid_xyz      (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_edgeid_v1       (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_edgeid_v1       (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_edgeid_v2       (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_edgeid_v2       (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_polyid_loopstart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_polyid_loopstart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_polyid_totloop  (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_polyid_totloop  (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_polyid_mat_nr   (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_polyid_mat_nr   (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_set_polyid_flag     (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_get_polyid_flag     (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_update              (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totvert             (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totedge             (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totface             (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totselect           (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totpoly             (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_mesh_totloop             (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
