class MeshVertex {
  constructor(mesh, i) {
    this.mesh = mesh;
    this.i = i;
  }
  get x() {
    return mesh_get_vertid_xyz_val(this.mesh.pointer, this.i, 0);
  }
  get y() {
    return mesh_get_vertid_xyz_val(this.mesh.pointer, this.i, 1);
  }
  get z() {
    return mesh_get_vertid_xyz_val(this.mesh.pointer, this.i, 2);
  }
  set x(value) {
    return mesh_set_vertid_xyz_val(this.mesh.pointer, this.i, 0, value);
  }
  set y(value) {
    return mesh_set_vertid_xyz_val(this.mesh.pointer, this.i, 1, value);
  }
  set z(value) {
    return mesh_set_vertid_xyz_val(this.mesh.pointer, this.i, 2, value);
  }
}
