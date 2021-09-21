import { SELECT, _mesh_get_vertid_bweight, _mesh_get_vertid_flag, _mesh_get_vertid_xyz, _mesh_set_vertid_xyz } from "./blenja";
import { Mesh } from "./Mesh";

export class MeshVertex {
  constructor(readonly mesh: Mesh, readonly i: number) {
    // nothing yet
  }

  get       x(     ) { return _mesh_get_vertid_xyz    (this.mesh.pointer, this.i, 0       ); }
  get       y(     ) { return _mesh_get_vertid_xyz    (this.mesh.pointer, this.i, 1       ); }
  get       z(     ) { return _mesh_get_vertid_xyz    (this.mesh.pointer, this.i, 2       ); }
  set       x(value) {        _mesh_set_vertid_xyz    (this.mesh.pointer, this.i, 0, value); }
  set       y(value) {        _mesh_set_vertid_xyz    (this.mesh.pointer, this.i, 1, value); }
  set       z(value) {        _mesh_set_vertid_xyz    (this.mesh.pointer, this.i, 2, value); }
  get    flag(     ) { return _mesh_get_vertid_flag   (this.mesh.pointer, this.i          ); }
  get bweight(     ) { return _mesh_get_vertid_bweight(this.mesh.pointer, this.i          ); }

  get selected() {
    return this.flag & /*SELECT*/ 1; // TODO
  }

  toString() {
    return `vertex[${this.i}] = ${this.x.toFixed(3)} ${this.y.toFixed(3)} ${this.z.toFixed(3)}`;
  }
}
