function main() {
    require('Console.js');
    require('MeshVertex.js');
    require('MeshLoop.js');
    require('MeshEdge.js');
    require('MeshPoly.js');
    require('Mesh.js');
    require('Entity.js');
    require('selectedObjects.js');
}

try {
    main();
} catch (e) {
    // Print error to console, when QuickJS initialisation scripts fail
    log('Exception:');
    log(e);
}
