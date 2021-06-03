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

function createVA(vtx, texc, idx, clr) {
  var debug;
  var i;
  var n;
  var polygons;
  // #########################
  debug = false;
  n = idx.length / 3; // three indices make a triangle
  polygons = new Int32Array(n * 2); // loopstart + totloop
  idx = new Int32Array(idx);
  for (i=0; i<n; i++) {
    polygons[i * 2 + 0] = i * 3;
    polygons[i * 2 + 1] = 3;
  }
  if (debug) {
    if (vtx) {
      console.log(`vtx  (length: ${vtx.length.toString().padStart(5)}) = ${[...vtx].map(x=>x.toFixed(2)).join(',')}`);
    }
    if (texc) {
      console.log(`texc (length: ${texc.length.toString().padStart(5)}) = ${texc}`);
    }
    if (idx) {
      console.log(`idx  (length: ${idx.length.toString().padStart(5)}) = ${idx}`);
    }
    if (clr) {
      console.log(`clr  (length: ${clr.length.toString().padStart(5)}) = ${clr}`);
    }
    console.log(`polygons  (length: ${polygons.length.toString().padStart(5)}) = ${polygons}`);
  }
  return Mesh.fromBuffers(vtx, idx, polygons);
}

class Blender {
  static graph(items) {
    var item;
    var type;
    var blender_vertices;
    var blender_loops;
    var blender_polygons;
    var vertices_n;
    var vertex_a;
    var vertex_b;
    var vertex_c;
    var i;
    var mesh;
    // #########################
    console.log("Blender#graph", items);
    // Could be a function
    //console.log(`typeof items=${typeof items}`);
    //console.log(`items.length=${items.length}`);
    while (typeof items == "function") {
        items = items();
    }
    // Should be a mixed type array of colors/names/multivectors/lines/faces
    //console.log(`typeof items=${typeof items}`);
    //console.log(`items.length=${items.length}`);
    vertices_n = 0;
    blender_vertices = [];
    blender_loops    = [];
    blender_polygons = [];
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
        } else if (item.length == 16) {
          // Single Multivector vertex/plane/line
          //console.log(`${}, ${item[12]}, ${item[13]}`);
        }
      } else if (type == "function") {
        var e = item;
        // Motor orbits
        //if ( e.call && e.length == 2 && !e.va3) {
        //  var countx = e.dx || 32;
        //  var county = e.dy || 32;
        //  var temp = new Float32Array(3 * countx * county);
        //  var et=[];
        //  for (var pp=0,ii=0; ii<countx; ii++) for (var jj=0; jj<county; jj++,pp+=3)
        //    temp.set(Element.sw(e(ii/(countx-1),jj/(county-1)),no).slice(1,4),pp);
        //  for (ii=0; ii<countx-1; ii++) for (var jj=0; jj<county; jj++)
        //    et.push((ii+0)*county+(jj+0),(ii+0)*county+(jj+1),(ii+1)*county+(jj+1),(ii+0)*county+(jj+0),(ii+1)*county+(jj+1),(ii+1)*county+(jj+0));
        //  e.va3 = createVA(temp,undefined,et.map(x=>x%(countx*county)));
        //  e.va3.tcount = (countx-1)*county*2*3;
        //}
        // Render orbits of parametrised motors, as well as lists of points.. 
        function sw_mot_orig(A, R) {
          var a0 = A[ 0];
          var a1 = A[ 5];
          var a2 = A[ 6];
          var a3 = A[ 7];
          var a4 = A[ 8];
          var a5 = A[ 9];
          var a6 = A[10];
          var a7 = A[15];
          R[2] = -2 * (a0 * a3 + a4 * a7 - a6 * a2 - a5 * a1);
          R[1] = -2 * (a4 * a1 - a0 * a2 - a6 * a3 + a5 * a7);
          R[0] = 2 * (a0 * a1 + a4 * a2 + a5 * a3 + a6 * a7);
          return R;
        }
        if (e.call && e.length == 2 && !e.va) {
          var countx = e.dx || 64;
          var county = e.dy || 32;
          //countx = 8;
          //county = 8;
          var temp = new Float32Array(3 * countx * county);
          var o = new Float32Array(3);
          var et = [];
          // Vertices
          for (var pp = 0, ii = 0; ii < countx; ii++) {
            for (var jj = 0; jj < county; jj++, pp += 3) {
              temp.set(sw_mot_orig(e(ii / (countx - 1), jj / (county - 1)), o), pp);
            }
          }
          // Indices
          for (ii = 0; ii < countx - 1; ii++) {
            for (var jj = 0; jj < county; jj++) {
              et.push(
                (ii + 0) * county + (jj + 0),
                (ii + 0) * county + (jj + 1),
                (ii + 1) * county + (jj + 1),
                (ii + 0) * county + (jj + 0),
                (ii + 1) * county + (jj + 1),
                (ii + 1) * county + (jj + 0)
              );
            }
          }
          e.va = createVA(temp, undefined, et.map(x => x % (countx * county)));
          e.va.tcount = (countx - 1) * county * 2 * 3;
      }
      }
    } // for
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
