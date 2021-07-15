var fs = require('fs');
var name = 'quickjs-opcode-gen-out.h';
var content = fs.readFileSync(name, 'utf8');
content = content.replace(/^# .*/gm, '');
content = content.replace(/\n[\n]+/g, '\n');
fs.writeFileSync(name, content);
