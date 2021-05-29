/**
 * @example
 * ```
 *   reload();
 *   console.clear();
 *   a = new Entity('a');
 *   a.mesh.loops.forEach((i)=>console.log(i));
 * ```
 */

class MeshLoop {
  constructor(mesh, i) {
    this.mesh = mesh;
    this.i = i;
  }

  get e() {
    return mesh_get_loopid_ev(this.mesh.pointer, this.i, 0);
  }

  get v() {
    return mesh_get_loopid_ev(this.mesh.pointer, this.i, 1);
  }

  set e(value) {
    return mesh_set_loopid_ev(this.mesh.pointer, this.i, 0, value);
  }

  set v(value) {
    return mesh_set_loopid_ev(this.mesh.pointer, this.i, 1, value);
  }

  toString() {
    var tmp;
    tmp = 'MeshLoop { ';
    tmp += `meshPointer: ${this.mesh.pointer}, `;
    tmp += `i: ${this.i}, `;
    tmp += `e: ${this.e}, `;
    tmp += `v: ${this.v}`;
    tmp += ' }';
    return tmp;
  }
}
