/**
 * @example
 * ```
 *   reload();
 *   console.clear();
 *   a = new Entity('a');
 *   a.mesh.edges.forEach((i)=>console.log(i));
 * ```
 */

class MeshEdge {
  constructor(mesh, i) {
    this.mesh = mesh;
    this.i = i;
  }

  get v1(     ) { return mesh_get_edgeid_v1(this.mesh.pointer, this.i       ); }
  get v2(     ) { return mesh_get_edgeid_v2(this.mesh.pointer, this.i       ); }
  set v1(value) { return mesh_set_edgeid_v1(this.mesh.pointer, this.i, value); }
  set v2(value) { return mesh_set_edgeid_v2(this.mesh.pointer, this.i, value); }

  toString() {
    var tmp;
    tmp = 'MeshEdge { ';
    tmp += `meshPointer: ${this.mesh.pointer}, `;
    tmp += `i: ${this.i}, `;
    tmp += `v1: ${this.v1}, `;
    tmp += `v2: ${this.v2}`;
    tmp += ' }';
    return tmp;
  }
}
