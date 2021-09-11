import { mesh_get_vertid_xyz, mesh_set_vertid_xyz } from "./blenja";
import { Mesh } from "./Mesh";

export class MeshVertex {
  mesh: Mesh;
  i: number;

  constructor(mesh: Mesh, i: number) {
    this.mesh = mesh;
    this.i = i;
  }

  get x(     ) { return mesh_get_vertid_xyz(this.mesh.pointer, this.i, 0       ); }
  get y(     ) { return mesh_get_vertid_xyz(this.mesh.pointer, this.i, 1       ); }
  get z(     ) { return mesh_get_vertid_xyz(this.mesh.pointer, this.i, 2       ); }
  set x(value) {        mesh_set_vertid_xyz(this.mesh.pointer, this.i, 0, value); }
  set y(value) {        mesh_set_vertid_xyz(this.mesh.pointer, this.i, 1, value); }
  set z(value) {        mesh_set_vertid_xyz(this.mesh.pointer, this.i, 2, value); }
}
