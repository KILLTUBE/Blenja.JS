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
    // Set reference in `struct Object *` itself
    object_reference_set(this.pointer, this);
    thingsHaveChanged();
  }

  /**
   * @param {number} pointer 
   * @returns {Entity}
   */
  static fromPointer(pointer) {
    var entity = Object.create(Entity.prototype);
    entity.pointer = pointer;
    return entity;
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

  // This method is called from Blender when the Entity is about to destroyed
  _destroy() {
    this.pointer = 0;
  }

  update() {
    if (this.pointer == 0) {
      return false;
    }
    object_update(this.pointer);
    thingsHaveChanged();
    return true;
  }

  get children() {
    var pointer;
    var pointers;
    var i;
    // #########################
    if (this.pointer == 0) {
      return undefined;
    }
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
    // #########################
    if (this.pointer == 0) {
      return undefined;
    }
    // will be something like 'OBxxx'
    tmp = object_name_get(this.pointer);
    tmp = tmp.slice(2); // remove 'OB'
    return tmp;
  }

  get mesh() {
    var mesh;
    var meshPointer;
    // #########################
    if (this.pointer == 0) {
      return undefined;
    }
    meshPointer = object_mesh_get(this.pointer);
    if (meshPointer === false) {
      return undefined;
    }
    mesh = Mesh.fromPointer(meshPointer);
    return mesh;
  }

  toString() {
    var name;
    if (this.pointer == 0) {
      return 'Entity(destroyed)';
    }
    name = JSON.stringify(this.name);
    return `Entity(name: ${name}, children.length: ${this.children.length})`;
  }
}
