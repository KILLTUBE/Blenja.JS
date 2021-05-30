/**
 * @example
 * ```
 *   reload();
 *   console.clear();
 *   a = new Entity('a');
 *   a.mesh.edges.forEach((i)=>console.log(i));
 * ```
 */

class MeshPoly {
  constructor(mesh, i) {
    this.mesh = mesh;
    this.i = i;
  }

  get loopstart(     ) { return mesh_get_polyid_loopstart(this.mesh.pointer, this.i       ); }
  get totloop  (     ) { return mesh_get_polyid_totloop  (this.mesh.pointer, this.i       ); }
  get mat_nr   (     ) { return mesh_get_polyid_mat_nr   (this.mesh.pointer, this.i       ); }
  get flag     (     ) { return mesh_get_polyid_flag     (this.mesh.pointer, this.i       ); }
  set loopstart(value) { return mesh_set_polyid_loopstart(this.mesh.pointer, this.i, value); }
  set totloop  (value) { return mesh_set_polyid_totloop  (this.mesh.pointer, this.i, value); }
  set mat_nr   (value) { return mesh_set_polyid_mat_nr   (this.mesh.pointer, this.i, value); }
  set flag     (value) { return mesh_set_polyid_flag     (this.mesh.pointer, this.i, value); }

  toString() {
    var tmp;
    tmp = 'MeshPoly { ';
    tmp += `meshPointer: ${this.mesh.pointer}, `;
    tmp += `i: ${this.i}, `;
    tmp += `loopstart: ${this.loopstart}, `;
    tmp += `totloop: ${this.totloop}, `;
    tmp += `mat_nr: ${this.mat_nr}, `;
    tmp += `flag: ${this.flag}`;
    tmp += ' }';
    return tmp;
  }
}
