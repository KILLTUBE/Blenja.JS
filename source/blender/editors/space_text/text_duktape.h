#ifndef TEXT_DUKTAPE
#define TEXT_DUKTAPE

#include "duktape.h"

// BLENDER
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "MEM_guardedalloc.h"
#include "DNA_anim_types.h"
#include "DNA_camera_types.h"
#include "DNA_collection_types.h"
#include "DNA_curve_types.h"
#include "DNA_gpencil_modifier_types.h"
#include "DNA_gpencil_types.h"
#include "DNA_key_types.h"
#include "DNA_light_types.h"
#include "DNA_lightprobe_types.h"
#include "DNA_material_types.h"
#include "DNA_meshdata_types.h"
#include "DNA_mesh_types.h"
#include "DNA_meta_types.h"
#include "DNA_object_fluidsim_types.h"
#include "DNA_object_force_types.h"
#include "DNA_object_types.h"
#include "DNA_pointcloud_types.h"
#include "DNA_scene_types.h"
#include "DNA_vfont_types.h"
#include "BLI_ghash.h"
#include "BLI_listbase.h"
#include "BLI_math.h"
#include "BLI_string.h"
#include "BLI_utildefines.h"
#include "BLT_translation.h"
#include "BKE_action.h"
#include "BKE_anim_data.h"
#include "BKE_armature.h"
#include "BKE_attribute.h"
#include "BKE_camera.h"
#include "BKE_collection.h"
#include "BKE_constraint.h"
#include "BKE_context.h"
#include "BKE_curve.h"
#include "BKE_displist.h"
#include "BKE_duplilist.h"
#include "BKE_effect.h"
#include "BKE_font.h"
#include "BKE_geometry_set.h"
#include "BKE_gpencil_curve.h"
#include "BKE_gpencil_geom.h"
#include "BKE_gpencil_modifier.h"
#include "BKE_hair.h"
#include "BKE_key.h"
#include "BKE_lattice.h"
#include "BKE_layer.h"
#include "BKE_lib_id.h"
#include "BKE_lib_query.h"
#include "BKE_lib_remap.h"
#include "BKE_light.h"
#include "BKE_lightprobe.h"
#include "BKE_main.h"
#include "BKE_material.h"
#include "BKE_mball.h"
#include "BKE_mesh.h"
#include "BKE_mesh_runtime.h"
#include "BKE_nla.h"
#include "BKE_object.h"
#include "BKE_particle.h"
#include "BKE_pointcloud.h"
#include "BKE_report.h"
#include "BKE_scene.h"
#include "BKE_speaker.h"
#include "BKE_volume.h"
#include "DEG_depsgraph.h"
#include "DEG_depsgraph_build.h"
#include "DEG_depsgraph_query.h"
#include "RNA_access.h"
#include "RNA_define.h"
#include "RNA_enum_types.h"
#include "UI_interface.h"
#include "WM_api.h"
#include "WM_types.h"
#include "ED_armature.h"
#include "ED_curve.h"
#include "ED_gpencil.h"
#include "ED_mball.h"
#include "ED_mesh.h"
#include "ED_node.h"
#include "ED_object.h"
#include "ED_outliner.h"
#include "ED_physics.h"
#include "ED_render.h"
#include "ED_screen.h"
#include "ED_select_utils.h"
#include "ED_transform.h"
#include "ED_view3d.h"
#include "UI_resources.h"
//#include "object_intern.h"

#include <QuickJS.h>

// Duktape
extern duk_context *ctx;
extern void *lines;
// QuickJS
extern JSRuntime *quickjs_runtime;
extern JSContext *quickjs_ctx;
// Blender
extern bContext *globalC;

typedef void (*text_duktape_lines_each_callback)(int i, char *str);

duk_bool_t js_push_global_by_name(char *name);
void text_duktape_eval(const char *code);
void js_add_function(char *name, duk_c_function func, duk_idx_t nargs);
JSValue quickjs_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
void quickjs_add_function(char *name, JSCFunction *funcPtr, int length);
void text_duktape_init();
void text_duktape_lines_each(text_duktape_lines_each_callback cb);
void quickjs_lines_each(text_duktape_lines_each_callback cb);
int js_eval_file_safe(char *filename);
int js_call(char *function, char *params, ...);
int jsfunc_file_get_contents(duk_context *ctx);
int jsfunc_file_put_contents(duk_context *ctx);
int jsfunc_get_global       (duk_context *ctx);
int js_printf(char *msg, ...);
void js_reload();
void quickjs_reload();
int jsfunc_reload (duk_context *ctx);
int jsfunc_include(duk_context *ctx);
int jsfunc_exe    (duk_context *ctx);
int jsfunc_exedir (duk_context *ctx);
// QuickJS
int jsfunc_JS_NewRuntime(duk_context *ctx);
int jsfunc_JS_NewContext(duk_context *ctx);
int jsfunc_JS_Eval(duk_context *ctx);
void quickjs_eval(char *buf);
void quickjs_set_bContext(bContext *C);


void quickjs_funcs_mesh();
void quickjs_funcs_object();

#endif
