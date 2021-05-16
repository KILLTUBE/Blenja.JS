var TypeSpirit = {};

/**
 * @param {string} path 
 * @returns {string}
 */
function normalize(path) {
  var parts = path.split('/');
  var newParts = [];
  var started = false; // keep a path like '../../somewhere' as it is
  for (var part of parts) {
    if (part == '..' && started) {
      newParts.pop();
    } else if (part == '.') {
      // skip paths like '/./'
    } else {
      if (part != '..') {
        started = true;
      }
      newParts.push(part);
    } 
  }
  return newParts.join('/');
}

/**
 * @returns {string}
 */
function join() {
  var ret = '';
  var slash = '';
  for (var arg of arguments) {
    ret += slash + arg;
    slash = '/';
  }
  ret = ret.replace(/\/\/+/g, function(got, pos, str) {
    var prevChar = str[pos-1];
    if (prevChar == ':') {
      return got;
    }
    return '/';
  });
  return ret;
}

/**
 * @param {string} path 
 * @returns {string}
 */
function dirname(path) {
  var ret = '.';
  var i = path.lastIndexOf('/');
  if (i != -1) {
    ret = path.substr(0, i);
  }
  return ret;
}

class ImporterWeb {
  debug = false;
  base = 'src/';
  cache = {}; // keep track which file is read/parsed
  time = 0;
  ignore = [];

  /**
   * @param {WebOptions} options
   */
  constructor(options) {
    this.options = options;
    Object.assign(this, options);
  }

  log() {
    if (!this.debug) {
      return;
    }
    console.log(...arguments);
  }

  /**
   * @param {string} path 
   * @returns {string}
   */
  properPath(path) {
    var _;
    if (path.startsWith('http')) {
      _ = path;

      if (path.endsWith('.js') || path.endsWith('.ts')) {

      } else {
        _ += '.ts';
      }

    } else if (path.endsWith('.js') || path.endsWith('.ts'))
      _ = join(this.base, path);
    else {
      _ = join(this.base, path + ".ts");
    }
    return _;
  }

  /**
   * @param {string} path 
   * @returns {Promise<TypeSpiritRet | undefined>}
   */
  async readTypeScript(path) {
    path = this.properPath(path);
    try {
      var res = await fetch(path);
      if (!res.ok) {
        return undefined;
      }
      var txt = await res.text();
      //
      var start = performance.now();
      var typeSpirit = rewrite(txt, {
        keepImport: false,
        base: this.base
      });
      var end = performance.now();
      this.time += end - start;
      this.cache[path] = typeSpirit;
      return typeSpirit;
    } catch (e) {
      console.log(`> readTypeScript("${path}"): ${e}`);
      return undefined;
    }
  }

  /**
   * @param {string} path 
   * @returns {Promise}
   */
  async parseRecursively(path) {
    path = this.properPath(path);
    path = normalize(path);
    this.log(`>parse_recursively("${path}")`);
    if (this.cache[path]) {
      this.log("Cached path", path);
      return;
    }
    var typeSpirit = await this.readTypeScript(path);
    if (typeSpirit == undefined) {
      this.log(`readTypeScript(${JSON.stringify(path)}) failed`);
      return;
    }
    await this.onImport(typeSpirit);
    for (var importurl in typeSpirit.imports) {
      if (this.ignore.includes(importurl)) {
        this.log(`ignore importurl: ${importurl}`);
        continue;
      }
      var importpath = join(dirname(path), importurl);
      await this.parseRecursively(importpath);
    }
  }

  /**
   * @param {TypeSpiritRet} typeSpirit
   */
  async onImport(typeSpirit) {
    // extend class and do what you want
  }
}

class BundlerWeb extends ImporterWeb {
  bundle = '';
  names = {};

  /**
   * @param {TypeSpiritRet} typeSpirit
   */
  async onImport(typeSpirit) {
    this.bundle += typeSpirit.out;
    Object.assign(this.names, typeSpirit.names);
  }

  /**
   * @returns {string}
   */
  get code() {
    var out = '';
    if (this.debug) {
      out += 'debugger;\n';
    }
    out += this.bundle;
    out += '\n\nreturn {\n';
    var names_ = Object.keys(this.names);
    for (var name of names_) {
      out += `  ${name},\n`;
    }
    out += '};\n';
    return out;
  }

  cacheBundlers = {};
  cacheLibs = {};
  async imp0rt(path) {
    path = this.properPath(path);
    if (this.cacheLibs[path]) {
      this.log("imp0rt cache", path);
      return this.cacheLibs[path];
    }
    this.log('imp0rt', path);
    var importBundler = new BundlerWeb(this.options);
    this.cacheBundlers[path] = importBundler;
    await importBundler.parseRecursively(path);
    var lib = importBundler.getLib();
    this.cacheLibs[path] = lib;
    this.log('importBundler lib', lib);
    return lib;
  }

  getLib() {
    var func = new Function('imp0rt', this.code);
    var imp0rt = this.imp0rt.bind(this);
    return func(imp0rt);
  }

  run() {
    try {
      return eval(this.code);
    } catch (e) {
      console.log(`> BundlerWeb#run(): ${e}`);
      return undefined;
    }
  }
}

/**
 * @param {string} str 
 * @returns {boolean}
 */
function isPascalCaseArray(str) {
  return /^([A-Z]+[a-z0-9]+)+_?([A-Za-z0-9]+)*(\[\])*$/.test(str);
}

/**
 * @param {string} str 
 * @returns {boolean}
 */
function isStockTypeArray(str) {
  return /^(any|boolean|number|string|undefined|null|void)(\[\])*$/.test(str);
}

/**
 * @param {string} str 
 * @returns {boolean}
 */
function isStockType(str) {
  if (
    str == "any" ||
    str == "boolean" ||
    str == "number" ||
    str == "string" ||
    str == "undefined" ||
    str == "null" ||
    str == "void"
  ) {
    return true;
  }
  return false;
}

/**
 * @param {string} str
 * @returns {boolean}
 */
function isEnumType(str) {
  if (str.trim() == "") {
    return false;
  }
  var parts = str.split(',');
  for (var part of parts) {
    part = part.trim();
    if (!isType(part)) {
      return false;
    }
  }
  return true;
}

/**
 * @param {string} str 
 * @returns {boolean}
 * @example
 * isTemplate('  PR4omIse  < pc.Model  > ')
 */
function isTemplate(str) {
  return /^\s*([A-Z]+[a-z0-9_$]+)+\s*<.+>\s*$/g.test(str);
}

/**
 * @param {string} str 
 * @returns {boolean}
 */
function isPascalCase(str) {
  return /^([A-Z]+[a-z0-9]+)+_?([A-Za-z0-9]+)*$/.test(str);
}

/**
 * @param {string} str 
 * @returns {boolean}
 */
function isType(str) {
  if (isStockTypeArray(str)) {
    return true;
  }
  if (isPascalCaseArray(str)) {
    return true;
  }
  if (isTemplate(str)) {
    return true;
  }
  var lastDot = str.lastIndexOf('.');
  if (lastDot >= 0) {
    var lastPart = str.substr(lastDot + 1);
    if (isPascalCaseArray(lastPart)) {
      return true;
    }
  }
  return false;
}

/**
 * @param {string} str 
 * @returns {boolean}
 */
function isUnionType(str) {
  if (str.trim() == "") {
    return false;
  }
  var parts = str.split('|');
  for (var part of parts) {
    part = part.trim();
    if (!isType(part)) {
      return false;
    }
  }
  return true;
}

/**
 * @param {string} str
 * @returns {object}
 */
function rewrite(str, options) {
  var keepImport = true;
  var base;
  if (options) {
    if (typeof options.keepImport != 'undefined') {
      keepImport = options.keepImport;
    }
    if (typeof options.base != 'undefined') {
      base = options.base;
    }
  }
  var bundle = !keepImport;
  var prevToken;
  var prevRealToken;
  var Tokens = {
    Unknown     :  0,
    Comment     :  1,
    String      :  2,
    Name        :  3,
    PascalCase  :  9,
    Spaces      : 10,
    Comma       : 11,
    Assign      : 12,
    Hex         : 13,
    Integer     : 14,
    Newline     : 15,
    QuestionMark: 17,
    Literal     : 18,
    Arrow       : 19,
    RegEx       : 20,
    Import      : 21,
    Class       : 30,
     0: 'Unknown',
     1: 'Comment',
     2: 'String',
     3: 'Name',
     9: 'PascalCase',
    10: 'Spaces',
    11: 'Comma',
    12: 'Assign',
    13: 'Hex',
    14: 'Integer',
    15: 'Newline',
    17: 'QuestionMark',
    18: 'Literal',
    19: 'Arrow',
    20: 'RegEx',
    21: 'Import',
    30: 'Class'
  };
  var i;
  var n = str.length;
  var char;
  var nextChar;
  var prevChar;
  var out = '';
  var j;
  var next;
  var curlyBraceLevel;
  var afterEqual;
  var checkTypeOf;
  var name = '';
  var c;
  var type = Tokens.Unknown;
  var token = '';
  var stack = [];
  var debug = false;
  var debugFrames = false;
  var endless = 0;
  var frames = [];
  var frame;
  var imports = {};
  var importsFunctions = {};
  var inExport;
  var names = {};
  function nextToken() {
    prevToken = token;
    char = str[i];
    nextChar = str[i + 1];
    token = '';
    if (char == '/') {
      if (nextChar == '/') {
        token += '//';
        i += 2;
        for (; i<n; i++) {
          token += str[i];
          if (str[i+1] == '\n') {
            i++;
            break;
          }
        }
        return Tokens.Comment;
      } else if (nextChar == '*') {
        token += '/*';
        i += 2;
        for (; i<n; i++) {
          char = str[i];
          nextChar = str[i + 1];
          token += char;
          if (char == '*' && nextChar == '/') {
            token += '/';
            i+=2;
            break;
          }
        }
        return Tokens.Comment;
      } else {
        token += char;
        i++;
        // Check if it could be a RegEx,
        // in these cases it can't be confused with a div sign:
        // str.replace( /something/g, '')
        // regex = /something/g;
        // regexes = [/a/g, /b/g, /c/g]
        if (
          prevRealToken == '(' ||
          prevRealToken == '=' ||
          prevRealToken == '[' ||
          prevRealToken == ','
        ) {
          // This could be a regex now, skip the entire line
          for (; i<n; i++) {
            c = str[i];
            token += c;
            if (c == '\n' || c == '\r') {
              token += c;
              return Tokens.Unknown;
            }
            // like `a = /\//g`
            if (c == '\\') {
              // eat next char
              token += str[i + 1];
              i++;
              continue;
            }
            if (str[i] == '/') {
              i++;
              // check modifiers, all: gmisuy
              if (str[i] == 'g') {
                token += 'g';
                i++;
              }
              break;
            }
          }
          if (debug) {
            token = `<REGEX>token=${JSON.stringify(token)} prevToken=${JSON.stringify(prevRealToken)}</REGEX>`;
          }
          return Tokens.RegEx;
        }
        // Normal div sign...
        return Tokens.Unknown;
      }
    }
    else if (char == '"') {
      token += char;
      while (i++ < n) {
        char = str[i];
        token += char;
        if (char == '\\') {
          // eat next char
          token += str[i + 1];
          i++;
          continue;
        }
        if (char == '"') {
          i++;
          break;
        }
      }
      return Tokens.String;
    }
    else if (char == "'") {
      token += char;
      while (i++ < n) {
        char = str[i];
        token += char;
        if (char == '\\') {
          // eat next char
          token += str[i + 1];
          i++;
          continue;
        }
        if (char == "'") {
          i++;
          break;
        }
      }
      return Tokens.String;
    }
    else if (char == '`') {
      token += char;
      while (i++ < n) {
        char = str[i];
        token += char;
        if (char == '\\') {
          // eat next char
          token += str[i + 1];
          i++;
          continue;
        }
        if (char == '`') {
          i++;
          break;
        }
      }
      return Tokens.String;
    } else if (char >= 'A' && char <= 'Z') {
      getName();
      token = name;
      return Tokens.PascalCase;
    } else if (char >= 'a' && char <= 'z') {
      getName();
      token = name;
      switch (token) {
        case 'enum':
          return tokenEnum;
        case 'export':
          return tokenExport;
        case 'interface':
          return tokenInterface;
        case 'declare':
          return tokenDeclare;
        case 'type':
          return tokenType;
        case 'import':
          return tokenImport;
        case 'new':
          return tokenNew;
        case 'as':
          return tokenAs;
        case 'public':
          return tokenPublic;
        case 'var':
        case 'const':
        case 'let':
          return tokenVar;
        case 'function':
          return tokenFunction;
        case 'async':
          return outExportToken;
        case 'class':
          return tokenClass;
        case 'this':
          return tokenThis;
      }
      return Tokens.Literal;
    } else if (char == ',') {
      i++;
      token = char;
      return Tokens.Comma;
    } else if (char == ':') {
      i++;
      token = char;
      return tokenColon;
    } else if (char == ' ' || char == '\t') {
      for (; i<n; i++) {
        c = str[i];
        if (c != ' ' && c != '\t') {
          break;
        }
        token += c;
      }
      return Tokens.Spaces;
    } else if (char == '=') {
      i++;
      if (nextChar == '>') {
        i++;
        token = '=>';
        return Tokens.Arrow;
      } else {
        token = '=';
        return Tokens.Assign;
      }
    } else if (char == '\n' || char == '\r') {
      token = char;
      i++;
      return Tokens.Newline;
    } else if (char == '?') {
      token = '?';
      i++;
      return Tokens.QuestionMark;
    } else if (char == '0' && nextChar == 'x') {
      token = '0x';
      i += 2;
      for (; i<n; i++) {
        c = str[i];
        if (c >= '0' && c <= '9') {
          token += c;
          continue;
        }
        if (c == '_') {
          token += c;
          continue;
        }
        break;
      }
      return Tokens.Hex;
    } else if (char >= '0' && char <= '9') {
      token += char;
      i++;
      for (; i<n; i++) {
        c = str[i];
        if (c >= '0' && c <= '9') {
          token += c;
          continue;
        }
        if (c == '_') {
          token += c;
          continue;
        }
        break;
      }
      return Tokens.Integer;
    }
    token = char;
    i++;
    return Tokens.Unknown;
  }
  function skipSpaces() {
    for (; i<n; i++) {
      if (str[i] != ' ' && str[i] != '\t') {
        break;
      }
    }
  }
  function nextTokenReal() {
    while (i < n) {
      type = nextToken();
      if (type == Tokens.Spaces) {
        continue;
      }
      if (type == Tokens.Newline) {
        continue;
      }
      break;
    }
  }
  function getName() {
    for (name=''; i<n; i++) {
      c = str[i];
      if (c >= 'a' && c <= 'z') {
        name += c;
        continue;
      }
      if (c >= 'A' && c <= 'Z') {
        name += c;
        continue;
      }
      if (c == '_' || c == '$') {
        name += c;
        continue;
      }
      if (name.length) {
        if (c >= '0' && c <= '9') {
          name += c;
          continue;
        }
      }
      break;
    }
  }
  function skipNewline() {
    if (str[i] == '\n') {
      i++;
    }
  }
  function advanceToUnnestedSemicolon() {
    // find next semicolon not inside any curly braces
    curlyBraceLevel = 0;
    while (i++ < n) {
      c = str[i];
      if (c == '{') {
        curlyBraceLevel++;
      } else if (c == '}') {
        curlyBraceLevel--;
      } else if (c == ';') {
        if (curlyBraceLevel == 0) {
          // found end of type
          break;
        }
      }
    }
    i++;
    //skipNewline();
  }
  function tokenEnum() {
    // demand at least one space
    if (str[i] != ' ') {
      nvm();
      return;
    }
    skipSpaces();
    getName();
    if (!isPascalCase(name)) {
      nvm();
      return;
    }
    outExport();
    var start = str.indexOf('{', i) + 1;
    var end = str.indexOf('}', i);
    if (start == -1 || end == -1) {
      return;
    }
    out += 'var ';
    out += name;
    out += ' = {\n';
    if (str[start] == '\n') {
      start++;
    }
    var state = '';
    var startValue = 0;
    var map = [];
    var keyname = '';
    var spaces = '';
    for (i=start; i<end;) {
      type = nextToken();
      if (type == Tokens.PascalCase) {
        state = 'pascal';
        keyname = token;
      } else if (type == Tokens.Hex || type == Tokens.Integer) {
        try {
          startValue = eval(token);
        } catch (e) {
          out += `<EVALERROR>${token}</EVALERROR>`;
        }
      } else if (type == Tokens.Comma) {
        map.push([keyname, Number(startValue)]);
        //out += ': ' + Number(startValue);
        state = '';
        //out += token;
        startValue++;
      } else if (type == Tokens.Spaces) {
        // ignore spaces if in Pascal state
        if (state == '') {
          //out += token;
        }
      } else if (type == Tokens.Newline) {
        //state = '';
        if (state != 'assign') {
          //out += token;
          
        }
      } else {
        if (state != '') {
          // close up
        }
        //out += `TOKEN: type=${Tokens[type]} token=${token}\n`;
        //if (token == '}') {}
      }
      //out += `i=${i} token=${Tokens[type]} >>>${token}<<<\n`;
      //out += `>>>${str.substr(i, 10).replace(/\n/g,'⤶')}<<<`;
    }
    //out += `state: '${state}'\n`;
    if (state == 'pascal') {
      // there was no comma, but still an assignment
      map.push([keyname, Number(startValue)]);
    }
    spaces = '';
    // add original amount of spaces
    for (j=end-1; j>=0; j--) {
      if (str[j] != ' ') {
        break;
      }
      spaces += ' ';
    }
    var gen = '';
    for (j=0; j<map.length; j++) {
      var m = map[j];
      var a = m[0];
      var b = m[1];
      
      gen += `${spaces}  ${a}: ${b},\n`;
    }
    for (j=0; j<map.length; j++) {
      var m = map[j];
      var a = m[0];
      var b = m[1];
      gen += `${spaces}  ${b}: '${a}'`;
      if (j != map.length - 1) {
        gen += ',';
      }
      gen += '\n';
    }
    out += gen;
    out += spaces;
    out += '}';
    i++;
  }
  function tokenImport() {
    // skip any import('') in code
    if (frames.length > 0) {
      stack.push(i);
      nextTokenReal(); // should be '('
      nextTokenReal(); // should be the url
      //i = stack.pop();
      var url = token.slice(1, -1); // remove single/double quotes
      importsFunctions[url] = [];
      nextTokenReal(); // should be ')'
      if (url.startsWith('http')) {
        out += `imp0rt('${url}')`;
      } else {
        if (base) {
          out += `imp0rt('${join(base, url)}')`;
        } else {
          out += `imp0rt('${url}')`;
        }
      }
      return;
    }
    var state = '';
    var _ = '';
    _ += token;
    var vars = [];
    while (i < n) {
      type = nextToken();
      if (state == 'collect') {
        if (type == Tokens.Literal || type == Tokens.PascalCase) {
          vars.push(token);
        } else if (token == '}') {
          state = '';
        }
      }
      if (type == Tokens.String) {
        if (keepImport) {
          var lastChar = token[token.length - 1];
          token = token.slice(0, -1);
          token += '.js';
          token += lastChar;
          _ += token;
        }
        imports[token.slice(1, -1)] = vars;
        break;
      } else if (type == tokenType) {
        _ += '/* type */';
      } else {
        if (token == '{') {
          state = 'collect';
        }
        _ += token;
      }
    }
    stack.push(i);
    nextTokenReal();
    if (token == ';') {
      _ += ';';
    } else {
      //nvm();
      i = stack.pop();
    }
    if (keepImport) {
      out += _;
    } else {
      stack.push(i);
      type = nextToken();
      if (type != Tokens.Newline) {
        i = stack.pop();
      }
      stack.push(i);
      type = nextToken();
      if (type != Tokens.Newline) {
        i = stack.pop();
      }
    }
  }
  function tokenNew() {
    nextTokenReal();
    if (!isPascalCase(token)) {
      nvm();
      return;
    }
    out += 'new ' + token;
    nextTokenReal();
    if (token != '<') {
      out += token;
    } else {
      var counter = 1;
      while (i < n) {
        type = nextToken();
        if (token == '<') {
          counter++;
        } else if (token == '>') {
          counter--;
          if (counter == 0) {
            break;
          }
        } else {
          if (counter == 0) {
            out += token;
          }
        }
      }
    }
  }
  function tokenAs() {
    var collect = '';
    var counter = 0;
    out += '/* as ';
    while (i < n) {
      nextTokenReal();
      collect += token;
      if (token == '<') {
        counter++;
      } else if (token == '>') {
        counter--;
      }
      if (counter == 0) {
        if (!isType(collect)) {
          out += ' */';
          out += token;
          break;
        }
      }
      out += token;
    }
  }
  function tokenInterface() {
    nextTokenReal();
    var interfaceName = token;
    nextTokenReal(); // {
    // find next closing curly brace
    next = str.indexOf('}', i);
    if (next != -1) {
      var code = str.substring(i, next)
      code = code.trim();
      i = next + 1;
      //skipNewline(); // todo: remove spaces aswell, need peekToken.type == TS?
      var what = '';
      outExport();
      out += `${what}/** @interface */ var ${interfaceName} = ${JSON.stringify(code)};`;
    }
  }
  function tokenDeclare() {
    // find next semicolon
    next = str.indexOf(';', i);
    if (next != -1) {
      i = next + 1;
    }
  }
  function tokenType() {
    // demand at least one space
    if (str[i] != ' ') {
      nvm();
      return;
    }
    skipSpaces();
    //out += str.substr(i, 20);
    // Demand pascal case
    if (str[i] >= 'A' && str[i] <= 'Z') {
      nextTokenReal();
      name = token;
      nextTokenReal(); // {
      var start = i;
      // its pretty probably that this is a TS type now
      // so just go ahead and find the end of it
      //if (str[i] == '=') {
        //afterEqual = i + 1;
        // find next semicolon not inside any curly braces
        outExport();
        var what = '';
        advanceToUnnestedSemicolon();
        var code = str.substring(start, i);
        code = code.trim();
        out += `${what}/** @type */ var ${name} = ${JSON.stringify(code)};`;
        // skip all this from output
        //checkTypeOf = str.substring(afterEqual, i);
        //if (isUnionType(checkTypeOf)) {
          //i = j;
        //} // looks like a type
      //} // has equal sign
    } // pascal case
    else {
      nvm();
    }
  }
  function indexOfMany(chars) {
    var ret = str.length;
    for (c of chars) {
      var _ = str.indexOf(c, i);
      if (_ != -1) {
        ret = Math.min(ret, _);
      }
    }
    return ret;
  }
  function skipAllLiteralColonComma() {
    while (i < n) {
      nextTokenReal();
      if (type != Tokens.Literal) {
        break;
      }
      nextTokenReal();
      if (type != tokenColon) {
        break;
      }
      nextTokenReal();
      if (type != Tokens.Literal) {
        break;
      }
      nextTokenReal();
      if (type != Tokens.Comma) {
        break;
      }
    }
  }
  function showToken() {
    var asJSON = JSON.stringify(token);
    out += `/** @Token { type: ${Tokens[type]}, token==${asJSON}} */`;
  }
  function skipCallback() {
    skipAllLiteralColonComma();
    if (token != ')') {
      return false;
    }
    nextTokenReal();
    if (type != Tokens.Arrow) {
      return false;
    }
    nextTokenReal();
    if (type != Tokens.Literal) {
      return false;
    }
    nextTokenReal();
    if (token == ';') {
      out += ';';
      return true;
    }
    if (token == '=') {
      out += ' =';
      return true;
    }
    return false;
  }
  function skipTypemap(lastI) {
    var end = str.indexOf('}', lastI);
    if (end != -1) {
      i = end + 1;
    }
  }
  function skipTuple(lastI) {
    var end = str.indexOf(']', lastI);
    if (end == -1) {
      nvm();
      return;
    }
    checkTypeOf = str.substring(lastI, end);
    //out += `\ncheckTypeOf=='${checkTypeOf}''\n`;
    if (isEnumType(checkTypeOf)) {
      i = end + 1;
    } else {
      nvm();
      return;
    }
  }
  function tokenPublic() {
    if (frame && frame.name == 'class' && frame.i == 1) {
      skipSpaces();
    }
  }
  function tokenColon() {
    var end;
    var lastI;
    var valid;
    if (!frame) {
      nvm();
      return;
    }
    valid = false;
    // class attributes
    if (frame.name == 'class' && frame.i == 1) {
      valid = true;
    }
    // function arguments
    if (frame.name == 'function' && frame.i == 0) {
      valid = true;
    }
    // var types
    if (frame.name == 'var' && frame.i == 0) {
      valid = true;
    }
    if (!valid) {
      nvm();
      return;
    }
    //showFrame();
    end = indexOfMany(',}{);\n=');
    lastI = i;
    valid = true;
    nextTokenReal();
    if (token == '(') {
      valid = skipCallback();
      if (!valid) {
        nvm();
      }
      return;
    }
    if (token == '{') {
      skipTypemap(lastI);
      return;
    }
    if (token == '[') {
      skipTuple(lastI);
      return;
    }
    if (str[end - 1] == ' ') {
      end--;
    }
    checkTypeOf = str.substring(lastI, end);
    if (debug) {
      console.log("checkTypeOf", checkTypeOf);
    }
    if (isUnionType(checkTypeOf)) {
      i = end;
    } else {
      nvm();
    }
  }
  function showFrame() {
    if (frame) {
      out += `/** @frame name=${frame.name} i=${frame.i} */`;
    } else {
      out += `/** @frame none */`;
    }
  }
  function outExport() {
    if (keepImport && inExport) {
      out += 'export ';
    }
  }
  function outExportToken() {
    outExport();
    out += token;
  }
  function addFrame(name) {
    frames.push({
      name,
      i: 0
    });
    if (debugFrames) {
      out += `/** @addFrame ${name} */`;
    }
  }
  function addName() {
    if (frame) {
      return;
    }
    var ii = i;
    nextTokenReal();
    i = ii;
    if (type != Tokens.PascalCase && type != Tokens.Literal) {
      return;
    }
    if (names[token]) {
      console.warn(`name ${token} already exists`);
    }
    names[token] = {i};
  }
  function tokenClass() {
    addFrame('class');
    outExportToken();
    addName();
  }
  function tokenFunction() {
    addFrame('function');
    outExportToken();
    addName();
  }
  function tokenVar() {
    addFrame('var');
    outExportToken();
    addName();
  }
  function tokenThis() {
    if (frame && frame.name == 'function' && frame.i == 0) {
      //out += '/* this */';
      nextTokenReal();
      tokenColon();
      stack.push(i);
      nextTokenReal();
      //showToken();
      if (token != ',') {
        nvm();
        return;
      }
      skipSpaces();
    } else {
      out += 'this';
    }
  }
  function handleExportMap() {
    if (bundle) {
      var end = str.indexOf('}', i);
      if (end != -1) {
        out += '// skipped export { ... }\n';
        i = end + 1;
      }
    } else {
      nvm();
    }
  }
  function tokenExport() {
    nextTokenReal();
    if (token == 'default') {
      nextTokenReal();
    }
    inExport = true;
    if (typeof type == 'function') {
      type();
    } else {
      if (token == '{') {
        handleExportMap();
      } else {
        nvm();
      }
    }
  }
  function handleToken() {
    if (token == '=' || token == ';' || token == 'of' || token == 'in') {
      if (frame) {
        if (frame.name == 'var') {
          if (debugFrames) {
            out += '/** @leaveFrame var */';
          }
          frames.pop();
        }
      }
    } else if (token == '{') {
      if (frame) {
        frame.i++;
        if (debugFrames) {
          showFrame();
        }
      } else {
        if (debugFrames) {
          out += '/*NO-FRAME*/';
        }
      }
    } else if (token == '}') {
      if (frame) {
        frame.i--;
        if (frame.i == 0) {
          if (debugFrames) {
            out += `/** @leaveFrame ${frame.name} */`;
          }
          frames.pop();
        }
      } else {
        if (debugFrames) {
          out += '/*NO-FRAME*/';
        }
      }
    }
    frame = frames[frames.length - 1];
    // process all other tokens
    inExport = false; // reset in leaving frame?
    if (typeof type == 'function') {
      type();
    } else if (type == Tokens.QuestionMark) {
      nextTokenReal();
      if (type == tokenColon) {
        tokenColon();
      } else if (token == ';' || token == ')' || token == ',') {
        out += token;
      } else {
        nvm();
      }
    } else {
      if (debug) {
        var t = Tokens[type];
        if (token == '\n') {
          out += `<${t}>⤶</${t}>\n`;
        } else {
          out += `<${t}>${token}</${t}>\n`;
        }
      } else {
        out += token;
      }
    }
  }
  function nvm() {
    // If it is neither type/interface/enum,
    // then it is a normal JS expression.
    // So restore position from stack and just copy it all
    if (debug) {
      out += '\n> NVM\n';
    }
    i = stack.pop();
    out += str[i];
    i++;
    char = str[i];
  }
  if (debug) {
    out += `n=${n}\n`;
  }
  i = 0;
  while (i < n) {
    if (endless++ > n) {
      console.log('endless i', i, Tokens[type], token);
      break;
    }
    // just in case export is not a type, we have to restore old i
    // e.g. for 'export function'
    stack.push(i);
    if (type != Tokens.Spaces && type != Tokens.Newline) {
      prevRealToken = token;
    }
    type = nextToken();
    handleToken();
  }
  if (debug) {
    out += `\n> endless=${endless}\n`;
    out += `\n> stack=${stack.join(', ')}\n`;
  }
  return {
    imports,
    importsFunctions,
    out,
    names
  };
}

Object.assign(TypeSpirit, {
  isPascalCaseArray,
  isStockTypeArray,
  isStockType,
  isEnumType,
  isType,
  isTemplate,
  isUnionType,
  rewrite,
  ImporterWeb,
  BundlerWeb,
  normalize,
  join,
  dirname
});

window.TypeSpirit = TypeSpirit;
