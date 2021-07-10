/**
 * @example ```js
 * console.clear()
 * reload()
 * Collection.main.object.length
 * Collection.main.children[1].children[0].name
 * ```
 */

class Collection {
  constructor() {
    console.log("Collection#constructor: nothing");
  }

  static fromPointer(pointer) {
    var collection;
    // #########################
    collection = Object.create(Collection.prototype);
    collection.pointer = pointer;
    return collection;
  }

  static get selected() {
    var pointer;
    var collection;
    // #########################
    pointer = collection_selected(); // seems to be always the selected one... want the root collection tho
    collection = Collection.fromPointer(pointer);
    return collection;
  }

  static get main() {
    var pointer;
    var collection;
    // #########################
    pointer = collection_main(); // seems to be always the selected one... want the root collection tho
    collection = Collection.fromPointer(pointer);
    return collection;
  }

  get children() {
    var pointers;
    // #########################
    pointers = collection_get_children(this.pointer);
    return pointers.map(Collection.fromPointer);
  }

  get objects() {
    var pointers;
    // #########################
    pointers = collection_get_objects(this.pointer);
    return pointers.map(Entity.fromPointer);
  }

  get name() {
    var tmp = collection_get_name(this.pointer); // E.g. 'GRMaster collection'
    tmp = tmp.substr(2); // 'Master collection'
    return tmp;
  }
}
