import { mesh_get_vertid_xyz, mesh_set_vertid_xyz } from "./blenja";
import { Mesh } from "./Mesh";

export class MeshVertex {
  constructor(readonly mesh: Mesh, readonly i: number) {
    // nothing yet
  }

  get x(     ) { return mesh_get_vertid_xyz(this.mesh.pointer, this.i, 0       ); }
  get y(     ) { return mesh_get_vertid_xyz(this.mesh.pointer, this.i, 1       ); }
  get z(     ) { return mesh_get_vertid_xyz(this.mesh.pointer, this.i, 2       ); }
  set x(value) {        mesh_set_vertid_xyz(this.mesh.pointer, this.i, 0, value); }
  set y(value) {        mesh_set_vertid_xyz(this.mesh.pointer, this.i, 1, value); }
  set z(value) {        mesh_set_vertid_xyz(this.mesh.pointer, this.i, 2, value); }

  toString() {
    return `vertex[${this.i}] = ${this.x.toFixed(3)} ${this.y.toFixed(3)} ${this.z.toFixed(3)}`;
  }
}
