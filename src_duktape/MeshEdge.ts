/**
 * @example
 * ```
 *   reload();
 *   console.clear();
 *   a = new Entity('a');
 *   a.mesh.edges.forEach((i)=>console.log(i));
 *   a.mesh.edges.forEach(edge => console.log(edge.toString())); // TODO: do toString in console.log
 * ```
 */

import {
  _mesh_get_edgeid_v1,
  _mesh_get_edgeid_v2,
  _mesh_set_edgeid_v1,
  _mesh_set_edgeid_v2
} from "./blenja";
import { Mesh } from "./Mesh";

export class MeshEdge {
  constructor(readonly mesh: Mesh, readonly i: number) {
    // nothing yet
  }

  get v1(     ) { return _mesh_get_edgeid_v1(this.mesh.pointer, this.i       ); }
  get v2(     ) { return _mesh_get_edgeid_v2(this.mesh.pointer, this.i       ); }
  set v1(value) {        _mesh_set_edgeid_v1(this.mesh.pointer, this.i, value); }
  set v2(value) {        _mesh_set_edgeid_v2(this.mesh.pointer, this.i, value); }

  toString() {
    var tmp: string;
    // #########################
    tmp = 'MeshEdge { ';
    tmp += `meshPointer: ${this.mesh.pointer}, `;
    tmp += `i: ${this.i}, `;
    tmp += `v1: ${this.v1}, `;
    tmp += `v2: ${this.v2}`;
    tmp += ' }';
    return tmp;
  }
}
