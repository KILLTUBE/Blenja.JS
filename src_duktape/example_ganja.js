require("ganja.js")
console.clear();
Algebra(3,0,1, ()=>{
    var a = Element.Vector(1, 2, 3, 4);
    var b = Element.Vector(4, 3, 2, 1);
    var c = b >>> a;
    console.log('a', a);
    console.log('b', b);
    console.log('c', c);
});
