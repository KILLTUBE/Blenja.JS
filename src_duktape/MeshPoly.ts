/**
 * @example
 * ```js
 *   reload();
 *   console.clear();
 *   a = new Entity('a');
 *   a.mesh.edges.forEach((i)=>console.log(i));
 * ```
 */

import {
  mesh_get_polyid_flag, mesh_get_polyid_loopstart, mesh_get_polyid_mat_nr, mesh_get_polyid_totloop,
  mesh_set_polyid_flag, mesh_set_polyid_loopstart, mesh_set_polyid_mat_nr, mesh_set_polyid_totloop
} from "./blenja";
import { Mesh } from "./Mesh";

export class MeshPoly {
  mesh: Mesh;
  i: number;

  constructor(mesh: Mesh, i: number) {
    this.mesh = mesh;
    this.i = i;
  }

  get loopstart(     ) { return mesh_get_polyid_loopstart(this.mesh.pointer, this.i       ); }
  get totloop  (     ) { return mesh_get_polyid_totloop  (this.mesh.pointer, this.i       ); }
  get mat_nr   (     ) { return mesh_get_polyid_mat_nr   (this.mesh.pointer, this.i       ); }
  get flag     (     ) { return mesh_get_polyid_flag     (this.mesh.pointer, this.i       ); }
  set loopstart(value) {        mesh_set_polyid_loopstart(this.mesh.pointer, this.i, value); }
  set totloop  (value) {        mesh_set_polyid_totloop  (this.mesh.pointer, this.i, value); }
  set mat_nr   (value) {        mesh_set_polyid_mat_nr   (this.mesh.pointer, this.i, value); }
  set flag     (value) {        mesh_set_polyid_flag     (this.mesh.pointer, this.i, value); }

  toString() {
    var tmp: string;
    // #########################
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
