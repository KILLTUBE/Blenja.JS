/**
 * @example
 * ```
 *   reload();
 *   console.clear();
 *   a = new Entity('a');
 *   a.mesh.loops.forEach((i)=>console.log(i));
 * ```
 */

import { mesh_get_loopid_ev, mesh_set_loopid_ev } from "./blenja";
import { Mesh } from "./Mesh";

export class MeshLoop {
  constructor(readonly mesh: Mesh, readonly i: number) {
    // nothing yet
  }

  get e(     ) { return mesh_get_loopid_ev(this.mesh.pointer, this.i, 0       ); }
  get v(     ) { return mesh_get_loopid_ev(this.mesh.pointer, this.i, 1       ); }
  set e(value) {        mesh_set_loopid_ev(this.mesh.pointer, this.i, 0, value); }
  set v(value) {        mesh_set_loopid_ev(this.mesh.pointer, this.i, 1, value); }

  toString() {
    var tmp: string;
    // #########################
    tmp = 'MeshLoop { ';
    tmp += `meshPointer: ${this.mesh.pointer}, `;
    tmp += `i: ${this.i}, `;
    tmp += `e: ${this.e}, `;
    tmp += `v: ${this.v}`;
    tmp += ' }';
    return tmp;
  }
}