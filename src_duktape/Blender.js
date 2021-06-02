function nice(num) {
  return Number(num.toFixed(2))
}

function faces_to_mesh(faces) {
  var face;
  var i;
  var n;
  var vertex_a;
  var vertex_b;
  var vertex_c;
  var blender_vertices = [];
  var blender_loops    = [];
  var blender_polygons = [];
  var vertices_n;
  // #########################
  n = faces.length;
  vertices_n = 0;
  for (i=0; i<n; i++) {
    face = faces[i];
    vertex_a = face[0];
    vertex_b = face[1];
    vertex_c = face[2];
    //console.log(vertex_a)
    //console.log(vertex_b)
    //console.log(vertex_c)
    blender_polygons.push(vertices_n); // loopstart
    blender_polygons.push(3); // totloop
    // a
    blender_vertices.push(-vertex_a[11]);
    blender_vertices.push( vertex_a[12]);
    blender_vertices.push( vertex_a[13]);
    // b
    blender_vertices.push(-vertex_b[11]);
    blender_vertices.push( vertex_b[12]);
    blender_vertices.push( vertex_b[13]);
    // c
    blender_vertices.push(-vertex_c[11]);
    blender_vertices.push( vertex_c[12]);
    blender_vertices.push( vertex_c[13]);
    blender_loops.push(vertices_n++);
    blender_loops.push(vertices_n++);
    blender_loops.push(vertices_n++);
  }
  if (blender_vertices.length == 0) {
    console.log("Nothing to graph/render");
    return undefined;
  }
  mesh = Mesh.fromBuffers(
    new Float32Array(blender_vertices),
    new Int32Array(blender_loops),
    new Int32Array(blender_polygons)
  );
  return mesh;
}

class Blender {
  static graph(items) {
    var item;
    var type;
    var blender_vertices = [];
    var blender_loops    = [];
    var blender_polygons = [];
    var vertices_n;
    var vertex_a;
    var vertex_b;
    var vertex_c;
    var i;
    var mesh;
    // #########################
    console.log("Blender#graph", items);
    // Could be a function
    console.log(`typeof items=${typeof items}`);
    console.log(`items.length=${items.length}`);
    while (typeof items == "function") {
        items = items();
    }
    // Should be a mixed type array of colors/names/multivectors/lines/faces
    console.log(`typeof items=${typeof items}`);
    console.log(`items.length=${items.length}`);

    vertices_n = 0;
    for (i=0; i<items.length; i++) {
      item = items[i]
      type = typeof(item);

      if (type == "string") {
        // TODO: use as object/mesh name?
        //console.log(`'${item}', `);
      } else if (type == 'object') {
        // Vertex Array in Ganja.js, just treat it as normal mesh here
        if (item.data) {
          faces_to_mesh(item.data);
          continue;
        }
        if (item.length == 2) {
          // Add as unconnected edge?
          //console.log('item.length == 2');
        } else if (item.length == 3) {
          vertex_a = item[0];
          vertex_b = item[1];
          vertex_c = item[2];
          //console.log(vertex_a)
          //console.log(vertex_b)
          //console.log(vertex_c)
          blender_polygons.push(vertices_n); // loopstart
          blender_polygons.push(3); // totloop
          // a
          blender_vertices.push(nice(-vertex_a[11]));
          blender_vertices.push(nice( vertex_a[12]));
          blender_vertices.push(nice( vertex_a[13]));
          // b
          blender_vertices.push(nice(-vertex_b[11]));
          blender_vertices.push(nice( vertex_b[12]));
          blender_vertices.push(nice( vertex_b[13]));
          // c
          blender_vertices.push(nice(-vertex_c[11]));
          blender_vertices.push(nice( vertex_c[12]));
          blender_vertices.push(nice( vertex_c[13]));
          blender_loops.push(vertices_n++);
          blender_loops.push(vertices_n++);
          blender_loops.push(vertices_n++);
        } else if (item.length == 16) {
          // Single Multivector vertex/plane/line
          //console.log(`${}, ${item[12]}, ${item[13]}`);
          
          
        }
      }
    }
    //console.log("blender_vertices", blender_vertices)
    //console.log("blender_loops", blender_loops)
    //console.log("blender_polygons", blender_polygons)
    if (blender_vertices.length == 0) {
      console.log("Nothing to graph/render");
      return undefined;
    }
    mesh = Mesh.fromBuffers(
      new Float32Array(blender_vertices),
      new Int32Array(blender_loops),
      new Int32Array(blender_polygons)
    );
    return mesh;

    // TODO: Blender.graphString(items)
    // var tmp = '';
    // tmp += 'var blender_vertices = new Float32Array(';
    // tmp += blender_vertices.join(', ');
    // tmp += ');';
    // tmp += '\n';
    // tmp += 'var blender_loops = new Int32Array(';
    // tmp += blender_loops.join(', ');
    // tmp += ');';
    // tmp += '\n';
    // tmp += 'var blender_polygons = new Int32Array(';
    // tmp += blender_polygons.join(', ');
    // tmp += ');';
    // tmp += '\n';
    // console.log("tmp", tmp);
  }
}

//blender = new Blender;
