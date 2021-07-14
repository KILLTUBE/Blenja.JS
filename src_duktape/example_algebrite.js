require("Algebrite.js");

function addHTML(o) {
  //var ret = Algebrite.eval(str);
  if (o.k == Algebrite.TENSOR) {
    console.log("GOT TENSOR", o.toString());
  }
}

function toWolfram(expr) {
  if (typeof expr == "string") {
    expr = expr.replace(/\[/g, '{');
    expr = expr.replace(/\]/g, '}');
    return expr;
  } else {
    return "(unhandled type)";
  }
}

function toJS(expr) {
  
}

Algebrite.eval(`A = [
  [1, 2],
  [3, 4]
]`);

Algebrite.eval(`B = [
  [11, 22],
  [33, 44]
]`);

Algebrite.eval(`C = [
  [111, 222],
  [333, 444]
]`);

A = Algebrite.eval("A");
B = Algebrite.eval("B");
C = Algebrite.eval("C");

A.tensor.__proto__.get = function(x, y) {
  return this.elem[x * this.ndim + y];
}

var str = '';
var a;
var b;
var plus;

str += '[\n';
for (var i=0; i<2; i++) { // rows
  str += '[';
  for (var j=0; j<2; j++) { // cols
    // matrix multiplication is the dot product between A row and B col
    a_row_0 = A.tensor.get(i, 0).toString();
    a_row_1 = A.tensor.get(i, 1).toString();
    b_col_0 = B.tensor.get(0, j).toString();
    b_col_1 = B.tensor.get(1, j).toString();
    //str += `i=${i} j=${j} a=${a} b=${b}\n`;
    str += `${a_row_0} * ${b_col_0} + ${a_row_1} * ${b_col_1}`;
    // Add a comma if needed
    if (j == 0) {
      str += ', ';
    }
  }
  str += ']';
  // Add a comma if needed
  if (i == 0) {
    str += ', ';
  }
  str += '\n';
}
str += ']';
console.log(str);
// "{{1,2},{3,4}}*{{11,22},{33,44}}" -> "{{11, 44}, {99, 176}}"
toWolfram(Algebrite.eval("A*B").toString())

addHTML(A);
