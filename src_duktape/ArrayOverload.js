
Array.prototype[Symbol.operatorSet] = Operators.create({
  "neg"(a, b) {
    var i;
    var n;
    n = a.length;
    for (i=0; i<n; i++) {
      a[i] *= -1;
    }
    return a;
  }
}, {
  left: Number,
  "-"(num, person) {
    return `${person.name} got left-multiplied with ${num}`;
  }
}, {
  right: Number,
  "-"(person, num) {
    //console.log(Object.keys(this)); // globalThis
    return `${person.name} got right-multiplied with ${num}`;
  }
});

console.clear();
a = [1,2,3]
b = -a;
--a;
