var TypeSpirit = {};

/**
 * @param {object} lib
 */

function etterizeIntoWindow(lib) {
  let keys;
  keys = Object.keys(lib);
  for (let key of keys) {
    // only set getters/setters once
    if (Object.getOwnPropertyDescriptor(window.__proto__, key)) {
      continue;
    }
    Object.defineProperty(
      window.__proto__,
      key, {
        get: function() {
          return lib[key]
        },
        set: function(newValue) {
          lib[key] = newValue;
        }
    });
  }
}

function getDir() {
  var href = window.location.href;
  var last = href.lastIndexOf('/');
  return href.substr(0, last);
}

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
      _ = join(this.base, path + '.ts');
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
      console.log(`> readTypeScript('${path}'): ${e}`);
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
    this.log(`>parse_recursively('${path}')`);
    if (this.cache[path]) {
      this.log('Cached path', path);
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
  looseBirds = [];

  /**
   * @param {TypeSpiritRet} typeSpirit
   */
  async onImport(typeSpirit) {
    this.bundle += typeSpirit.out;
    // If last file ends with // without \n, the next files first line would be commented out
    this.bundle += '\n';
    Object.assign(this.names, typeSpirit.names);
    this.looseBirds.push(...typeSpirit.looseBirds);
    //console.log('Got looseBirds', this.looseBirds)
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
    if (this.looseBirds.length) {
      out += '\n\n// looseBirds\n';
      out += this.looseBirds.join('\n');
      out += '\n';
    }
    out += '\n\nreturn {\n';
    var names_ = Object.keys(this.names);
    for (var name of names_) {
      out += `  ${name},\n`;
    }
    out += '};\n';
    return out;
  }

  /**
   * Copy of `get code` without the names
   * @returns {string}
   */
  get code2() {
    var out = '';
    if (this.debug) {
      out += 'debugger;\n';
    }
    out += this.bundle;
    if (this.looseBirds.length) {
      out += '\n\n// looseBirds\n';
      out += this.looseBirds.join('\n');
      out += '\n';
    }
    return out;
  }

  cacheBundlers = {};
  cacheLibs = {};
  async imp0rt(path) {
    path = this.properPath(path);
    if (this.cacheLibs[path]) {
      this.log('imp0rt cache', path);
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
function isLiteralArray(str) {
  return /^([A-Za-z0-9_]+)(\[\])*$/.test(str);
}

/**
 * @param {string} str 
 * @returns {boolean}
 */
function isStockTypeArray(str) {
  return /^(any|unknown|object|boolean|number|string|undefined|null|void)(\[\])*$/.test(str);
}

/**
 * @param {string} str 
 * @returns {boolean}
 */
function isStockType(str) {
  if (
    str == 'any' ||
    str == 'unknown' ||
    str == 'object' ||
    str == 'boolean' ||
    str == 'number' ||
    str == 'string' ||
    str == 'undefined' ||
    str == 'null' ||
    str == 'void'
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
  if (str.trim() == '') {
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
 * @example ```js
 * isTuple('[number,number]');
 * ```
 * @summary Does not contain spaces, as the "real" tokens are just concatenated
 * @param {string} str 
 * @returns {boolean}
 */
function isTuple(str) {
  if (str[0] != '[' || str[str.length-1]!=']') {
    return false;
  }
  str = str.substring(1, str.length - 1);
  // only works for simple tuples like [number,number]
  var parts = str.split(',');
  // Doesn't work for: [Coords3D, Coords3D] eg.
  //// parts.find basically?
  //for (var part of parts) {
  //  if (!isStockType(part)) {
  //    return false;
  //  }
  //}
  //return true;
  // Just return true if more than one field:
  return parts.length > 1;
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
function isType(str) {
  var c = str[0];
  // Something like: var dir: 'left' | 'right';
  if (c == "'" || c == '"') {
    return true;
  }
  // Numbers are types too: var oneTwoOrThree: 1 | 2 | 3;
  if (c > '0' && c < '9') {
    return true;
  }
  if (isStockTypeArray(str)) {
    return true;
  }
  if (isLiteralArray(str)) {
    return true;
  }
  if (isTemplate(str)) {
    return true;
  }
  if (isTuple(str)) {
    return true;
  }
  var lastDot = str.lastIndexOf('.');
  if (lastDot >= 0) {
    var lastPart = str.substr(lastDot + 1);
    if (isLiteralArray(lastPart)) {
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
  if (str.trim() == '') {
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
  var looseBirds = [];
  var prevToken;
  var prevRealToken;
  var Tokens = {
    Unknown     :  0,
    Comment     :  1,
    String      :  2,
    Name        :  3,
    Spaces      : 10,
    Comma       : 11,
    Assign      : 12,
    Hex         : 13,
    Integer     : 14,
    Newline     : 15,
    Literal     : 18,
    Arrow       : 19,
    RegEx       : 20,
    Import      : 21,
    Class       : 30,
     0: 'Unknown',
     1: 'Comment',
     2: 'String',
     3: 'Name',
    10: 'Spaces',
    11: 'Comma',
    12: 'Assign',
    13: 'Hex',
    14: 'Integer',
    15: 'Newline',
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
        token = '//';
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
        token = '/*';
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
        token = char;
        i++;
        // Check if it could be a RegEx,
        // in these cases it can't be confused with a div sign:
        // str.replace( /something/g, '')
        // regex = /something/g;
        // regexes = [/a/g, /b/g, /c/g]
        //jsdoc({prevRealToken});
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
              console.warn('Considered a RegEx but reached end of line', {i});
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
              // check modifiers
              for (; i<n; i++) {
                if (!'gmisuy'.includes(str[i])) {
                  break;
                }
                token += str[i];
              }
              //jsdoc({regex: token})
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
    } else if (char == '"' || char == "'" || char == '`') {
      var dropout = char;
      token = char;
      while (i++ < n) {
        char = str[i];
        token += char;
        if (char == '\\') {
          // eat next char
          token += str[i + 1];
          i++;
          continue;
        }
        if (char == dropout) {
          i++;
          break;
        }
      }
      return Tokens.String;
    } else if (char >= 'A' && char <= 'Z') {
      getName();
      token = name;
      return Tokens.Literal;
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
          return tokenPublicPrivate;
        case 'private':
          return tokenPublicPrivate;
        case 'protected':
          return tokenPublicPrivate;
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
        case 'implements':
          return tokenImplements;
        case 'readonly':
          return tokenReadonly;
        case 'abstract':
          return tokenAbstract;
        case 'extends':
          return tokenExtends;
        case 'super':
          return tokenSuper;
        case 'constructor':
          return tokenConstructor;
      }
      return Tokens.Literal;
    } else if (char == ' ' || char == '\t') {
      for (; i<n; i++) {
        c = str[i];
        if (c != ' ' && c != '\t') {
          break;
        }
        token += c;
      }
      return Tokens.Spaces;
    } else if (char == '=' || char == '!') {
      if (char == '=' && nextChar == '>') {
        i += 2;
        token = '=>';
        return Tokens.Arrow;
      } else {
        // Check longest possibility first
        var allThree = char + nextChar + str[i + 2];
        //console.log(all)
        if (allThree == '!==' || allThree == '===') {
          //console.log('allThree', allThree)
          token = allThree;
          i += 3;
          return Tokens.Unknown;
        }
        var both = char + nextChar;
        //console.log('both', both)
        if (both == '==' || both == '!=') {
          token = both;
          i += 2;
          return Tokens.Unknown;
        }
        i++;
        token = char;
        return Tokens.Assign;
      }
    } else if (char == '\n' || char == '\r') {
      token = char;
      i++;
      return Tokens.Newline;
    } else if (char == '?') {
      token = char;
      i++;
      return tokenQuestionMark;
    } else if (char == '(') {
      token = char;
      i++;
      return tokenParenthesis;
    } else if (char == ',') {
      i++;
      token = char;
      return Tokens.Comma;
    } else if (char == ':') {
      i++;
      token = char;
      return tokenColon;
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
      token = char;
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
    nextTokenReal();
    if (type != Tokens.Literal) {
      nvm();
      return;
    }
    nextTokenReal();
    if (token != '{') {
      nvm();
      return;
    }
    outExport();
    out += 'var ' + name + ' = {\n';
    var state = '';
    var startValue = 0;
    var all = [];
    var keyname = '';
    var spaces = '';
    while (i<n) {
      nextTokenReal();
      if (type == Tokens.Literal) {
        state = 'literal';
        keyname = token;
      } else if (type == Tokens.Hex || type == Tokens.Integer) {
        try {
          startValue = eval(token);
        } catch (e) {
          out += `<EVALERROR>${token}</EVALERROR>`;
        }
      } else if (type == Tokens.Comma) {
        all.push([keyname, Number(startValue)]);
        state = '';
        startValue++;
      } else if (token == '}') {
        if (state == 'literal') {
          // there was no comma, but still an assignment
          all.push([keyname, Number(startValue)]);
        }
        break;
      } else {
        // Usually comments and '='
        //jsdoc({unhandled: token});
        //out += '\n';
      }
    }
    spaces = '';
    // add original amount of spaces
    for (j=i-1; j>=0; j--) {
      if (str[j] != ' ') {
        break;
      }
      spaces += ' ';
    }
    var keyval = all.map(([key, val])=>`${spaces}  ${key}: ${val}`);
    var valkey = all.map(([key, val])=>`${spaces}  ${val}: '${key}'`);
    out += keyval.concat(valkey).join(',\n') + '\n';
    out += spaces + '}';
    i++;
  }
  // vars = []; jsdoc({vars})
  // jsdoc({i});
  // Output: /** @vars [] */
  function jsdoc() {
    out += '/** ';
    [...arguments].forEach(arg=>{
      if (typeof arg == 'string') {
        out += '@' + arg + ' ';
      } else if (arg instanceof Array) {
        out += JSON.stringify(arg) + 'arr ';
      } else if (arg instanceof Object) {
        Object.keys(arg).forEach(key => {
          var val = arg[key];
          out += '@' + key + ' ';
          out += JSON.stringify(val) + ' ';
        });
      } else {
        out += JSON.stringify(arg) + ' ';
      }
    })
    out += '*/';
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
    skipSpaces()
    //jsdoc({i});
    nextTokenReal();
    //jsdoc('token', {token})
    if (token == '*') {
      var tokens = [];
      nextTokenReal(); // token == 'as'
      tokens.push(token);
      nextTokenReal(); // token == 'abc'
      tokens.push(token);
      nextTokenReal(); // token == 'from'
      tokens.push(token);
      nextTokenReal(); // token == './abc'
      tokens.push(token);
      nextTokenReal(); // token == ';'
      tokens.push(token);
      // TODO... not so easy?
      var msg = {Unsupported: 'import * ' + tokens.join(' ')};
      jsdoc       (msg);
      console.warn(msg);
    } else {
      var state = '';
      var _ = '';
      _ += token;
      var vars = [];
      while (i < n) {
        type = nextToken();
        if (state == 'collect') {
          if (type == Tokens.Literal) {
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
    skipSpaces();
    if (str[i] == ':') {
      out += 'as';
    } else {
      skipType();
    }
  }
  function tokenInterface() {
    nextTokenReal();
    var interfaceName = token;
    var counter = 0;
    var tokens = [];
    // find next closing curly brace
    while (i < n) {
      nextTokenReal();
      if (token == '{') {
        counter++;
      } else if (token == '}') {
        counter--;
        if (counter == 0) {
          break;
        }
      }
      tokens.push(token);
    }
    // TODO: Some global helper function to accumulate/collect all interfaces for the same interfaceName
    outExport();
    out += `\n/** @interface */ var ${'interface' + interfaceName} = ${JSON.stringify(tokens)};`;
  }
  function tokenDeclare() {
    nextTokenReal();
    if (token == 'interface') {
      tokenInterface();
    } else {
      advanceToUnnestedSemicolon();
    }
  }
  function tokenType() {
    // demand at least one space
    if (str[i] != ' ') {
      nvm();
      return;
    }
    nextTokenReal();
    //jsdoc({token});
    name = token;
    nextTokenReal(); // token == '=' || token == '<'
    var start = i;
    //jsdoc({token});
    // its pretty probably that this is a TS type now
    // so just go ahead and find the end of it
    if (token == '<') {
      start--;
      nextTokenReal(); // token == 'T'
      nextTokenReal(); // token == '>'
      nextTokenReal(); // token == '='
    }
    if (token == '=') {
      //afterEqual = i + 1;
      // find next semicolon not inside any curly braces
      outExport();
      advanceToUnnestedSemicolon();
      var code = str.substring(start, i);
      code = code.trim();
      out += `/** @type */ var tuple${name} = ${JSON.stringify(code)};`;
      // skip all this from output
      //checkTypeOf = str.substring(afterEqual, i);
      //jsdoc({checkTypeOf})
      //if (isUnionType(checkTypeOf)) {
        //i = j;
      //} // looks like a type
    } else {
      nvm();
    }
  }
  function inArgs() {
    if (!frame) {
      return false;
    }
    if (frame.name == 'function' && frame.i == 0) {
      return true;
    } else if (frame.name == 'fatArrowMulti' && frame.i == 0) {
      return true;
    } else if (frame.name == 'fatArrowSingle') {
      return true;
    } else if (frame.name == 'class' && frame.i == 1) {
      // catches attributes too, for tokenParenthesis
      return true;
    }
    return false;
  }
  function tokenQuestionMark() {
    if (inArgs()) {
      return;
    }
    out += '?';
  }
  function tokenParenthesis() {
    // If this turns out to be a fat arrow function: add a function frame
    // So when we detect the next ':', we can assume to safely remove types
    // find the closing )
    var bracelevel = 1; // ( equaling first level
    var oldI = i;
    // i here is one after '('
    while (i < n) {
      c = str[i++];
      if (c == '(') {
        bracelevel++;
        //out += `/** @bracelevel++ ${bracelevel}*/`;
      } else if (c == ')') {
        var till = i;
        bracelevel--;
        //out += `/** @bracelevel-- ${bracelevel}*/`;
        if (bracelevel == 0) {
          nextTokenReal();
          // Detect fat arrow functions with a return type attached
          if (token == ':') {
            skipType();
            nextTokenReal(); // Maybe token == '=>'
            //jsdoc({token})
          }
          if (token == '=>') {
            nextTokenReal();
            if (inArgs()) {
              // Before our token function is called, the brace level was increased
              // Now we resolved it as callback, so we have to reduce it
              frame.bracelevel--;
              // Skip callback type in function arguments entirely
              return;
            } else {
              if (token == '{') {
                var f = addFrame('fatArrowMulti');
                f.bracelevel = 1; // Starting at 1 unlike tokenFunction (which starts at 0)
              } else {
                var f = addFrame('fatArrowSingle');
                f.bracelevel = 1;
                f.till = till; // unused atm
              }
            }
          }
          break;
        }
      }
    }
    out += '(';
    i = oldI;
    token = '('; // restore for e.g. prevRealToken
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
    if (token == ')') {
      out += ')';
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
    checkTypeOf = str.substring(lastI, end + 1);
    checkTypeOf = checkTypeOf.trim();
    //jsdoc({checkTypeOf});
    if (isTuple(checkTypeOf)) {
      i = end + 1;
    } else {
      nvm();
      return;
    }
  }
  function tokenPublicPrivate() {
    if (inClass() && frame.i == 1) {
      skipSpaces();
    } else {
      out += token;
    }
  }
  function skipType() {
    var valid;
    var ii;
    //showFrame();
    //end = indexOfMany(',}{);=');
    var lastI = i;
    //valid = true;
    nextTokenReal();
    if (token == '(') {
      valid = skipCallback();
      if (!valid) {
        i = lastI;
        //nvm();
      }
      return;
    }
    i = lastI;
    //if (token == '{') {
    //  skipTypemap(lastI);
    //  return;
    //}
    //if (token == '[') {
    //  skipTuple(lastI);
    //  return;
    //}
    //if (str[end - 1] == ' ') {
    //  end--;
    //}
    //checkTypeOf = str.substring(lastI, end);
    //if (debug) {
    //}
    //  console.log('checkTypeOf', checkTypeOf);
    var collect = '';
    var counter = 0;
    var wait = false;
    var first = true;
    while (i < n) {
      ii = i;
      nextTokenReal();
      if (first && token == '{' || (counter && token == '{')) {
        counter++;
      } else if (token == '<' || token == '[') {
        counter++;
      } else if (token == '>' || token == ']' || token == '}') {
        counter--;
        wait = true;
      }
      //console.log({counter})
      if (counter < 0) {
        // happens in a statement like (last as running into ]):
        // return [prediction as tf.Tensor2D, decodedBounds, scores as tf.Tensor1D];
        i = ii;
        break;
      }
      if (counter == 0 && wait == false) {
        if (',}{);='.includes(token[0])) {
          //console.log('break type at ', token, 'collect', collect, 'ii', ii)
          i = ii;
          break;
        }
      }
      collect += token;
      wait = false;
      first = false;
    }
    // Uncomment, nice to see all types used in project
    //console.log('break type at ', token, 'collect', collect, 'i', i, 'ii', ii)
    //if (isUnionType(checkTypeOf)) {
    //  i = end;
    //} else {
    //  nvm();
    //}
    return collect;
  }
  function tokenColon() {
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
    if ((frame.name == 'function' || frame.name == 'fatArrowMulti') && frame.i == 0) {
      valid = true;
    }
    // ()=>123 arguments
    if (frame.name == 'fatArrowSingle') {
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
    skipType();
  }
  function showFrame() {
    if (frame) {
      var _ = Object.keys(frame).map(key => `${key}=${frame[key]}`).join(' ');
      out += `/** @frame ${_} */`;
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
    var ret = {
      name,
      i: 0
    } 
    frames.push(ret);
    if (debugFrames) {
      out += `/** @addFrame ${name} */`;
    }
    return ret;
  }
  function peekTokenReal() {
    var ii = i;
    nextTokenReal();
    i = ii;
  }
  function addName() {
    if (frame || !inExport) {
      return;
    }
    peekTokenReal();
    if (type != Tokens.Literal) {
      return;
    }
    if (names[token]) {
      console.warn(`name ${token} already exists`);
    }
    names[token] = {i};
  }
  function skipGeneric() {
    peekTokenReal();
    if (token == '<') {
      nextTokenReal(); // token == '<'
      nextTokenReal(); // token == 'T'
      nextTokenReal(); // token == '>'
    }
  }
  function tokenClass() {
    var classframe = addFrame('class');
    classframe.bracelevel = 0;
    addName();
    nextTokenReal();
    classframe.classname = token;
    if (!inExport) {
      // Work around code like: var gen = class Gen;
      out += 'window.' + token + ' = ';
    } else {
      outExport();
    }
    out += 'class';
    out += ' ' + token; // add classname
    skipGeneric();
  }
  function tokenFunction() {
    var funcframe = addFrame('function');
    funcframe.bracelevel = 0;
    outExportToken();
    addName();
    nextTokenReal();
    out += ' ' + token; // add classname
    skipGeneric();
  }
  function tokenVar() {
    addFrame('var');
    outExportToken();
    addName();
  }
  function tokenThis() {
    if (frame && frame.name == 'function' && frame.i == 0) {
      //out += '/* this */';
      nextTokenReal(); // token == ':'
      //showToken();
      skipType();
      var ii = i;
      nextTokenReal();
      if (token != ',') {
        i = ii;
        return;
      }
      skipSpaces();
    } else {
      out += 'this';
    }
  }
  function tokenImplements() {
    out += '/* ';
    out += token;
    out += ' ';
    nextTokenReal(); // todo: nextType() which skips <> and {} and [] etc. like tokenAs
    out += token;
    out += ' */';
  }
  function tokenReadonly() {
    if (frame.bracelevel == 0) {
      // must be inside a constructor, not a readonly class attribute e.g.
      return;
    }
    nextTokenReal();
    out += token;
    if (!frame.readonlies) {
      frame.readonlies = [];
    }
    frame.readonlies.push(token);
  }
  // TODO: classes in classes probably can't depend on `if (frame)` alone
  function tokenAbstract() {
    if (frame) {
      var code;
      var start = i;
      advanceToUnnestedSemicolon();
      code = str.substring(start+1, i);
      out += `/* abstract ${JSON.stringify(code)} */`;
    } else {
      out += '/* abstract */';
    }
  }
  function nextExpression() {
    var collect = '';
    var counter = 0;
    while (i < n) {
      nextTokenReal();
      if        (token == '<' || token == '[' || token == '{' || token == '(') {
        counter++;
      } else if (token == '>' || token == ']' || token == '}' || token == ')') {
        counter--;
      }
      collect += token;
      //console.log({counter});
      if (counter <= 0) {
        // Negative counter happens in a statement like (last as running into ]):
        // return [prediction as tf.Tensor2D, decodedBounds, scores as tf.Tensor1D];
        break;
      }
    }
    return collect;
  }
  function keepExtends(_) {
    return _ == 'pc.ScriptType' || _ == 'Float32Array' || _ == 'Float64Array' || _ == 'Array' || _ == 'generator';
  }
  function tokenExtends() {
    if (inClass()) {
      //var _ = nextExpression(); // class X extends (a || b)
      var _ = skipType();
      frame.extends = _;
      // TODO options: {keepExtends = ['pc.ScriptType']}
      if (keepExtends(_)) {
        out += 'extends ' + _;
      } else {
        out += '/* extends ' + _ + ' */';
        looseBirds.push(`${frame.classname}.prototype.__proto__  = ${_}.prototype`);
      }
    }
  }
  function tokenSuper() {
    //showFrame();
    if (inClass() && !keepExtends(frame.extends)) {
      nextTokenReal();
      if (token == '(') { // Constructor call looks like: super(name)
        // Only call constructor when it exists
        var c = `${frame.extends}.prototype.constructorTypeSpirit`;
        out += `if (${c}) ${c}.call(this, `;
        //nextTokenReal
      } else if (token == '.') { // Method call looks like: super.sell();
        nextTokenReal();
        var method = token;
        nextTokenReal(); // remove '('
        out += `${frame.extends}.prototype.${method}.call(this, `;
      } else {
        console.warn('unhandled super statement');
        out += token;
      }
    } else {
      out += token;
    }
  }
  function inClass() {
    return frame && frame.name == 'class';
  }
  function tokenConstructor() {
    if (inClass() && frame.i == 1 && !keepExtends(frame.extends)) {
      out += 'constructor() {\n';
      out += '    this.constructorTypeSpirit(...arguments);\n';
      out += '  }\n';
      out += '  constructorTypeSpirit';
      frame.hasConstructor = true;
    } else {
      out += token;
    }
  }
  function handleExportMap() {
    if (bundle) {
      while (i<n) {
        nextTokenReal();
        if (token == '}') {
          break;
        }
        //out += `/** @Got ${token} */\n`; // export names and commas probably
      }
      var lastI = i;
      nextTokenReal();
      if (token == 'from') {
        nextTokenReal(); // eat path, e.g. './types'
        if (str[i] == ';') {
          i++; // skip semicolon
        }
      } else {
        i = lastI;
      }
    } else {
      nvm();
    }
  }
  function tokenExport() {
    var ii = i;
    // If we are inside a frame, just keep it, e.g. a method called export
    if (frame) {
      out += token;
      return;
    }
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
        peekTokenReal();
        if (token == ';') {
          nextTokenReal(); // eat ;
        }
        var what = 'export' + str.substring(ii, i);
        console.warn('Unsupported:', what);
        out += `// Unsupported: ${what}`;
      }
    }
  }
  function popFrame() {
    //if (frame.name == 'class' && !frame.hasConstructor && frame.extends) {
    //  // This class extends another class without having a constructor
    //  // So we have to add a constructor calling the parent constructor
    //  showFrame();
    //  out += '\n';
    //  out += `  constructor() {\n`;
    //  out += `    return new ${frame.extends}(...arguments);\n`;
    //  out += `  }\n`;
    //}
    frames.pop();
    inExport = false;
    if (debugFrames) {
      out += `/** @leaveFrame ${frame.name} */`;
    }
  }
  function handleToken() {
    //debugFrames = true;
    frame = frames[frames.length - 1];
    if (frame) {
      var hasBracelevel = frame.name == 'function' || frame.name == 'fatArrowMulti' || frame.name == 'fatArrowSingle' || frame.name == 'class';
      if (hasBracelevel) {
        if (token == '(') {
          frame.bracelevel++;
          //jsdoc({bracelevel: frame.bracelevel})
        } else if (token == ')') {
          frame.bracelevel--;
          //jsdoc({bracelevel: frame.bracelevel})
        }
      }
      if (token == '=' || token == ';' || token == 'of' || token == 'in') {
        if (frame.name == 'var' && frame.i == 0) {
          popFrame();
        } else if (hasBracelevel && token == '=' && frame.i == 0) {
          addFrame('defaultArg');
        }
      } else if (token == '{') {
        if (hasBracelevel && frame.bracelevel == 1) {
          var tmp = addFrame('funcObjectDestruct')
          tmp.i = 1;
        } else {
          frame.i++;
          if (debugFrames) {
            showFrame();
          }
          if (frame.readonlies) {
            out += '{\n' + frame.readonlies.map(x=>`    this.${x} = ${x};`).join('\n');
            delete frame.readonlies;
            return;
          }
        }
      } else if (token == '}') {
        frame.i--;
        if (frame.i == 0) {
          popFrame();
        }
      } else if (token == ')' && frame.name == 'fatArrowSingle') {
        // Depending on ) token is probably enough, otherwise integrate it at some point:
        //out += `@fatArrowSingle i=${i} till=${frame.till}`;
        var oldI = i;
        nextTokenReal(); // Maybe: token == ':'
        if (token == ':') {
          skipType();
        } else {
          i = oldI;
        }
        token = ')'; // Add back initial: token == ')'
        popFrame();
      } else if ((token == ',' || token == ')') && frame.name == 'defaultArg') {
        if (frame.i == 0) {
          popFrame();
          if (token == ')') {
            // Handle this token once more, to reduce bracelevel of the hasBracelevel frame
            i--;
            return; // Don't add token twice
          }
        }
      }
    }
    // process all other tokens
    if (typeof type == 'function') {
      type();
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
      //console.log({i, prevRealToken, token});
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
    names,
    looseBirds
  };
}

Object.assign(TypeSpirit, {
  etterizeIntoWindow,
  isLiteralArray,
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
