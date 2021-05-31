#include "text_duktape.h"
#include <quickjs.h>

JSValue quickjsfunc_mesh_add(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  JSValue js_mesh;
  Main *bmain;
  Scene *scene;
  ViewLayer *view_layer;
  struct Object *obedit;
  struct Mesh *mesh;
  bContext *C;
  // #########################
  C = globalC;
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
  // Return mesh as pointer
  js_mesh = JS_MKPTR(JS_TAG_INT, mesh);
  return js_mesh;
}


JSValue quickjsfunc_mesh_from_buffers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  JSValue js_mesh;
  Main *bmain;
  Scene *scene;
  ViewLayer *view_layer;
  struct Object *obedit;
  struct Mesh *mesh;
  bContext *C;
  size_t psize_vertices;
  size_t psize_loops;
  size_t psize_polygons;
  float *vertices = NULL;
  int *loops      = NULL;
  int *polygons   = NULL;
  int vertices_n  = 0;
  int loops_n     = 0;
  int polygons_n  = 0;
  int i           = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting four arguments (vertices, loops, polygons)\n");
    return JS_FALSE;
  }
  
  JSValue js_vertices_arraybuffer;
  JSValue js_loops_arraybuffer;
  JSValue js_polygons_arraybuffer;
  js_vertices_arraybuffer = JS_GetTypedArrayBuffer(quickjs_ctx, argv[0], NULL, NULL, NULL);
  js_loops_arraybuffer    = JS_GetTypedArrayBuffer(quickjs_ctx, argv[1], NULL, NULL, NULL);
  js_polygons_arraybuffer = JS_GetTypedArrayBuffer(quickjs_ctx, argv[2], NULL, NULL, NULL);
  vertices = (float *) JS_GetArrayBuffer(quickjs_ctx, &psize_vertices, js_vertices_arraybuffer);
  loops    = (int *)   JS_GetArrayBuffer(quickjs_ctx, &psize_loops   , js_loops_arraybuffer   );
  polygons = (int *)   JS_GetArrayBuffer(quickjs_ctx, &psize_polygons, js_polygons_arraybuffer);
  vertices_n = psize_vertices / 12; // 3 floats = 3 * 4 = 12
  loops_n    = psize_loops    /  4; // 1 int    = 1 * 4 =  4
  polygons_n = psize_polygons /  8; // 2 ints   = 2 * 4 =  8
  printf("vertices psize=%d pointer=%p n=%d\n", psize_vertices, vertices, vertices_n);
  printf("loops    psize=%d pointer=%p n=%d\n", psize_loops   , loops   , loops_n   );
  printf("polygons psize=%d pointer=%p n=%d\n", psize_polygons, polygons, polygons_n);

  //if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
  //  js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
  //  return JS_FALSE;
  //}


  C = globalC;
  bmain = CTX_data_main(C);
  scene = CTX_data_scene(C);
  view_layer = CTX_data_view_layer(C);
  obedit = BKE_object_add(bmain, /*scene,*/ view_layer, OB_MESH, "xxx");
  mesh = (struct Mesh *)obedit->data;

  mesh->totvert = vertices_n; /* total number of vertices */
  mesh->mvert   = (struct MVert *)CustomData_add_layer(&mesh->vdata, CD_MVERT, CD_CALLOC, NULL, mesh->totvert);

  for (i=0; i<mesh->totvert; i++) {
    mesh->mvert[i].co[0] = vertices[i * 3 + 0];
    mesh->mvert[i].co[1] = vertices[i * 3 + 1];
    mesh->mvert[i].co[2] = vertices[i * 3 + 2];
  }
  
  mesh->totpoly = polygons_n;  /* this is the total number of faces */
  mesh->totloop = loops_n;  /* this is the total number of vertices required to describe the faces */
            /* Since we're making a single quad here, this value is 4, if we had chosen */
            /* to make 2 triangles, we would have needed 6 to properly describe both triangles */

  mesh->mpoly   = (MPoly *)CustomData_add_layer(&mesh->pdata, CD_MPOLY, CD_CALLOC, NULL, mesh->totpoly);
  mesh->mloop   = (MLoop *)CustomData_add_layer(&mesh->ldata, CD_MLOOP, CD_CALLOC, NULL, mesh->totloop);
  
  for (i=0; i<mesh->totloop; i++) {
    mesh->mloop[i].v = loops[i];
  }
  
  for (i=0; i<mesh->totpoly; i++) {
    mesh->mpoly[i].loopstart = polygons[i * 2 + 0];
    mesh->mpoly[i].totloop   = polygons[i * 2 + 1];
  }
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

// mesh->mvert

JSValue quickjsfunc_mesh_set_vertid_xyz(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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

JSValue quickjsfunc_mesh_get_vertid_xyz(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int vertid        = 0;
  int xyz           = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting three arguments (mesh pointer, vertid, 0-2 for x/y/z)\n");
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

// mesh->mloop

JSValue quickjsfunc_mesh_set_loopid_ev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int loopid        = 0;
  int e_or_v        = 0;
  float val         = 0.0;
  int valTag        = 0;
  // #########################
  if (argc != 4) {
    js_printf(__FUNCTION__ "> expecting four arguments (mesh pointer, vertid, 0-1 for 'e' or 'v', new float value)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  loopid = JS_VALUE_GET_INT(argv[1]);
  e_or_v = JS_VALUE_GET_INT(argv[2]);
  valTag = JS_VALUE_GET_TAG(argv[3]);
  if (valTag == JS_TAG_INT) {
    val = JS_VALUE_GET_INT(argv[3]);
  } else if (valTag == JS_TAG_FLOAT64) {
    val = JS_VALUE_GET_FLOAT64(argv[3]);
  }
  if (e_or_v == 0) {
    mesh->mloop[loopid].e = val;
  } else {
    mesh->mloop[loopid].v = val;
  }
  return JS_TRUE;
}

JSValue quickjsfunc_mesh_get_loopid_ev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int loopid        = 0;
  int e_or_v        = 0;
  JSValue ret       = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting three arguments (mesh pointer, loopid, 0-1 for 'e' or 'v')\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  loopid = JS_VALUE_GET_INT(argv[1]);
  e_or_v = JS_VALUE_GET_INT(argv[2]);
  if (e_or_v == 0) {
    ret = JS_MKVAL(JS_TAG_INT, mesh->mloop[loopid].e);
  } else {
    ret = JS_MKVAL(JS_TAG_INT, mesh->mloop[loopid].v);
  }
  return ret;
}


// mesh->medges v1

JSValue quickjsfunc_mesh_set_edgeid_v1(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int edgeid        = 0;
  float val         = 0.0;
  int valTag        = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting four arguments (mesh pointer, edgeid, new 'unsigned int' value)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  edgeid = JS_VALUE_GET_INT(argv[1]);
  valTag = JS_VALUE_GET_TAG(argv[2]);
  if (valTag == JS_TAG_INT) {
    val = JS_VALUE_GET_INT(argv[2]);
  } else if (valTag == JS_TAG_FLOAT64) {
    val = JS_VALUE_GET_FLOAT64(argv[2]);
  }
  mesh->medge[edgeid].v1 = val;
  return JS_TRUE;
}

JSValue quickjsfunc_mesh_get_edgeid_v1(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int edgeid        = 0;
  JSValue ret       = 0;
  // #########################
  if (argc != 2) {
    js_printf(__FUNCTION__ "> expecting three arguments (mesh pointer, edgeid)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  edgeid = JS_VALUE_GET_INT(argv[1]);
  ret = JS_MKVAL(JS_TAG_INT, mesh->medge[edgeid].v1);
  return ret;
}


// mesh->medges v2

JSValue quickjsfunc_mesh_set_edgeid_v2(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int edgeid        = 0;
  float val         = 0.0;
  int valTag        = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting four arguments (mesh pointer, edgeid, new 'unsigned int' value)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  edgeid = JS_VALUE_GET_INT(argv[1]);
  valTag = JS_VALUE_GET_TAG(argv[2]);
  if (valTag == JS_TAG_INT) {
    val = JS_VALUE_GET_INT(argv[2]);
  } else if (valTag == JS_TAG_FLOAT64) {
    val = JS_VALUE_GET_FLOAT64(argv[2]);
  }
  mesh->medge[edgeid].v2 = val;
  return JS_TRUE;
}

JSValue quickjsfunc_mesh_get_edgeid_v2(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int edgeid        = 0;
  JSValue ret       = 0;
  // #########################
  if (argc != 2) {
    js_printf(__FUNCTION__ "> expecting three arguments (mesh pointer, edgeid)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  edgeid = JS_VALUE_GET_INT(argv[1]);
  ret = JS_MKVAL(JS_TAG_INT, mesh->medge[edgeid].v2);
  return ret;
}


//typedef struct MPoly {
//  /** Offset into loop array and number of loops in the face. */
//  int loopstart;
//  /** Keep signed since we need to subtract when getting the previous loop. */
//  int totloop;
//  short mat_nr;
//  char flag, _pad;
//} MPoly;

JSValue quickjsfunc_mesh_set_polyid_loopstart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int polyid        = 0;
  float val         = 0.0;
  int valTag        = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting four arguments (mesh pointer, polyid, new 'int' value)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  polyid = JS_VALUE_GET_INT(argv[1]);
  valTag = JS_VALUE_GET_TAG(argv[2]);
  if (valTag == JS_TAG_INT) {
    val = JS_VALUE_GET_INT(argv[2]);
  } else if (valTag == JS_TAG_FLOAT64) {
    val = JS_VALUE_GET_FLOAT64(argv[2]);
  }
  mesh->mpoly[polyid].loopstart = val;
  return JS_TRUE;
}

JSValue quickjsfunc_mesh_get_polyid_loopstart(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int polyid        = 0;
  JSValue ret       = 0;
  // #########################
  if (argc != 2) {
    js_printf(__FUNCTION__ "> expecting three arguments (mesh pointer, polyid)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  polyid = JS_VALUE_GET_INT(argv[1]);
  ret = JS_MKVAL(JS_TAG_INT, mesh->mpoly[polyid].loopstart);
  return ret;
}

JSValue quickjsfunc_mesh_set_polyid_totloop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int polyid        = 0;
  float val         = 0.0;
  int valTag        = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting four arguments (mesh pointer, polyid, new 'int' value)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  polyid = JS_VALUE_GET_INT(argv[1]);
  valTag = JS_VALUE_GET_TAG(argv[2]);
  if (valTag == JS_TAG_INT) {
    val = JS_VALUE_GET_INT(argv[2]);
  } else if (valTag == JS_TAG_FLOAT64) {
    val = JS_VALUE_GET_FLOAT64(argv[2]);
  }
  mesh->mpoly[polyid].totloop = val;
  return JS_TRUE;
}

JSValue quickjsfunc_mesh_get_polyid_totloop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int polyid        = 0;
  JSValue ret       = 0;
  // #########################
  if (argc != 2) {
    js_printf(__FUNCTION__ "> expecting three arguments (mesh pointer, polyid)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  polyid = JS_VALUE_GET_INT(argv[1]);
  ret = JS_MKVAL(JS_TAG_INT, mesh->mpoly[polyid].totloop);
  return ret;
}

// mpoly->mat_nr
JSValue quickjsfunc_mesh_set_polyid_mat_nr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int polyid        = 0;
  float val         = 0.0;
  int valTag        = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting four arguments (mesh pointer, polyid, new 'int' value)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  polyid = JS_VALUE_GET_INT(argv[1]);
  valTag = JS_VALUE_GET_TAG(argv[2]);
  if (valTag == JS_TAG_INT) {
    val = JS_VALUE_GET_INT(argv[2]);
  } else if (valTag == JS_TAG_FLOAT64) {
    val = JS_VALUE_GET_FLOAT64(argv[2]);
  }
  mesh->mpoly[polyid].mat_nr = val;
  return JS_TRUE;
}

JSValue quickjsfunc_mesh_get_polyid_mat_nr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int polyid        = 0;
  JSValue ret       = 0;
  // #########################
  if (argc != 2) {
    js_printf(__FUNCTION__ "> expecting three arguments (mesh pointer, polyid)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  polyid = JS_VALUE_GET_INT(argv[1]);
  ret = JS_MKVAL(JS_TAG_INT, mesh->mpoly[polyid].mat_nr);
  return ret;
}

// mpoly[polyid]->flag

JSValue quickjsfunc_mesh_set_polyid_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int polyid        = 0;
  float val         = 0.0;
  int valTag        = 0;
  // #########################
  if (argc != 3) {
    js_printf(__FUNCTION__ "> expecting four arguments (mesh pointer, polyid, new 'int' value)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  polyid = JS_VALUE_GET_INT(argv[1]);
  valTag = JS_VALUE_GET_TAG(argv[2]);
  if (valTag == JS_TAG_INT) {
    val = JS_VALUE_GET_INT(argv[2]);
  } else if (valTag == JS_TAG_FLOAT64) {
    val = JS_VALUE_GET_FLOAT64(argv[2]);
  }
  mesh->mpoly[polyid].flag = val;
  return JS_TRUE;
}

JSValue quickjsfunc_mesh_get_polyid_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  struct Mesh *mesh = NULL;
  int polyid        = 0;
  JSValue ret       = 0;
  // #########################
  if (argc != 2) {
    js_printf(__FUNCTION__ "> expecting three arguments (mesh pointer, polyid)\n");
    return JS_FALSE;
  }
  if (JS_VALUE_GET_TAG(argv[0]) != JS_TAG_INT) {
    js_printf(__FUNCTION__ "> missing arguments[0] needs to be a pointer (JS_TAG_INT for lack of pointer tag)\n");
    return JS_FALSE;
  }
  // TODO: arg checking or implement JS_GetParams("piif", &mesh, &vertid, &xyz, &val);
  mesh   = JS_VALUE_GET_PTR(argv[0]);
  polyid = JS_VALUE_GET_INT(argv[1]);
  ret = JS_MKVAL(JS_TAG_INT, mesh->mpoly[polyid].flag);
  return ret;
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

void quickjs_funcs_mesh() {
  quickjs_add_function("mesh_add"           , quickjsfunc_mesh_add                , 0);
  quickjs_add_function("mesh_from_buffers"  , quickjsfunc_mesh_from_buffers       , 3);
  // mvert
  quickjs_add_function("mesh_set_vertid_xyz", quickjsfunc_mesh_set_vertid_xyz     , 4);
  quickjs_add_function("mesh_get_vertid_xyz", quickjsfunc_mesh_get_vertid_xyz     , 3);
  // mloop
  quickjs_add_function("mesh_set_loopid_ev" , quickjsfunc_mesh_set_loopid_ev      , 4);
  quickjs_add_function("mesh_get_loopid_ev" , quickjsfunc_mesh_get_loopid_ev      , 3);
  // medge
  quickjs_add_function("mesh_set_edgeid_v1" , quickjsfunc_mesh_set_edgeid_v1      , 3);
  quickjs_add_function("mesh_get_edgeid_v1" , quickjsfunc_mesh_get_edgeid_v1      , 2);
  quickjs_add_function("mesh_set_edgeid_v2" , quickjsfunc_mesh_set_edgeid_v2      , 3);
  quickjs_add_function("mesh_get_edgeid_v2" , quickjsfunc_mesh_get_edgeid_v2      , 2);
  // mpoly
  quickjs_add_function("mesh_set_polyid_loopstart", quickjsfunc_mesh_set_polyid_loopstart, 3);
  quickjs_add_function("mesh_get_polyid_loopstart", quickjsfunc_mesh_get_polyid_loopstart, 2);
  quickjs_add_function("mesh_set_polyid_totloop"  , quickjsfunc_mesh_set_polyid_totloop  , 3);
  quickjs_add_function("mesh_get_polyid_totloop"  , quickjsfunc_mesh_get_polyid_totloop  , 2);
  quickjs_add_function("mesh_set_polyid_mat_nr"   , quickjsfunc_mesh_set_polyid_mat_nr   , 3);
  quickjs_add_function("mesh_get_polyid_mat_nr"   , quickjsfunc_mesh_get_polyid_mat_nr   , 2);
  quickjs_add_function("mesh_set_polyid_flag"     , quickjsfunc_mesh_set_polyid_flag     , 3);
  quickjs_add_function("mesh_get_polyid_flag"     , quickjsfunc_mesh_get_polyid_flag     , 2);
  // ...
  quickjs_add_function("mesh_update"        , quickjsfunc_mesh_update             , 1);
  quickjs_add_function("mesh_totvert"       , quickjsfunc_mesh_totvert            , 1);
  quickjs_add_function("mesh_totedge"       , quickjsfunc_mesh_totedge            , 1);
  quickjs_add_function("mesh_totface"       , quickjsfunc_mesh_totface            , 1);
  quickjs_add_function("mesh_totselect"     , quickjsfunc_mesh_totselect          , 1);
  quickjs_add_function("mesh_totpoly"       , quickjsfunc_mesh_totpoly            , 1);
  quickjs_add_function("mesh_totloop"       , quickjsfunc_mesh_totloop            , 1);
}
