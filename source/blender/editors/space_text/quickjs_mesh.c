#include "text_duktape.h"
#include <quickjs.h>

JSValue addmesh(bContext *C) {
  Main *bmain;
  Scene *scene;
  ViewLayer *view_layer;
  struct Object *obedit;
  struct Mesh *mesh;
  // #########################
  bmain = CTX_data_main(C);
  scene = CTX_data_scene(C);
  view_layer = CTX_data_view_layer(C);
  obedit = BKE_object_add(bmain, /*scene,*/ view_layer, OB_MESH, "xxx");
  mesh = (struct Mesh *)obedit->data;

  mesh->totvert = 4; /* total number of vertices */
  mesh->mvert   = (struct MVert *)CustomData_add_layer(&mesh->vdata, CD_MVERT, CD_CALLOC, NULL, mesh->totvert);
  // 0
  mesh->mvert[0].co[0] = -1;
  mesh->mvert[0].co[1] = -1;
  mesh->mvert[0].co[2] =  0;
  // 1
  mesh->mvert[1].co[0] = -1;
  mesh->mvert[1].co[1] =  1;
  mesh->mvert[1].co[2] =  0;
  // 2
  mesh->mvert[2].co[0] = 1;
  mesh->mvert[2].co[1] = 1;
  mesh->mvert[2].co[2] = 0;
  // 3
  mesh->mvert[3].co[0] =  1;
  mesh->mvert[3].co[1] = -1;
  mesh->mvert[3].co[2] =  0;

  mesh->totpoly = 1;  /* this is the total number of faces */
  mesh->totloop = 4;  /* this is the total number of vertices required to describe the faces */
            /* Since we're making a single quad here, this value is 4, if we had chosen */
            /* to make 2 triangles, we would have needed 6 to properly describe both triangles */

  mesh->mpoly   = (MPoly *)CustomData_add_layer(&mesh->pdata, CD_MPOLY, CD_CALLOC, NULL, mesh->totpoly);
  mesh->mloop   = (MLoop *)CustomData_add_layer(&mesh->ldata, CD_MLOOP, CD_CALLOC, NULL, mesh->totloop);
  
  mesh->mloop[0].v = 0;
  mesh->mloop[1].v = 1;
  mesh->mloop[2].v = 2;
  mesh->mloop[3].v = 3;

  mesh->mpoly[0].loopstart = 0;
  mesh->mpoly[0].totloop = 4;

  /* Too lazy to add normals + edges myself, edges seem really needed */
  /* BKE_mesh_validate will do the tedious work for us. */
  BKE_mesh_calc_normals(mesh);
  BKE_mesh_validate(mesh, false, false);

  /* Tell blender things have changed */
  DEG_id_tag_update(&scene->id, /*DEG_TAG_COPY_ON_WRITE*/0);
  DEG_relations_tag_update(bmain);
  WM_event_add_notifier(C, NC_OBJECT | ND_TRANSFORM, NULL);
  return JS_MKPTR(JS_TAG_INT, mesh);
}

JSValue quickjsfunc_addmesh(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  JSValue mesh;
  //bContext *C;
  // #########################
  //if (argc != 1) {
  //  js_printf("addmesh> missing arguments[0]: bContext *C\n");
  //  return JS_FALSE;
  //}
  //if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
  //  js_printf("addmesh> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
  //  return JS_FALSE;
  //}
  //C = JS_VALUE_GET_PTR(argv[0]);
  //mesh = addmesh(C);
  mesh = addmesh(globalC);
  return mesh;
}

JSValue quickjsfunc_mesh_set_vertid_xyz_val(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int vertid        = 0;
  int xyz           = 0;
  float val         = 0.0;
  int valTag        = 0;
  // #########################
  if (argc != 4) {
    js_printf(__FUNCTION__ "> expecting four arguments (mesh pointer, vertid, 0-2 for xyz, new float value)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  vertid = JS_VALUE_GET_INT(argv[1]);
  xyz    = JS_VALUE_GET_INT(argv[2]);
  valTag = JS_VALUE_GET_TAG(argv[3]);
  if (valTag == JS_TAG_INT) {
    val = JS_VALUE_GET_INT(argv[3]);
  } else if (valTag == JS_TAG_FLOAT64) {
    val = JS_VALUE_GET_FLOAT64(argv[3]);
  }
  mesh->mvert[vertid].co[xyz] = val;
  return JS_TRUE;
}

JSValue quickjsfunc_mesh_get_vertid_xyz_val(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int vertid        = 0;
  int xyz           = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting three arguments (mesh pointer, vertid, 0-2 for xyz)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  vertid = JS_VALUE_GET_INT(argv[1]);
  xyz    = JS_VALUE_GET_INT(argv[2]);
  return JS_NewFloat64(quickjs_ctx, mesh->mvert[vertid].co[xyz]);
}

JSValue quickjsfunc_mesh_update(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (mesh pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh = JS_VALUE_GET_PTR(argv[0]);
  // As found in `blender\editors\mesh\mesh_data.c` file:
  // void ED_mesh_update(Mesh *mesh, bContext *C, bool calc_edges, bool calc_edges_loose)
  DEG_id_tag_update(&mesh->id, 0);
  WM_event_add_notifier(globalC, NC_GEOM | ND_DATA, mesh);
  return JS_TRUE;
}

JSValue quickjsfunc_mesh_totvert(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (mesh pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh = JS_VALUE_GET_PTR(argv[0]);
  return JS_MKVAL(JS_TAG_INT, mesh->totvert);
}

JSValue quickjsfunc_mesh_totedge(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (mesh pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh = JS_VALUE_GET_PTR(argv[0]);
  return JS_MKVAL(JS_TAG_INT, mesh->totedge);
}

JSValue quickjsfunc_mesh_totface(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (mesh pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh = JS_VALUE_GET_PTR(argv[0]);
  return JS_MKVAL(JS_TAG_INT, mesh->totface);
}

JSValue quickjsfunc_mesh_totselect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (mesh pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh = JS_VALUE_GET_PTR(argv[0]);
  return JS_MKVAL(JS_TAG_INT, mesh->totselect);
}

JSValue quickjsfunc_mesh_totpoly(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (mesh pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh = JS_VALUE_GET_PTR(argv[0]);
  return JS_MKVAL(JS_TAG_INT, mesh->totpoly);
}

JSValue quickjsfunc_mesh_totloop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  // #########################
  if (argc != 1) {
    js_printf(__FUNCTION__ "> expecting one argument (mesh pointer)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh = JS_VALUE_GET_PTR(argv[0]);
  return JS_MKVAL(JS_TAG_INT, mesh->totloop);
}
