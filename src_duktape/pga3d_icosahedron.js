Algebra(3,0,1,()=>{
  // Specify a point directly (trivectors specified with overloaded e-notation.)
  var point = (x,y,z)=>1e123-x*1e012+y*1e013+z*1e023;
  var rotor = (P,a)=>Math.cos(a/2)+Math.sin(a/2)*P;
  // Our camera position and orientation
  var  camera=0e0;
  // We construct faces, edges and vertices of an icosahedron.
  var r = rotor(1e13,Math.PI/2.5);
  var A = point(0,1,0);
  var B = point((1-Math.atan(0.5)**2)**0.5,Math.atan(0.5),0);
  var C = rotor(1e13,Math.PI/5)>>>(1e2>>>B);
  var items=[A,"A",B,"B",C,"C"];
  var i;
  // vertices
  items.push(0x4444FF);
  for (i=0; i<5; i++) {
    items.push(A);
    items.push(B = r >>> B);
    items.push(C = r >>> C);
    items.push(1e2 >>> A);
  }
  // edges
  items.push(0x444444);
  for (i=0; i<5; i++) {
    items.push([A, B]);
    items.push([B, C]);
    items.push([B, B = r >>> B]);
    items.push([B, C]);
    items.push([C, C = r >>> C]);
    items.push([1e2 >>> A, C]);
  }
  // faces
  items.push(0xFFCCCC);
  for (i=0; i<5; i++) {
    items.push([A, B, r >>> B]);
    items.push([B, B = r >>> B, C]);
    items.push([C, B, r >>> C]);
    items.push([C, 1e2 >>> A, C = r >>> C]);
  }
  Blender.graph(()=>{
    var time = performance.now() / 4000;    
    camera.set(rotor(1e13,time)*rotor(1e12,time*1.23131));                // animate camera
    // show more and more elements
    return items;
  });
});
