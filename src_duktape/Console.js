Console = function() {
	// nothing
}

Console.prototype.log = function() {
	var line = '';
	for (var i=0; i<arguments.length; i++) {
		line += arguments[i];
		line += ' ';
	}
	log(line); // to console
	lines.push(line); // to Blender editor
}

Console.prototype.clear = function() {
	lines.length = 0;
}

if (typeof console == 'undefined') {
	console = new Console();
} else {
	console.__proto__ = Console.prototype;
}
