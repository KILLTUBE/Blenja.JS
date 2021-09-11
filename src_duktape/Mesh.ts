/**
 * @example ```
 *   mesh = new Mesh();
 *   mesh.vertices[0].x = Math.random() * 2;
 *   mesh.vertices[0].y = Math.random() * 2;
 *   mesh.vertices[0].z = Math.random() * 2;
 *   mesh.vertices[1].x = Math.random() * 4;
 *   mesh.vertices[1].y = Math.random() * 4;
 *   mesh.vertices[1].z = Math.random() * 4;
 *   mesh.update()
 * ```
 * 
 * @example ```
 *   mesh = new Mesh();
 *   mesh.describe();
 * ```
 * 
 * @example ```
 *   console.clear();
 *   selectedObject().mesh.describe();
 * ```
 * 
 */

export class Mesh {
  pointer  = 0;
  vertices = [];
  loops    = [];
  edges    = [];
  polygons = [];

  constructor() {
    this.pointer = mesh_add();
    this.initVertices();
    this.initLoops();
    this.initEdges();
    this.initPolygons();
  }

  static fromPointer(meshPointer: number) {
    var mesh: Mesh;
    // #########################
    mesh = Object.create(Mesh.prototype);
    mesh.pointer  = meshPointer;
    mesh.vertices = [];
    mesh.loops    = [];
    mesh.edges    = [];
    mesh.polygons = [];
    mesh.initVertices();
    mesh.initLoops();
    mesh.initEdges();
    mesh.initPolygons();
    return mesh;
  }

  static fromBuffers(vertices, loops, polygons) {
    var meshPointer: number;
    meshPointer = mesh_from_buffers(vertices, loops, polygons);
    return Mesh.fromPointer(meshPointer);
  }

  static printRNA() {
    mesh_rna_print();
  }

  initVertices() {
    var i: number;
    var n: number;
    // #########################
    n = this.totvert;
    this.vertices.length = n;
    for (i=0; i<n; i++) {
      this.vertices[i] = new MeshVertex(this, i);
    }
  }

  initLoops() {
    var i: number;
    var n: number;
    // #########################
    n = this.totloop;
    this.loops.length = n;
    for (i=0; i<n; i++) {
      this.loops[i] = new MeshLoop(this, i);
    }
  }

  initEdges() {
    var i: number;
    var n: number;
    // #########################
    n = this.totedge;
    this.edges.length = n;
    for (i=0; i<n; i++) {
      this.edges[i] = new MeshEdge(this, i);
    }
  }

  initPolygons() {
    var i: number;
    var n: number;
    // #########################
    n = this.totpoly;
    this.polygons.length = n;
    for (i=0; i<n; i++) {
      this.polygons[i] = new MeshPoly(this, i);
    }
  }

  set_vertid_xyz_val(vertid, xyz, val) {
    mesh_set_vertid_xyz_val(this.pointer, vertid, xyz, val);
  }

  update() {
    mesh_update(this.pointer);
  }

  get totvert() {
    return mesh_totvert(this.pointer);
  }
  get totedge() {
    return mesh_totedge(this.pointer);
  }
  get totface() {
    return mesh_totface(this.pointer);
  }
  get totselect() {
    return mesh_totselect(this.pointer);
  }
  get totpoly() {
    return mesh_totpoly(this.pointer);
  }
  get totloop() {
    return mesh_totloop(this.pointer);
  }

  describe() {
    var i: number;
    var n: number;
    var vertices;
    var vertex;
    var loops;
    var loop;
    var edges;
    var edge;
    var polygons;
    var polygon;
    // #########################
    console.log(`Vertices (${this.totvert}):`);
    vertices = this.vertices;
    n = vertices.length;
    for (i=0; i<n; i++) {
      vertex = vertices[i];
      console.log(`i=${i} [${vertex.x}, ${vertex.x}, ${vertex.z}]`);
    }
    console.log(`Loops (${this.totloop}):`);
    loops = this.loops;
    n = loops.length;
    for (i=0; i<n; i++) {
      loop = loops[i];
      console.log(`i=${i} {v: ${loop.v}, e: ${loop.e}}`);
    }
    console.log(`Edges (${this.totedge}):`);
    edges = this.edges;
    n = edges.length;
    for (i=0; i<n; i++) {
      edge = edges[i];
      console.log(`i=${i} {v1: ${edge.v1}, v2: ${edge.v2}}`);
    }
    console.log(`Polygons (${this.totpoly}):`);
    polygons = this.polygons;
    n = polygons.length;
    for (i=0; i<n; i++) {
      polygon = polygons[i];
      console.log(`i=${i} {loopstart: ${polygon.loopstart}, totloop: ${polygon.totloop}, mat_nr: ${polygon.mat_nr}, flag: ${polygon.flag}}`);
    }
  }

  toString() {
    var ret: string;
    // #########################
    ret = 'Mesh { ';
    ret += `totvert: ${this.totvert}, `;
    ret += `totedge: ${this.totedge}, `;
    ret += `totface: ${this.totface}, `;
    ret += `totselect: ${this.totselect}, `;
    ret += `totpoly: ${this.totpoly}, `;
    ret += `totloop: ${this.totloop} `;
    ret += '}';
    return ret;
  }

  rna_vertex_color_new(name: string) {
    mesh_rna_vertex_color_new(this.pointer, name);
  }
}
