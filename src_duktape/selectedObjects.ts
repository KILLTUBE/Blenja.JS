import { _selectedObjects } from "./blenja";
import { Entity } from "./Entity";

export function selectedObjects() {
  return _selectedObjects().map(Entity.fromPointer);
}

// just for pure lazyniss
export function selectedObject() {
  var ret;
  ret = selectedObjects();
  return ret[0];
}
