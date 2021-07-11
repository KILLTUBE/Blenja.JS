function Console() {
  // nothing
}

Console.prototype.log = function() {
  var out = '';
  var arg;
  for (var i=0; i<arguments.length; i++) {
    arg = arguments[i];
    if (arg instanceof Array) {
      // If entire array is < 60 chars, don't add newlines.
      // Not the most performance way, could also iterate and break when > 60 condition hits
      var totalLength = sum(arg.map(x=>x.toString().length));
      if (totalLength > 60) {
        out += '[\n';
        out += joinBeforeAfter(arg, '  ', ',\n');
        out += '\n]';
      } else {
        out += '[';
        out += arg.join(',');
        out += ']';
      }
    } else {
      out += arg;
    }
    out += ' ';
  }
  out.split('\n').forEach(line => {
    log(line); // to console
    lines.push(line); // to Blender editor
  })
}

Console.prototype.warn = function() {
  var i = lines.length;
  log('WARN: ');
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
