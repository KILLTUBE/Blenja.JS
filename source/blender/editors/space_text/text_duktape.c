#include "text_duktape.h"

#include <quickjs.h>

// Duktape
duk_context *ctx = NULL;
void *lines      = NULL;
// QuickJS
JSRuntime *quickjs_runtime = NULL;
JSContext *quickjs_ctx     = NULL;

duk_bool_t js_push_global_by_name(char *name) {
  duk_bool_t exists = 0;
  // #########################
	duk_push_global_object(ctx);                 // [..., global                   ]
	exists = duk_get_prop_string(ctx, -1, name); // [..., global, prop || undefined]
	duk_remove(ctx, -2);                         // [...,         prop || undefined]
	return exists;
}

void text_duktape_eval(const char *code) {
  duk_peval_string(ctx, code);
  printf("text_duktape_eval> %s\n", duk_safe_to_string(ctx, -1));
  duk_pop(ctx);
}

duk_ret_t text_duktape_log(duk_context *ctx) {
  printf("%s\n", duk_to_string(ctx, 0));
  return 0; /* no return value (= undefined) */
}

void text_duktape_fatal(void *userdata, const char *msg) {
  (void)userdata; // ignored in this case, silence warning
  // Note that 'msg' may be NULL.
  fprintf(stderr, "*** FATAL DUKTAPE ERROR: %s\n", (msg ? msg : "no message"));
  fflush(stderr);
  //abort();
}

void js_add_function(char *name, duk_c_function func, duk_idx_t nargs) {
  duk_push_c_function(ctx, func, nargs); // [..., global                   ]
  // Give actual function a name
  // Fatal Error, setter undefined (IIRC I had to hack duktape.c)
  //duk_push_string(ctx, name);            // [..., global, name             ]
  //duk_put_prop_string(ctx, -2, "name");  // [..., global,                  ]
  // #####
  duk_put_global_string(ctx, name);      // [...                           ]
}

JSValue quickjs_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  const char *str = NULL;
  int i           = 0;
  size_t len      = 0;
  // #########################
  for (i = 0; i < argc; i++) {
    if (i != 0) {
      putchar(' ');
    }
    str = JS_ToCStringLen(ctx, &len, argv[i]);
    if (!str) {
      return JS_EXCEPTION;
    }
    fwrite(str, 1, len, stdout);
    JS_FreeCString(ctx, str);
  }
  putchar('\n');
  return JS_UNDEFINED;
}


JSValue quickjsfunc_include          (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_file_get_contents(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_exe              (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_exedir           (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
JSValue quickjsfunc_reload           (JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

void quickjs_add_function(char *name, JSCFunction *funcPtr, int length) {
  JSValue global = 0;
  JSValue func   = 0;
  // #########################
  global = JS_GetGlobalObject(quickjs_ctx);
  func = JS_NewCFunction(quickjs_ctx, funcPtr, name, length);
  JS_SetPropertyStr(quickjs_ctx, global, name, func);
  JS_FreeValue(quickjs_ctx, global);
}

/**
 * \brief
 * This function can be called multiple times, but only the first call initializes the engines
 */
/// <summary>whatever</summary>
void text_duktape_init() {
  // QuickJS variables
  JSValue global_obj    = 0;
  JSValue log           = 0;
  JSValue quickjs_lines = 0;
  //JSValue console = 0;
  // #########################
  if (ctx != NULL) {
    return;
  }
  // Create context with error handler
  ctx = duk_create_heap(NULL, NULL, NULL, NULL, text_duktape_fatal);
  // global.lines = [];
  duk_push_array(ctx);
  lines = duk_get_heapptr(ctx, -1);
  duk_put_global_string(ctx, "lines");
  // Add some test data into global.lines
  duk_eval_string_noresult(ctx, "lines.push(1, 2, 3)");
  // Add functions:
  js_add_function("log"              , text_duktape_log        , 1);
  js_add_function("file_get_contents", jsfunc_file_get_contents, 1);
  js_add_function("file_put_contents", jsfunc_file_put_contents, 2);
  js_add_function("exe"              , jsfunc_exe              , 0);
  js_add_function("exedir"           , jsfunc_exedir           , 0);
  js_add_function("reload"           , jsfunc_reload           , 0);
  js_add_function("include"          , jsfunc_include          , 1);
  // QuickJS
  js_add_function("JS_NewRuntime", jsfunc_JS_NewRuntime, 0);
  js_add_function("JS_NewContext", jsfunc_JS_NewContext, 1);
  js_add_function("JS_Eval", jsfunc_JS_Eval, 2);
  // Reload (files)
  js_reload();

  // Init QuickJS aswell
  quickjs_runtime = JS_NewRuntime();
  quickjs_ctx = JS_NewContext(quickjs_runtime);
  //JSClassID test_id;
  //JSClassDef test_class;
  //JS_NewClassID(&test_id);
  //JS_NewClass(quickjs_runtime, test_id, &test_class);

  // Nice example: D:\web\quickjspp\quickjs\libc.c

  global_obj = JS_GetGlobalObject(quickjs_ctx);

  log = JS_NewCFunction(quickjs_ctx, quickjs_log, "log", 0);
  //console = JS_NewObject(quickjs_ctx);
  //JS_SetPropertyStr(quickjs_ctx, console, "log", log);
  //JS_SetPropertyStr(quickjs_ctx, global_obj, "console", console);
  JS_SetPropertyStr(quickjs_ctx, global_obj, "log", log);

  quickjs_lines = JS_NewArray(quickjs_ctx);
  JS_SetPropertyStr(quickjs_ctx, global_obj, "lines", quickjs_lines);

  quickjs_add_function("include"          , quickjsfunc_include          , 1);
  quickjs_add_function("file_get_contents", quickjsfunc_file_get_contents, 1);
  quickjs_add_function("exe"              , quickjsfunc_exe              , 0);
  quickjs_add_function("exedir"           , quickjsfunc_exedir           , 0);
  quickjs_add_function("reload"           , quickjsfunc_reload           , 0);

  JS_FreeValue(quickjs_ctx, global_obj);

  // Reload (files)
  quickjs_reload();

  quickjs_eval(
    "var dir = exedir() + '/src_duktape';                           \n"
    "// If this file exists, use the files from the repo directly.  \n"
    "// No need to mess around with copying files back and forth.   \n"
    "var dirgit = file_get_contents(exedir() + '/src_duktape.txt'); \n"
    "if (dirgit) {                                                  \n"
    "  dir = dirgit;                                                \n"
    "}                                                              \n"
    "function require(filename) {                                   \n"
    "  var ret;                                                     \n"
    "  ret = include(dir + '/' + filename);                         \n"
    "  return ret;                                                  \n"
    "}                                                              \n"
    "console.log('dir', dir);                                       \n"
    "window = {};                                                   \n"
    "require('TypeSpirit.js');                                      \n"
    "var ret = TypeSpirit.rewrite('var idk: number;').out           \n"
    "console.log('ret', ret);                                       \n"
  );
}

void text_duktape_lines_each(text_duktape_lines_each_callback cb) {
  const char *str = NULL;
  duk_idx_t i     = 0;
  duk_idx_t n     = 0;
  // #########################
  duk_push_heapptr(ctx, lines);
  n = duk_get_length(ctx, -1);
  for (i=0; i<n; i++) {
    duk_get_prop_index(ctx, -1, i);
    str = duk_to_string(ctx, -1);
    cb(i, str);
    duk_pop(ctx);
  }
}

void quickjs_lines_each(text_duktape_lines_each_callback cb) {
  const char *str = NULL;
  int32_t i       = 0;
  int64_t n       = 0;
  JSValue line    = 0;
  JSValue lines   = 0;
  JSValue global  = 0;
  int ret         = 0;
  // #########################
  global = JS_GetGlobalObject(quickjs_ctx);
  lines = JS_GetPropertyStr(quickjs_ctx, global, "lines");
  ret = JS_GetPropertyLength(quickjs_ctx, &n, lines);
  if (ret < 0) {
    cb(0, "Exception getting lines.length");
    JS_FreeValue(quickjs_ctx, lines);
    JS_FreeValue(quickjs_ctx, global);
    return;
  }
  for (i=0; i<n; i++) {
    line = JS_GetPropertyUint32(quickjs_ctx, lines, i);
    str = JS_ToCString(quickjs_ctx, line);
    cb(i, str);
    JS_FreeCString(quickjs_ctx, str);
    JS_FreeValue(quickjs_ctx, line);
  }
  JS_FreeValue(quickjs_ctx, lines);
  JS_FreeValue(quickjs_ctx, global);
}

int js_eval_file_safe(char *filename) {
  int function_exists = 0;
  int rc              = 0;
  int ret             = 0;
  // #########################
  // [..., file_get_contents || undefined]
  function_exists = js_push_global_by_name("file_get_contents");
  if (function_exists) { // [..., file_get_contents]
    // [..., file_get_contents, filename]
    duk_push_string(ctx, filename);
    // [..., ret]
    // file_get_contents(name)
    rc = duk_pcall(ctx, 1);
    if (rc != 0) { // [..., undefined]
      // e.g. Callback failed: TypeError: undefined not callable
      js_printf("js> duk_eval_file_safe(%s) failed: %s\n", filename, duk_safe_to_string(ctx, -1));
      // [...]
      duk_pop(ctx);
    }
    else { // [..., file_contents]

      //printf("GOT CONTENT %s\n", duk_safe_to_string(ctx, -1));
   // I would rather not use safeeval javascript code, but the pcompile/pcall fails to give me a full error stack print when lib.js contains parsing errors
   // Oh well, I just forgot to check the return value of duk_pcompile()
#if 0
      js_push_global_by_name(ctx, "safeeval");
      duk_push_global_object(ctx);
      //duk_push_int(ctx, (int) widget);
      duk_dup(ctx, -3);
      ret = duk_pcall(ctx, 2);
      if (ret != 0) {
        Sys_Printf("safeeval failed on C level: %s\n", duk_safe_to_string(ctx, -1));
      }
      duk_pop(ctx);
#else
  // [..., file_contents, filename]
      duk_push_string(ctx, filename);
      // [..., function || err]
      ret = duk_pcompile(ctx, 0 /*DUK_COMPILE_SAFE | DUK_COMPILE_EVAL*/); // didn't see any difference in error reporting lol

      if (ret != 0) {
        js_printf("js> duk_eval_file_safe(%s) duk_pcompile failed: %s\n", filename, duk_safe_to_string(ctx, -1));
      }
      else {

        // [..., ret]
        ret = duk_pcall(ctx, 0);
        if (ret != 0) {
          // I just cannot get the whole stack trace error :(
          //duk_get_prop_string(ctx, -1, "stack");
          //Sys_Printf("ERROR  %s\n", duk_safe_to_string(ctx, -1));
          //duk_pop(ctx);
          js_printf("js> duk_eval_file_safe(%s) duk_pcall failed: %s\n", filename, duk_safe_to_string(ctx, -1));
        }
      }
#endif

      // [...]
      duk_pop(ctx);
      return ret == 0; // returns 1 if success
    }

  }
  else { // [..., undefined]
 // [...]
    duk_pop(ctx);
  }
  return 0;
}

// e.g. js_call("callback_call", "iiii", callback_id, widget, x, y);
int js_call(char *function, char *params, ...) {
  char *stringTemp            = NULL;
  va_list args                = 0;
  int len                     = 0;
  int ret                     = 0;
  int intTemp                 = 0;
  int i                       = 0;
  int numberOfPushedArguments = 0;
  float floatTemp             = 0;
  // #########################
	// Prepare stack
	js_push_global_by_name(function);
	// Push all vars to the stack
	va_start(args, params);
	len = strlen(params);
	for (i=0; i<len; i++) {
		switch (params[i]) {
			case 'i': {
				intTemp = va_arg(args, int);
				duk_push_int(ctx, intTemp);
				numberOfPushedArguments++;
				break;
			}
			case 'f': {
				floatTemp = va_arg(args, float);
				duk_push_number(ctx, floatTemp);
				numberOfPushedArguments++;
				break;
			}
			case 's': {
				stringTemp = va_arg(args, char *);
				duk_push_string(ctx, stringTemp);
				numberOfPushedArguments++;
				break;
			}
			default:
				js_printf("script> WARNING: Identifier '%c' is not implemented!\n", params[i]);
		}
	}
	// Call the function
	ret = duk_pcall(ctx, numberOfPushedArguments);
	if (ret != 0) {
		//js_printf("script> call of \"%s\" failed: %s\n", function, duk_safe_to_string(ctx, -1));
		//printf("script> call of \"%s\" failed: %s\n", function, duk_safe_to_string(ctx, -1));
		//__asm { int 3 }
	} else {
		//Sys_Printf("js> console_key_press() ret: %s\n", duk_safe_to_string(ctx, -1));
		//func_ret = duk_to_int(ctx, -1);
		//Sys_Printf("call back call success i guess\n");
	}
	// Either case, stack needs to be cleaned
	duk_pop(ctx);
	return 1;
}

int jsfunc_file_get_contents(duk_context *ctx) {
  const char *filename = NULL;
	const char *ret      = NULL;
	FILE *f              = NULL;
	void *buf            = NULL;
	long len             = 0;
	size_t got           = 0;
  // #########################
	filename = duk_to_string(ctx, 0);
	if (!filename) {
		goto fileerror;
	}
	f = fopen(filename, "rb");
	if (!f) {
		js_printf("jsfunc_file_get_contents> Can't open file: %s\n", filename);
		goto fileerror;
	}
	if (fseek(f, 0, SEEK_END) != 0) {
		js_printf("jsfunc_file_get_contents> Can't seek to end\n");
		goto fileerror;
	}
	len = ftell(f);
	if (fseek(f, 0, SEEK_SET) != 0) {
		js_printf("jsfunc_file_get_contents> Can't seek to start\n");
		goto fileerror;
	}
	buf = duk_push_fixed_buffer(ctx, (size_t) len);
	got = fread(buf, 1, len, f);
	if (got != (size_t) len) {
		js_printf("jsfunc_file_get_contents> Can't read content\n");
		goto fileerror;
	}
	fclose(f);
	f = NULL;
	// Convert the fixed buffer to string
  ret = duk_buffer_to_string(ctx, -1);
	//js_printf("jsfunc_file_get_contents> ret=%s\n", ret); // would print the file contents
	return 1;
	fileerror:
	if (f) {
		fclose(f);
	}
	//js_printf("some error reading file in file_get_contents()\n");
	//return DUK_RET_ERROR;
	duk_push_undefined(ctx);
	return 1;
}

int jsfunc_file_put_contents(duk_context *ctx) {
  const char *filename = NULL;
	const char *text     = NULL;
	FILE *f              = NULL;
  // #########################
	filename = duk_to_string(ctx, 0);
	text = duk_to_string(ctx, 1);
	f = fopen(filename, "w");
	if (!f) {
		duk_push_int(ctx, 0);
		return 1;
	}
	fputs(text, f);
	fclose(f);
	duk_push_int(ctx, 1);
	return 1;
}

int jsfunc_get_global(duk_context *ctx) {
	duk_push_global_object(ctx);
	return 1;
}

int js_printf(char *msg, ...) {
  va_list argptr = 0;
  int ret        = 0;
  // #########################
  va_start(argptr, msg);
  ret = vprintf(msg, argptr);
  va_end(argptr);
  return ret;
}

void js_reload() {
  char *exedir;
  char filename[512];
  // #########################
  duk_push_c_function(ctx, jsfunc_exedir, 0);
  duk_pcall(ctx, 0);
  exedir = duk_safe_to_string(ctx, -1);
  duk_pop(ctx);
  js_printf("js_reload> exedir=\"%s\"\n", exedir);
  sprintf(filename, "%s\\src_duktape\\init.js", exedir);
  js_eval_file_safe(filename);
  //js_call("PostReload", "");
}

void quickjs_reload() {
  JSValue ret        = 0;
  JSValue exception  = 0;
  JSValue func       = 0;
  JSValue jsFilename = 0;
  JSValue includeRet = 0;
  const char *path   = NULL;
  const char *str    = NULL;
  char filename[512] = {0};
  // #########################
  func = JS_NewCFunction(quickjs_ctx, quickjsfunc_exedir, "exedir", 0);
  ret = JS_Call(quickjs_ctx, func, JS_UNDEFINED, 0, NULL);
  if (JS_IsException(ret)) {
    //js_std_dump_error(ctx);
    exception = JS_GetException(quickjs_ctx);
    str = JS_ToCString(quickjs_ctx, exception);
    duk_push_string(ctx, str);
    printf("quickjs_reload> exception=\"%s\"\n", str);
    JS_FreeCString(quickjs_ctx, str);
    JS_FreeValue(quickjs_ctx, exception);
    return;
  }
  path = JS_ToCString(quickjs_ctx, ret);
  sprintf(filename, "%s\\src_duktape\\init.js", path);
  //js_printf("quickjs_reload> path=\"%s\"\n", path);
  //js_printf("quickjs_reload> filename=\"%s\"\n", filename);
  jsFilename = JS_NewString(quickjs_ctx, filename);
  includeRet = quickjsfunc_include(quickjs_ctx, JS_UNDEFINED, 1, &jsFilename);
  JS_FreeValue(quickjs_ctx, ret);
  JS_FreeValue(quickjs_ctx, func);
  JS_FreeValue(quickjs_ctx, jsFilename);
  JS_FreeValue(quickjs_ctx, includeRet);
  JS_FreeCString(quickjs_ctx, path);
  //quickjs_call("PostReload", "");
}

int jsfunc_reload(duk_context *ctx) {
  js_reload();
  return 0;
}

int jsfunc_include(duk_context *ctx) {
  const char *filename = NULL;
  // #########################
  filename = duk_to_string(ctx, 0);
  js_eval_file_safe(filename);
  return 0;
}

#include <Windows.h>

int jsfunc_exe(duk_context *ctx) {
#ifdef _WIN32
  char exedir[MAX_PATH];
  HMODULE hModule;
  // #########################
  // When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
  hModule = GetModuleHandle(NULL);
  GetModuleFileName(hModule, exedir, sizeof(exedir));
  duk_push_string(ctx, exedir);
#else
  // todo for linux server
  duk_push_string(ctx, ".");
#endif
  return 1;
}

int jsfunc_exedir(duk_context *ctx) {
#ifdef _WIN32
  char exedir[MAX_PATH];
  HMODULE hModule;
  int i;
  int n;
  // #########################
  // When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
  hModule = GetModuleHandle(NULL);
  GetModuleFileName(hModule, exedir, sizeof(exedir));
  n = strlen(exedir);
  for (i=n; i>=0; i--) {
    if (exedir[i] == '\\') {
      exedir[i] = 0;
      break;
    }
  }
  duk_push_string(ctx, exedir);
#else
  // todo for linux server
  duk_push_string(ctx, ".");
#endif
  return 1;
}

// QuickJS api

int jsfunc_JS_NewRuntime(duk_context *ctx) {
  JSRuntime *rt = NULL;
  // #########################
  rt = JS_NewRuntime();
  duk_push_pointer(ctx, rt);
  return 1;
}

int jsfunc_JS_NewContext(duk_context *ctx) {
  JSRuntime *rt          = NULL;
  JSContext *ctx_quickjs = NULL;
  // #########################
  rt = (JSRuntime *) duk_to_pointer(ctx, 0);
  ctx_quickjs = JS_NewContext(rt);
  duk_push_pointer(ctx, ctx_quickjs);
  return 1;
}

int jsfunc_JS_Eval(duk_context *ctx) {
  const char *str        = NULL;
  JSContext *ctx_quickjs = NULL;
  JSValue ret       = 0;
  JSValue retloop   = 0;
  JSValue strForTag = 0;
  JSValue exception = 0;
  char defaultMessage[256] = {0};
  int tag     = 0;
  int tagloop = 0;
  int success = 0;
  uint32_t i = 0;
  int64_t n  = 0;
  duk_idx_t objectIndex = 0;
  duk_idx_t arrayIndex  = 0;
  // #########################
  ctx_quickjs = (JSContext *) duk_to_pointer(ctx, 0);
  str = duk_to_string(ctx, 1);
  ret = JS_Eval(ctx_quickjs, str, strlen(str), "<evalScript>", 0 /*flags*/);
  tag = JS_VALUE_GET_TAG(ret);
  switch (tag) {
    case JS_TAG_FLOAT64:
      duk_push_number(ctx, JS_VALUE_GET_FLOAT64(ret));
      return 1;
    case JS_TAG_INT:
      duk_push_int(ctx, JS_VALUE_GET_INT(ret));
      return 1;
    case JS_TAG_STRING:
      str = JS_ToCString(ctx_quickjs, ret);
      duk_push_string(ctx, str);
      JS_FreeCString(ctx_quickjs, str);
      return 1;
    case JS_TAG_NULL:
      duk_push_null(ctx);
      return 1;
    case JS_TAG_UNDEFINED:
      duk_push_undefined(ctx);
      return 1;
    case JS_TAG_BOOL:
      duk_push_boolean(ctx, JS_VALUE_GET_BOOL(ret));
      return 1;
    case JS_TAG_EXCEPTION:
      exception = JS_GetException(ctx_quickjs);
      str = JS_ToCString(ctx_quickjs, exception);
      duk_push_string(ctx, str);
      JS_FreeCString(ctx_quickjs, str);
      JS_FreeValue(ctx_quickjs, exception);
      return 1;
    case JS_TAG_OBJECT:
      if (JS_IsObjectPlain(ctx_quickjs, ret)) {
        objectIndex = duk_push_object(ctx);
        duk_push_string(ctx, "value");
        duk_put_prop_string(ctx, objectIndex, "key");
      }
      else if (JS_IsArray(ctx_quickjs, ret)) {
        arrayIndex = duk_push_array(ctx);
        success = JS_GetPropertyLength(ctx_quickjs, &n, ret);
        if (success == 0) {
          for (i=0; i<n; i++) {
            retloop = JS_GetPropertyUint32(ctx_quickjs, ret, i);
            tagloop = JS_VALUE_GET_TAG(retloop);
            switch (tagloop) {
              case JS_TAG_FLOAT64:
                duk_push_number(ctx, JS_VALUE_GET_FLOAT64(retloop));
                break;
              case JS_TAG_STRING:
                str = JS_ToCString(ctx_quickjs, retloop);
                duk_push_string(ctx, str);
                JS_FreeCString(ctx_quickjs, str);
                break;
              case JS_TAG_INT:
                duk_push_int(ctx, JS_VALUE_GET_INT(retloop));
                break;
              case JS_TAG_UNDEFINED:
                duk_push_undefined(ctx);
                break;
              case JS_TAG_NULL:
                duk_push_null(ctx);
                break;
              case JS_TAG_BOOL:
                duk_push_boolean(ctx, JS_VALUE_GET_BOOL(ret));
                break;
            }
            duk_put_prop_index(ctx, arrayIndex, i);
            JS_FreeValue(ctx_quickjs, retloop);
          } // for
        }
        else {
          duk_push_string(ctx, "Can't get .length property");
        }
      }
      else if (JS_IsFunction(ctx_quickjs, ret)) {
        str = JS_ToCString(ctx_quickjs, ret);
        duk_push_string(ctx, str);
        JS_FreeCString(ctx_quickjs, str);
      }
      else {
        duk_push_string(ctx, "unknown JS_TAG_OBJECT (neither array nor object)");
        
      }
      return 1;
    default:
      snprintf(defaultMessage, sizeof(defaultMessage), "jsfunc_JS_Eval: unhandled tag=%d", tag);
      duk_push_string(ctx, defaultMessage);
      return 1;
  }
}

void quickjs_push_line(JSValue line) {
  JSValue global = 0;
  JSValue lines  = 0;
  // #########################
  global = JS_GetGlobalObject(quickjs_ctx);
  lines = JS_GetPropertyStr(quickjs_ctx, global, "lines");
  // JSValue js_array_push(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, int unshift);
  js_array_push(quickjs_ctx, lines, 1, &line, 0);
  JS_FreeValue(quickjs_ctx, global);
  JS_FreeValue(quickjs_ctx, lines);
}

void quickjs_eval(char *str) {
  const char *tostr = NULL;
  JSValue ret       = 0;
  JSValue exception = 0;
  // #########################
  ret = JS_Eval(quickjs_ctx, str, strlen(str), "<quickjs_eval>", 0 /*flags*/);
  if (JS_IsException(ret)) {
    exception = JS_GetException(quickjs_ctx);
    tostr = JS_ToCString(quickjs_ctx, exception);
    //printf("quickjs_eval exception> %s\n", tostr);
    JS_FreeCString(quickjs_ctx, tostr);
    quickjs_push_line(exception);
    JS_FreeValue(quickjs_ctx, exception);
    return;
  }
#if 0
  tostr = JS_ToCString(quickjs_ctx, ret);
  printf("quickjs_eval> %s\n", tostr);
  JS_FreeCString(quickjs_ctx, tostr);
#endif
  quickjs_push_line(ret);
}

uint8_t *js_load_file(JSContext *ctx, size_t *pbuf_len, const char *filename) {
  FILE *f        = NULL;
  uint8_t *buf   = NULL;
  size_t buf_len = 0;
  long lret      = 0;
  // #########################
  f = fopen(filename, "rb");
  if (!f) {
    return NULL;
  }
  if (fseek(f, 0, SEEK_END) < 0) {
    goto fail;
  }
  lret = ftell(f);
  if (lret < 0) {
    goto fail;
  }
  /* XXX: on Linux, ftell() return LONG_MAX for directories */
  if (lret == LONG_MAX) {
    errno = EISDIR;
    goto fail;
  }
  buf_len = lret;
  if (fseek(f, 0, SEEK_SET) < 0) {
    goto fail;
  }
  if (ctx) {
    buf = js_malloc(ctx, buf_len + 1);
  } else {
    buf = malloc(buf_len + 1);
  }
  if (!buf) {
    goto fail;
  }
  if (fread(buf, 1, buf_len, f) != buf_len) {
    errno = EIO;
    if (ctx) {
      js_free(ctx, buf);
    } else {
      free(buf);
    }
  fail:
    fclose(f);
    return NULL;
  }
  buf[buf_len] = '\0';
  fclose(f);
  *pbuf_len = buf_len;
  return buf;
}

/* load and evaluate a file */
JSValue quickjsfunc_include(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  uint8_t *buf         = NULL;
  const char *filename = NULL;
  JSValue ret          = 0;
  size_t buf_len       = 0;
  // #########################
  filename = JS_ToCString(ctx, argv[0]);
  if (!filename) {
    return JS_EXCEPTION;
  }
  buf = js_load_file(ctx, &buf_len, filename);
  if (!buf) {
    JS_ThrowReferenceError(ctx, "could not load '%s'", filename);
    JS_FreeCString(ctx, filename);
    return JS_EXCEPTION;
  }
  ret = JS_Eval(ctx, (char *)buf, buf_len, filename, JS_EVAL_TYPE_GLOBAL);
  js_free(ctx, buf);
  JS_FreeCString(ctx, filename);
  return ret;
}

/* load and evaluate a file */
JSValue quickjsfunc_file_get_contents(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  uint8_t *buf         = NULL;
  const char *filename = NULL;
  JSValue ret          = 0;
  size_t buf_len       = 0;
  // #########################
  filename = JS_ToCString(ctx, argv[0]);
  if (!filename) {
    return JS_EXCEPTION;
  }
  buf = js_load_file(ctx, &buf_len, filename);
  if (!buf) {
    JS_ThrowReferenceError(ctx, "could not load '%s'", filename);
    JS_FreeCString(ctx, filename);
    return JS_EXCEPTION;
  }
  ret = JS_NewStringLen(ctx, (char *)buf, buf_len);
  js_free(ctx, buf);
  JS_FreeCString(ctx, filename);
  return ret;
}

JSValue quickjsfunc_exe(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  JSValue ret = 0;
  // #########################
#ifdef _WIN32
  char path[MAX_PATH];
  HMODULE hModule;
  // #########################
  // When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
  hModule = GetModuleHandle(NULL);
  GetModuleFileName(hModule, path, sizeof(path));
  ret = JS_NewString(quickjs_ctx, path);
#else
  ret = JS_NewString(quickjs_ctx, "todo");
#endif
  return ret;
}

JSValue quickjsfunc_exedir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  JSValue ret = 0;
  // #########################
#ifdef _WIN32
  char path[MAX_PATH];
  HMODULE hModule;
  int i;
  int n;
  // #########################
  // When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
  hModule = GetModuleHandle(NULL);
  GetModuleFileName(hModule, path, sizeof(path));
  n = strlen(path);
  for (i=n; i>=0; i--) {
    if (path[i] == '\\') {
      path[i] = 0;
      break;
    }
  }
  ret = JS_NewString(quickjs_ctx, path);
#else
  // todo for linux server
  ret = JS_NewString(quickjs_ctx, "todo");
#endif
  return ret;
}

JSValue quickjsfunc_reload(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  quickjs_reload();
  return JS_UNDEFINED;
}
