/**
 * @example
 * ```
 *   object = new Object();
 *   object.position[0] += 1;
 *   object.update();
 *   object.children;
 *   object.destroy();
 * ```
 */

class Object {
  pointer = 0;

  constructor() {
    this.pointer = new_object_with_mesh();
    this.position = new Float32Array(object_position(this.pointer));
  }

  destroy() {
    if (this.pointer == 0) {
      return false;
    }
    object_destroy(this.pointer);
    this.pointer = 0;
    thingsHaveChanged();
    return true;
  }

  update() {
    object_update(this.pointer);
    thingsHaveChanged();
  }

  get children() {
    return object_children(this.pointer);
  }
}
