function main() {
    require("Console.js");
    require("MeshVertex.js");
    require("Mesh.js");
    require("Entity.js");
}

try {
    main();
} catch (e) {
    // Print error to console, when QuickJS initialisation scripts fail
    log("Exception:");
    log(e);
}
