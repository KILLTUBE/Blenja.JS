class GHash {
    pointer = 0;
    constructor() {
        console.log('GHash#constructor> not implemented');
    }
    static fromPointer(pointer) {
        var ret;
        ret = Object.create(GHash.prototype);
        ret.pointer = pointer;
        return ret;
    }
    
}