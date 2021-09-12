function objectToString(o) {
  var keys = Object.keys(o);
  return '{' + keys.map(key=>{
    `${key}: ${o[key]}`
  }).join(',') + '}';
}

function Console() {
  // nothing
}

Console.prototype.log = function() {
  var out = '';
  var arg;
  for (var i=0; i<arguments.length; i++) {
    arg = arguments[i];
    // Handle highest elements in prototype chain first
    if (arg instanceof Array) {
      // If entire array is < 60 chars, don't add newlines.
      // Not the most performant way, could also iterate and break when > 60 condition hits
      var totalLength = 0;
      try {
        totalLength = sum(arg.map(x=>x.toString().length));
      } catch (e) {
        //console.log('try-catch Array');
      }
      if (totalLength > 60) {
        out += '[\n';
        out += joinBeforeAfter(arg, '  ', ',\n');
        out += '\n]';
      } else {
        out += '[';
        out += arg.join(',');
        out += ']';
      }
    } else if (arg instanceof Function) {
      out += arg;
    } else if (arg instanceof Object) {
      out += objectToString(arg);
    } else {
      out += arg;
    }
    out += ' ';
  }
  out.split('\n').forEach(line => {
    println(line); // to console
    lines.push(line); // to Blender editor
  })
}

Console.prototype.warn = function() {
  var i = lines.length;
  println('WARN: ');
  Console.prototype.log.apply(console, arguments);
  lines[i] = 'WARN: ' + lines[i];
}

Console.prototype.clear = function() {
  lines.length = 0;
}

if (typeof console == 'undefined') {
  console = new Console();
} else {
  console.__proto__ = Console.prototype;
}
