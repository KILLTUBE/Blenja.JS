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

export class Entity {
  position: Float32Array;
  pointer = 0;

  constructor(name = "Untitled") {
    this.pointer = new_object_with_mesh(name);
    this.init();
    thingsHaveChanged();
  }

  init() {
    this.position = new Float32Array(object_position(this.pointer));
    // Set reference in `struct Object *` itself
    object_reference_set(this.pointer, this);
  }

  static fromPointer(pointer: number) {
    var entity: Entity;
    // #########################
    entity = Object.create(Entity.prototype);
    entity.pointer = pointer;
    entity.init();
    return entity;
  }

  static get all(): Entity[] {
    return _allEntities().map(Entity.fromPointer);
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

  // This is called for e.g. setting x/y/z, if this becomes a performance concern,
  // simply mark the entity as dirty and call these methods only once at end.
  update() {
    if (this.pointer == 0) {
      return false;
    }
    object_update(this.pointer);
    thingsHaveChanged();
    return true;
  }

  get children() {
    var pointers;
    // #########################
    if (this.pointer == 0) {
      return undefined;
    }
    pointers = object_children(this.pointer);
    return pointers.map(Entity.fromPointer);
  }

  get name() {
    var tmp: string;
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
    var mesh: Mesh;
    var meshPointer: number;
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
    var name: string;
    // #########################
    if (this.pointer == 0) {
      return 'Entity(destroyed)';
    }
    name = JSON.stringify(this.name);
    return `Entity(name: ${name}, children.length: ${this.children.length})`;
  }

  /**
   * @example ```js
   * Entity.all.forEach(e=>e.select())
   * ```
   */
  select() {
    _entity_select(this.pointer);
  }

  /**
   * @example ```js
   * Entity.all.forEach(e=>e.deselect())
   * ```
   */
  deselect() {
    _entity_deselect(this.pointer);
  }

  /**
   * Looks like this just selects the object so far... would be nice to have focus()
   * @returns {boolean}
   */
  jumpTo() {
    return _entity_jump_to(this.pointer);
  }

  get x() { return this.position[0]; }
  get y() { return this.position[1]; }
  get z() { return this.position[2]; }
  set x(value) {
    this.position[0] = value;
    this.update();
  }
  set y(value) {
    this.position[1] = value;
    this.update();
  }
  set z(value) {
    this.position[2] = value;
    this.update();
  }
}
