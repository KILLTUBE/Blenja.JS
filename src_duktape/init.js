function main() {
    require("Console.js");
    require("Mesh.js");
}

try {
    main();
} catch (e) {
    // Print error to console, when QuickJS initialisation scripts fail
    log("Exception:");
    log(e);
}
