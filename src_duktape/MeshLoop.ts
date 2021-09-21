/**
 * @example
 * ```
 *   reload();
 *   console.clear();
 *   a = new Entity('a');
 *   a.mesh.loops.forEach((i)=>console.log(i));
 * ```
 */

import {
  _mesh_get_loopid_ev,
  _mesh_set_loopid_ev
} from "./blenja";
import { Mesh } from "./Mesh";

export class MeshLoop {
  constructor(readonly mesh: Mesh, readonly i: number) {
    // nothing yet
  }

  get e(     ) { return _mesh_get_loopid_ev(this.mesh.pointer, this.i, 0       ); }
  get v(     ) { return _mesh_get_loopid_ev(this.mesh.pointer, this.i, 1       ); }
  set e(value) {        _mesh_set_loopid_ev(this.mesh.pointer, this.i, 0, value); }
  set v(value) {        _mesh_set_loopid_ev(this.mesh.pointer, this.i, 1, value); }

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
