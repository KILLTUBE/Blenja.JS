/**
 * @example
 * ```
 *   mesh = new Mesh();
 *   mesh.set_vertid_xyz_val(0, 0, 0.24);
 *   mesh.update()
 * ```
 */

class Mesh {
  pointer = 0;

  constructor() {
    this.pointer = addmesh();
  }

  set_vertid_xyz_val(vertid, xyz, val) {
    mesh_set_vertid_xyz_val(this.pointer, vertid, xyz, val);
  }

  update() {
    mesh_update(this.pointer);
  }
}
