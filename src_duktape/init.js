window = this; // TODO: window = getGlobal();

function requireTS(filename) {
  var final = dir + '/' + filename;
  var contents = file_get_contents(final);
  var ret = TypeSpirit.rewrite(contents,{keepImport:false});
  var out = ret.out;
  //console.log(Object.keys(ret.names))
  console.log(Object.keys(ret.names)) // .join(',')
  var names = Object.keys(ret.names);
  out += '\nreturn {\n';
  for (var name of names) {
      out += '  ' + name + ',\n';
  }
  out += '};';
  //console.log(out);
  var f = new Function(out);
  Object.assign(window, f());
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
  requireTS('Entity.ts');
  // skip example_ganja.js
  require('ganja.js');
  requireTS('GHash.ts');
  requireTS('Icosahedron.ts');
  // requireTS('jsx.ts');
  // skip init.js
  requireTS('Mesh.ts');
  requireTS('MeshEdge.ts');
  requireTS('MeshLoop.ts');
  requireTS('MeshPoly.ts');
  requireTS('MeshVertex.ts');
  requireTS('Performance.ts');
  // requireTS('Person.ts');
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
