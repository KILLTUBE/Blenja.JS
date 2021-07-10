#include "quickjs_collection.h"

void quickjs_funcs_collection() {
  quickjs_add_function("collection_first"            , quickjsfunc_collection_first        , 0);
  quickjs_add_function("collection_get_name"         , quickjsfunc_collection_get_name     , 1);
  quickjs_add_function("collection_get_children"     , quickjsfunc_collection_get_children , 1);
  quickjs_add_function("collection_get_objects"      , quickjsfunc_collection_get_objects  , 1);
}

// JS name: _collection_first (needs wrapper)
JSValue quickjsfunc_collection_first(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  bContext *C           = NULL;
  //struct Object *object = NULL;
  //int i                 = 0;
  //JSValue js_array      = 0;
  JSValue js_pointer    = 0;
  // #########################
  C = globalC;
  struct Collection *collection = CTX_data_collection(C);
  js_pointer = JS_MKPTR(JS_TAG_INT, collection);
  return js_pointer;
  //js_array = JS_NewArray(quickjs_ctx);
  //CTX_DATA_BEGIN (C, Object *, object, selected_objects) {
  //  //printf("selectedObjects()[%d]: %s\n", i, object->id.name + 2);
  //  //i++;
  //  js_pointer = JS_MKPTR(JS_TAG_INT, object);
  //  js_array_push(quickjs_ctx, js_array, 1, &js_pointer, false);
  //}
  //CTX_DATA_END;
  //return js_array;
}


JSValue quickjsfunc_collection_get_name(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Collection *collection = NULL;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (collection pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  collection = JS_VALUE_GET_PTR(argv[0]);
  return JS_NewString(ctx, collection->id.name);
}

JSValue quickjsfunc_collection_get_children(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Collection *collection       = NULL;
  JSValue js_ret                      = 0;
  JSValue js_child                    = 0;
  CollectionChild *collection_child   = 0;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (collection pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  collection = JS_VALUE_GET_PTR(argv[0]);
  collection_child = (CollectionChild *) collection->children.first;
  js_ret = JS_NewArray(quickjs_ctx);
  for (;collection_child; collection_child = collection_child->next) {
    js_child = JS_MKPTR(JS_TAG_INT, collection_child->collection);
    js_array_push(quickjs_ctx, js_ret, 1, &js_child, false);
  }
  return js_ret;
}

JSValue quickjsfunc_collection_get_objects(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Collection *collection       = NULL;
  JSValue js_ret                      = 0;
  JSValue js_child                    = 0;
  CollectionObject *collection_object = NULL;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (collection pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  collection = JS_VALUE_GET_PTR(argv[0]);
  collection_object = (CollectionObject *) collection->gobject.first;
  js_ret = JS_NewArray(quickjs_ctx);
  // Code from blender\editors\object\object_transform.c line 542
  for (; collection_object; collection_object = collection_object->next) {
    js_child = JS_MKPTR(JS_TAG_INT, collection_object->ob);
    js_array_push(quickjs_ctx, js_ret, 1, &js_child, false);
  }
  return js_ret;
}
