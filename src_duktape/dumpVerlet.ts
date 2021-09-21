import { Mesh } from "./Mesh";
import { selectedObject } from "./selectedObjects";

/**
 * @example ```js
 *   console.clear();
 *   reload();
 *   var mesh = selectedObject().mesh;
 *   // select in edit mode, go to object mode, otherwise vertices are not selected for some reason
 *   pins = mesh.vertices.filter(v=>v.selected).map(v=>v.i);
 *   out = dumpVerlet(mesh, pins);
 *   console.log(out);
 * ```
 * @returns 
 */

export function dumpVerlet(mesh: Mesh, pins: number[]) {
  var out = '{\n';
  out += '  "kind": "verlet",\n';
  out += `  "name": "${mesh.name}",\n`;
  out += '  "vertices": [\n';
  out += mesh.vertices.map(vertex=>`    [${vertex.x}, ${vertex.y}, ${vertex.z}]`).join(',\n') + '\n';
  out += '  ],\n';
  out += '  "edges": [\n';
  out += mesh.edges.map(edge=>`    [${edge.v1}, ${edge.v2}]`).join(',\n') + '\n';
  out += '  ],\n';
  out += `  "pins": [${pins.join(', ')}]\n`;
  out += '}\n';
  return out;
}
