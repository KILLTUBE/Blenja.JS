/**
 * @example
 * ```
 *   mesh = new Mesh();
 *   mesh.vertices[0].x = Math.random() * 2
 *   mesh.vertices[0].y = Math.random() * 2
 *   mesh.vertices[0].z = Math.random() * 2
 *   mesh.vertices[1].x = Math.random() * 4
 *   mesh.vertices[1].y = Math.random() * 4
 *   mesh.vertices[1].z = Math.random() * 4
 *   mesh.update()
 * ```
 */

class Mesh {
  pointer = 0;
  vertices = [];

  constructor() {
    this.pointer = addmesh();
    this.initVertices();
  }

  static fromPointer(meshPointer) {
    var mesh;
    mesh = Object.create(Mesh.prototype);
    mesh.pointer = meshPointer;
    mesh.vertices = [];
    mesh.initVertices();
    return mesh;
  }

  initVertices() {
    var i;
    var n;
    // #########################
    n = this.totvert;
    this.vertices.length = n;
    for (i=0; i<n; i++) {
      this.vertices[i] = new MeshVertex(this, i);
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

  toString() {
    var ret;
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
}
