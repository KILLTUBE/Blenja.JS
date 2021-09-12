function selectedObjects() {
    var pointers;
    var pointer;
    var i;
    var n;
    var entity;
    // #########################
    pointers = _selectedObjects();
    n = pointers.length;
    for (i=0; i<n; i++) {
        pointer = pointers[i];
        entity = Object.create(Entity.prototype);
        entity.pointer = pointer;
        pointers[i] = entity;
    }
    return pointers;
}

// just for pure lazyniss
function selectedObject() {
    var ret;
    ret = selectedObjects();
    return ret[0];
}
