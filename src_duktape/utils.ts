import { thingsHaveChanged } from "./blenja";
import { Entity } from "./Entity";

/**
 * @example ```js
 *   sum([1, 2, 3]); // == 6
 * ```
 */
export function sum(arr: number[]) {
  return arr.reduce((acc, cur)=>acc+cur, 0);
}

/**
 * @example ```js
 *   console.log(joinBeforeAfter([1,2,3], "  ", ",\n"))
 * ```
 */
export function joinBeforeAfter(arr: [], before: string, after: string) {
  var ret = '';
  var i: number;
  var n: number;
  // #########################
  n = arr.length;
  for (i=0; i<n; i++) {
    ret += before;
    ret += arr[i];
    if (i + 1 < n) {
      ret += after;
    }
  }
  return ret;
}

export function isArray(arr: any) {
  if (arr instanceof Array) {
    return true;
  }
  if (arr instanceof Float32Array) {
    return true;
  }
  // Not using any other array's currently and there should be a better way.
  //if (arr instanceof Float64Array) {
  //  return true;
  //}
  // Check Int32Array etc.
}

/**
 * @example ```js
 *   // Prints: [5,10,12]
 *   console.log(min([10,11,12], [5,10,15]))
 * ```
 * @todo
 * Support varargs, so code like `min(...Entity.all.map(e=>e.position))` will work
 */
export function min(a: number[], b: number[]) {
  if (isArray(a) && isArray(b)) {
    var n = Math.max(a.length, b.length);
    var ret: number[] = Array(n);
    for (var i=0; i<n; i++) {
      // If one array is shorter than the other, this will compare undefined and return NaN...
      // IMO programmers fault to find minimum values in such cases
      ret[i] = Math.min(a[i], b[i]);
    }
    return ret;
  } else {
    console.warn(`Unimplemented: min(${typeof a}, ${typeof b})`);
  }
}

/**
 * @summary Returns every entity whose mesh has no faces. This happens for e.g. imported SVG lines.
 * Once all of them are selected, you can extrude them with "e" to give them a mesh for e.g. glTF exporting
 * @example ```js
 *   noFaces = selectNoFaces();
 * ```
 * @returns {Entity[]}
 */
export function selectNoFaces() {
  var ents = Entity.all.filter(e=>e.mesh && e.mesh.totpoly == 0);
  ents.forEach(ent=>ent.select());
  thingsHaveChanged();
  return ents;
}

// noFaces = deselectNoFaces();
export function deselectNoFaces() {
  var ents = Entity.all.filter(e=>e.mesh && e.mesh.totpoly == 0);
  ents.forEach(ent=>ent.deselect());
  thingsHaveChanged();
  return ents;
}

export function deselectAll() {
  Entity.all.forEach(e => e.deselect());
  thingsHaveChanged();
}

/**
 * @summary Used for Functional programming.
 * @example ```js
 * Entity.all.forEach(select);
 * thingsHaveChanged();
 * ```
 */
export function select(o) {
  o.select();
}

export function selectManyFaces() {
  var manyFaces = Entity.all.filter(function(e) {
    var mesh = e.mesh;
    if (!mesh) {
      return false;
    }
    return mesh.totpoly > 40;
  });
  manyFaces.forEach(select);
  thingsHaveChanged();
  return manyFaces;
}

export function selectManyEdges(n = 40) {
  var manyFaces = Entity.all.filter(function(e) {
    var mesh = e.mesh;
    if (!mesh) {
      return false;
    }
    return mesh.totedge > n;
  });
  manyFaces.forEach(select);
  thingsHaveChanged();
  return manyFaces;
}
