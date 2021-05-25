/**
 * @example
 * ```
 *   entity = new Entity();
 *   entity.position[0] += 1;
 *   entity.update();
 *   entity.children;
 *   entity.destroy();
 * ```
 */

class Entity {
  pointer = 0;

  constructor(name = "Untitled") {
    this.pointer = new_object_with_mesh(name);
    this.position = new Float32Array(object_position(this.pointer));
    thingsHaveChanged();
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
    var pointer;
    var pointers;
    var i;
    pointers = object_children(this.pointer);
    for (i=0; i<pointers.length; i++) {
      pointer = pointers[i];
      pointers[i] = Object.create(Entity.prototype);
      pointers[i].pointer = pointer;
    }
    return pointers;
  }

  get name() {
    var tmp;
    // will be something like "OBxxx"
    tmp = object_name_get(this.pointer);
    tmp = tmp.slice(2); // remove "OB"

    return tmp;
  }

  toString() {
    if (this.pointer == 0) {
      return 'Entity(destroyed)';
    }
    return `Entity(name: ${this.name}, children.length: ${this.children.length})`;
  }
}
