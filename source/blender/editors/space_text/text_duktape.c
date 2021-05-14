#include "text_duktape.h"

duk_context *ctx = NULL;
void *lines = NULL;

duk_bool_t js_push_global_by_name(char *name) {
  duk_bool_t exists;
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

void text_duktape_init() {
  // Allow to call this function multiple times
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
  // Reload (files)
  js_reload();
}

void text_duktape_lines_each(text_duktape_lines_each_callback cb) {
  duk_idx_t i;
  duk_idx_t n;
  const char *str;
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

int js_eval_file_safe(char *filename) {
  // [..., file_get_contents || undefined]
  int function_exists = js_push_global_by_name("file_get_contents");
  if (function_exists) { // [..., file_get_contents]
    // [..., file_get_contents, filename]
    duk_push_string(ctx, filename);
    // [..., ret]
    // file_get_contents(name)
    int rc = duk_pcall(ctx, 1);
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
      int ret = duk_pcall(ctx, 2);
      if (ret != 0) {
        Sys_Printf("safeeval failed on C level: %s\n", duk_safe_to_string(ctx, -1));
      }
      duk_pop(ctx);
#else
  // [..., file_contents, filename]
      duk_push_string(ctx, filename);
      // [..., function || err]
      int ret = duk_pcompile(ctx, 0 /*DUK_COMPILE_SAFE | DUK_COMPILE_EVAL*/); // didn't see any difference in error reporting lol

      if (ret != 0) {
        js_printf("js> duk_eval_file_safe(%s) duk_pcompile failed: %s\n", filename, duk_safe_to_string(ctx, -1));
      }
      else {

        // [..., ret]
        int ret = duk_pcall(ctx, 0);
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
	// prepare stack
	js_push_global_by_name(function);
	// push all vars to the stack
	va_list args;
	va_start(args, params);
	int len = strlen(params);
	int i;
	int numberOfPushedArguments = 0;
	for (i=0; i<len; i++) {
		switch (params[i]) {
			case 'i': {
				int tmp = va_arg(args, int);
				duk_push_int(ctx, tmp);
				numberOfPushedArguments++;
				break;
			}
			case 'f': {
				float tmp = va_arg(args, float);
				duk_push_number(ctx, tmp);
				numberOfPushedArguments++;
				break;
			}
			case 's': {
				char *tmp = va_arg(args, char *);
				duk_push_string(ctx, tmp);
				numberOfPushedArguments++;
				break;
			}
			default:
				js_printf("script> WARNING: Identifier '%c' is not implemented!\n", params[i]);
		}
	}
	// call the function
	int ret = duk_pcall(ctx, numberOfPushedArguments);
	if (ret != 0) {
		//js_printf("script> call of \"%s\" failed: %s\n", function, duk_safe_to_string(ctx, -1));
		//printf("script> call of \"%s\" failed: %s\n", function, duk_safe_to_string(ctx, -1));
		//__asm { int 3 }
	} else {
		//Sys_Printf("js> console_key_press() ret: %s\n", duk_safe_to_string(ctx, -1));
		//func_ret = duk_to_int(ctx, -1);
		//Sys_Printf("call back call success i guess\n");
	}
	// either case, stack needs to be cleaned
	duk_pop(ctx);
	return 1;
}

int jsfunc_file_get_contents(duk_context *ctx) {
	const char *filename = duk_to_string(ctx, 0);
	FILE *f = NULL;
	long len;
	void *buf;
	size_t got;
	char *ret = NULL;
	if (!filename) {
		goto fileerror;
	}
	f = fopen(filename, "rb");
	if (!f) {
		//js_printf("Cant open file: %s\n", filename);
		//printf("cant open file: %s", filename);
		goto fileerror;
	}
	if (fseek(f, 0, SEEK_END) != 0) {
		
		//js_printf("cant seek to end\n");
		goto fileerror;
	}
	len = ftell(f);
	if (fseek(f, 0, SEEK_SET) != 0) {
		//js_printf("cant seek to start\n");
		goto fileerror;
	}
	buf = duk_push_fixed_buffer(ctx, (size_t) len);
	got = fread(buf, 1, len, f);
	if (got != (size_t) len) {
		//js_printf("cant read content\n");
		goto fileerror;
	}
	fclose(f);
	f = NULL;
	// convert the fixed buffer to string
	//ret = (char *) duk_to_string(ctx, -1);
  ret = (char*)duk_buffer_to_string(ctx, -1);
	//printf("ret=%s\n", ret); // would print the file contents
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
	char *filename = (char *)duk_to_string(ctx, 0);
	char *text = (char *)duk_to_string(ctx, 1);
	FILE *f = fopen(filename, "w");
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
  va_list argptr;
  va_start(argptr, msg);
  int ret = vprintf(msg, argptr);
  va_end(argptr);
  return ret;
}

void js_reload() {
  duk_push_c_function(ctx, jsfunc_exedir, 0);
  duk_pcall(ctx, 0);
  char *exedir = duk_safe_to_string(ctx, -1);
  duk_pop(ctx);
  js_printf("JS root: %s\n", exedir);
  char filename[512];
  sprintf(filename, "%s\\src_duktape\\init.js", exedir);
  js_eval_file_safe(filename);
  //sprintf(filename, "%s\\src_duktape\\Console.js", exedir);
  //js_eval_file_safe(filename);
  //js_eval_file_safe("assets\\javascript\\pre_create.js");
  //js_eval_file_safe("./javascript/init.js");
  //js_eval_file_safe("F:\\repos\\OpenDF2\\OpenDF2\\codemp\\javascript\\printf.js");
  //js_eval_file_safe("assets\\javascript\\lib_quake.js");
  //js_call("PostReload", "");
}

int jsfunc_reload(duk_context *ctx) {
  js_reload();
  return 0;
}

int jsfunc_include(duk_context *ctx) {
  char *filename = (char *) duk_to_string(ctx, 0);
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
