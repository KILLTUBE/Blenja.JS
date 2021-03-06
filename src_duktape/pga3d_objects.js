// Create a Clifford Algebra with 3,0,1 metric. 
Algebra(3,0,1,()=>{

// rotation helper and Lathe function.     
var rot = (a,P)=>Math.cos(a)+Math.sin(a)*P.Normalized,
    lathe=(X,n,P,m)=>[...Array(n+1)].map((x,i)=>rot(i/n*Math.PI*(m||1),P)>>>X),

// wrap takes X, a double array of points, and generates triangles.    
    wrap=(X)=>{ 
      var u=X.length-1,v=X[0].length-1; X=[].concat.apply([],X);
      var P=[],vp=v+1; for(var i=0;i<u*vp;i+=vp)for(var j=0;j<v;j++)P.push([i+j,i+j+1,vp+i+j],[i+j+1,vp+i+j,vp+i+j+1]);
      return P.map(x=>x.map(x=>X[x]));
    },
    
// Basic primitives constructed by Lathing points, line segments, etc.    
    cylinder = (r=1,h=1,x=32)=>wrap(lathe([!1e0,!(1e0+r*1e3),!(1e0+r*1e3+h*1e1),!(1e0+h*1e1)],x,1e23)),
    torus    = (r=.3,r2=.25,x=32,y=16)=>wrap(lathe((1+r*.5e03)>>>lathe(!(1e0+r2*(1e1+1e3)/2**.5),y,1e13),x,1e23)),
    sphere   = (r=1,x=32,y=16)=>wrap(lathe(lathe(!(1e0+r*1e1),y,1e13,.5),x,1e23)),
    cone     = (r=1,h=1,x=64)=>wrap(lathe([!1e0,!(1e0+r*1e3),!(1e0+h*1e1)],x,1e23)),
    arrow    = ()=>[...cone(.15,.3),...cone(.15,0),...cylinder(.05,-2)],
    
// A selection of these objects.     
    objs=[arrow(),         torus(0.8,.3),       sphere(.8),        sphere(.8,3,2),
          cone(1,2**.5,3), cone(1,2**.5,4),     cone(1,2**.5),     torus(.8,.2,5,32),
          cylinder(),      cylinder(1,2**.5,4), torus(.8,.3,4,4),  torus(.8,.3,64,4)].map(x=>({data:x}));

    Blender.graph(objs)
});
