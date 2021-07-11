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

JSValue quickjsfunc_object_position(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  bContext *C           = NULL;
  Main *bmain           = NULL;
  Scene *scene          = NULL;
  struct Object *object = NULL;
  JSValue ret           = 0;
  // #########################
  C = globalC;
  bmain = CTX_data_main(C);
  scene = CTX_data_scene(C);
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (object pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  object = JS_VALUE_GET_PTR(argv[0]);
  ret = JS_NewArrayBuffer(quickjs_ctx, (uint8_t *) object->loc, sizeof(float) * 3, NULL, NULL, true);
  return ret;
}

JSValue quickjsfunc_object_update(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  bContext *C           = NULL;
  Main *bmain           = NULL;
  Scene *scene          = NULL;
  struct Object *object = NULL;
  // #########################
  C = globalC;
  bmain = CTX_data_main(C);
  scene = CTX_data_scene(C);
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (object pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  object = JS_VALUE_GET_PTR(argv[0]);
  // As in blender\editors\object\object_transform.c line 358
  /* tag for updates */
  DEG_id_tag_update(&object->id, ID_RECALC_TRANSFORM);
  return JS_TRUE;
}

JSValue quickjsfunc_object_children(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  bContext *C             = NULL;
  Main *bmain             = NULL;
  Scene *scene            = NULL;
  struct Object *object   = NULL;
  struct Object *ob_child = NULL;
  JSValue js_ret          = 0;
  JSValue js_child        = 0;
  int i                   = 0;
  // #########################
  C = globalC;
  bmain = CTX_data_main(C);
  scene = CTX_data_scene(C);
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (object pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  object = JS_VALUE_GET_PTR(argv[0]);
  js_ret = JS_NewArray(quickjs_ctx);
  // Code from blender\editors\object\object_transform.c line 542
  for (ob_child = bmain->objects.first; ob_child; ob_child = ob_child->id.next) {
    if (ob_child->parent == object) {
      //Object *ob_child_eval = DEG_get_evaluated_object(depsgraph, ob_child);
      js_child = JS_MKPTR(JS_TAG_INT, ob_child);
      js_array_push(quickjs_ctx, js_ret, 1, &js_child, false);
    }
  }
  return js_ret;
}

JSValue quickjsfunc_object_name_get(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  bContext *C             = NULL;
  Main *bmain             = NULL;
  Scene *scene            = NULL;
  struct Object *object   = NULL;
  JSValue js_ret          = 0;
  // #########################
  C = globalC;
  bmain = CTX_data_main(C);
  scene = CTX_data_scene(C);
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (object pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  object = JS_VALUE_GET_PTR(argv[0]);
  js_ret = JS_NewString(quickjs_ctx, object->id.name);
  return js_ret;
}

/*
e = getSelection() or selectedMesh()
e = new Entity("aaa")
mesh = e.mesh
mesh.vertices[0].x += 1 etc.
*/
JSValue quickjsfunc_object_mesh_get(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  bContext *C             = NULL;
  Main *bmain             = NULL;
  Scene *scene            = NULL;
  struct Object *object   = NULL;
  struct Mesh *mesh       = NULL;
  JSValue js_ret          = 0;
  // #########################
  C = globalC;
  bmain = CTX_data_main(C);
  scene = CTX_data_scene(C);
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (object pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  object = JS_VALUE_GET_PTR(argv[0]);
  if (object->type != OB_MESH) {
    return JS_FALSE;
  }
  mesh = (struct Mesh *)object->data;
  js_ret = JS_MKPTR(JS_TAG_INT, mesh);
  return js_ret;
}

JSValue quickjsfunc_object_reference_set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Object *object = NULL;
  // #########################
  if (argc != 2) {
    js_printf(__FUNCTION__ "> expecting two arguments (object pointer and entity reference)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  if (!JS_IsObject(argv[1])) {
    js_printf(__FUNCTION__ "> arguments[1] needs to be a reference to an entity\n");
    return JS_FALSE;
  }
  object = JS_VALUE_GET_PTR(argv[0]);
  *(JSValue *)&object->quickjs = argv[1];
  // Increase ref count, because C owns a pointer to it now
  JS_DupValue(quickjs_ctx, argv[1]);
  return JS_TRUE;
}

//

JSValue quickjsfunc_entity_jump_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  bContext *C           = NULL;
  struct Object *object = NULL;
  bool ret              = false;
  // #########################
  C = globalC;
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting two arguments (entity pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  object = JS_VALUE_GET_PTR(argv[0]);
  ret = ED_object_jump_to_object(C, object, true);
  return JS_MKVAL(JS_TAG_BOOL, ret);
}

JSValue quickjsfunc_entity_select(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  Object *object        = NULL;
  Object *baseobject    = NULL;
  bContext *C           = NULL;
  ViewLayer *view_layer = NULL;
  // #########################
  C = globalC;
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting two arguments (entity pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  view_layer = CTX_data_view_layer(C);
  object = JS_VALUE_GET_PTR(argv[0]);
  LISTBASE_FOREACH (Base *, base, &view_layer->object_bases) {
    baseobject = base->object;
    //ob->flag &= ~OB_DONE;
    //ob->id.tag &= ~LIB_TAG_DOIT;
    ///* parent may be in another scene */
    //if (ob->parent) {
    //  ob->parent->flag &= ~OB_DONE;
    //  ob->parent->id.tag &= ~LIB_TAG_DOIT;
    //}
    if (baseobject == object) {
      //void ED_object_base_select(Base *base, eObjectSelect_Mode mode)
      ED_object_base_select(base, BA_SELECT);
    }
  }
  return JS_TRUE;
}


JSValue quickjsfunc_entity_deselect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  Object *object        = NULL;
  Object *baseobject    = NULL;
  bContext *C           = NULL;
  ViewLayer *view_layer = NULL;
  // #########################
  C = globalC;
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting two arguments (entity pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  view_layer = CTX_data_view_layer(C);
  object = JS_VALUE_GET_PTR(argv[0]);
  LISTBASE_FOREACH (Base *, base, &view_layer->object_bases) {
    baseobject = base->object;
    //ob->flag &= ~OB_DONE;
    //ob->id.tag &= ~LIB_TAG_DOIT;
    ///* parent may be in another scene */
    //if (ob->parent) {
    //  ob->parent->flag &= ~OB_DONE;
    //  ob->parent->id.tag &= ~LIB_TAG_DOIT;
    //}
    if (baseobject == object) {
      //void ED_object_base_select(Base *base, eObjectSelect_Mode mode)
      ED_object_base_select(base, BA_DESELECT);
    }
  }
  return JS_TRUE;
}

// JS name: _selectedObjects (needs wrapper)
JSValue quickjsfunc_selectedObjects(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  bContext *C           = NULL;
  struct Object *object = NULL;
  //int i                 = 0;
  JSValue js_array      = 0;
  JSValue js_pointer    = 0;
  // #########################
  C = globalC;
  js_array = JS_NewArray(quickjs_ctx);
  CTX_DATA_BEGIN (C, Object *, object, selected_objects) {
    //printf("selectedObjects()[%d]: %s\n", i, object->id.name + 2);
    //i++;
    js_pointer = JS_MKPTR(JS_TAG_INT, object);
    js_array_push(quickjs_ctx, js_array, 1, &js_pointer, false);
  }
  CTX_DATA_END;
  return js_array;
}

// Called from blender\editors\object\object_add.c
// TODO: When deleted from Outliner, this is not called yet
void quickjs_delete_object(Object *object) {
  JSValue js_entity  = 0;
  JSValue js_destroy = 0;
  // #########################
  js_entity = *(JSValue *)&object->quickjs;
  //js_printf(__FUNCTION__ "> js_entity = %llu\n", js_entity);
  if (js_entity == 0) {
    return;
  }
  // TODO: Implement JS_CallMethod
  js_destroy = JS_GetPropertyStr(quickjs_ctx, js_entity, "_destroy");
  JS_Call(quickjs_ctx, js_destroy, js_entity, 0, NULL);
  JS_FreeValue(quickjs_ctx, js_destroy);
  // Decrease ref count, because C stopped owning the pointer
  JS_FreeValue(quickjs_ctx, js_entity);
}

/*
entities = _allEntities().map(Entity.fromPointer);
console.log(entities);
*/
JSValue quickjsfunc_allEntities(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  bContext *C             = NULL;
  Main *bmain             = NULL;
  Scene *scene            = NULL;
  struct Object *ob_child = NULL;
  JSValue js_ret          = 0;
  JSValue js_child        = 0;
  int i                   = 0;
  // #########################
  C = globalC;
  bmain = CTX_data_main(C);
  scene = CTX_data_scene(C);
  js_ret = JS_NewArray(ctx);
  // Code from blender\editors\object\object_transform.c line 542
  for (ob_child = bmain->objects.first; ob_child; ob_child = ob_child->id.next) {
    js_child = JS_MKPTR(JS_TAG_INT, ob_child);
    js_array_push(ctx, js_ret, 1, &js_child, false);
  }
  return js_ret;
}

void quickjs_funcs_object() {
  quickjs_add_function("object_position"        , quickjsfunc_object_position         , 1);
  quickjs_add_function("object_update"          , quickjsfunc_object_update           , 1);
  quickjs_add_function("object_children"        , quickjsfunc_object_children         , 1);
  quickjs_add_function("object_name_get"        , quickjsfunc_object_name_get         , 1);
  quickjs_add_function("object_mesh_get"        , quickjsfunc_object_mesh_get         , 1);
  quickjs_add_function("object_reference_set"   , quickjsfunc_object_reference_set    , 2);
  quickjs_add_function("_entity_jump_to"        , quickjsfunc_entity_jump_to          , 1);
  quickjs_add_function("_entity_select"         , quickjsfunc_entity_select           , 1);
  quickjs_add_function("_entity_deselect"       , quickjsfunc_entity_deselect         , 1);
  quickjs_add_function("_selectedObjects"       , quickjsfunc_selectedObjects         , 0);
  quickjs_add_function("_allEntities"           , quickjsfunc_allEntities             , 0);
}
