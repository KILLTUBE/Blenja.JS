function requireTS(filename) {
  var final = dir + '/' + filename;
  var contents = file_get_contents(final);
  var out = TypeSpirit.rewrite(contents, {
    keepImport: false,
  }).out;
  eval.bind(window)(out);
}

function main() {
  // standalone
  require('TypeSpirit.js');

  require('Blender.js');
  require('Body.js');
  require('Collection.js');
  require('Console.js');
  // skip copyfiles.js
  // skip duktape_quickjs.js
  require('Entity.js');
  // skip example_ganja.js
  require('ganja.js');
  require('GHash.js');
  require('Icosahedron.js');
  // skip init.js
  require('Mesh.js');
  require('MeshEdge.js');
  require('MeshLoop.js');
  require('MeshPoly.js');
  require('MeshVertex.js');
  require('Performance.js');
  // skip pga3d_icosahedron.js
  // skip pga3d_objects.js
  require('selectedObjects.js');
  require('StructRNA.js');
  
  requireTS('utils.ts');
}

try {
  main();
} catch (e) {
  // Print error to console, when QuickJS initialisation scripts fail
  println('Exception:');
  println(e);
}
