
enum {
    __JS_ATOM_NULL = JS_ATOM_NULL,
JS_ATOM_null,
JS_ATOM_false,
JS_ATOM_true,
JS_ATOM_if,
JS_ATOM_else,
JS_ATOM_return,
JS_ATOM_var,
JS_ATOM_this,
JS_ATOM_delete,
JS_ATOM_void,
JS_ATOM_typeof,
JS_ATOM_new,
JS_ATOM_in,
JS_ATOM_instanceof,
JS_ATOM_do,
JS_ATOM_while,
JS_ATOM_for,
JS_ATOM_break,
JS_ATOM_continue,
JS_ATOM_switch,
JS_ATOM_case,
JS_ATOM_default,
JS_ATOM_throw,
JS_ATOM_try,
JS_ATOM_catch,
JS_ATOM_finally,
JS_ATOM_function,
JS_ATOM_debugger,
JS_ATOM_with,
JS_ATOM___file__,
JS_ATOM___dir__,
JS_ATOM_class,
JS_ATOM_const,
JS_ATOM_enum,
JS_ATOM_export,
JS_ATOM_extends,
JS_ATOM_import,
JS_ATOM_super,
JS_ATOM_implements,
JS_ATOM_interface,
JS_ATOM_let,
JS_ATOM_package,
JS_ATOM_private,
JS_ATOM_protected,
JS_ATOM_public,
JS_ATOM_static,
JS_ATOM_yield,
JS_ATOM_await,
JS_ATOM_empty_string,
JS_ATOM_length,
JS_ATOM_fileName,
JS_ATOM_lineNumber,
JS_ATOM_message,
JS_ATOM_errors,
JS_ATOM_stack,
JS_ATOM_name,
JS_ATOM_toString,
JS_ATOM_toLocaleString,
JS_ATOM_valueOf,
JS_ATOM_eval,
JS_ATOM_prototype,
JS_ATOM_constructor,
JS_ATOM_configurable,
JS_ATOM_writable,
JS_ATOM_enumerable,
JS_ATOM_value,
JS_ATOM_get,
JS_ATOM_set,
JS_ATOM_of,
JS_ATOM___proto__,
JS_ATOM_undefined,
JS_ATOM_number,
JS_ATOM_boolean,
JS_ATOM_string,
JS_ATOM_object,
JS_ATOM_symbol,
JS_ATOM_integer,
JS_ATOM_unknown,
JS_ATOM_arguments,
JS_ATOM_callee,
JS_ATOM_caller,
JS_ATOM__eval_,
JS_ATOM__ret_,
JS_ATOM__var_,
JS_ATOM__arg_var_,
JS_ATOM__with_,
JS_ATOM_lastIndex,
JS_ATOM_target,
JS_ATOM_index,
JS_ATOM_input,
JS_ATOM_defineProperties,
JS_ATOM_apply,
JS_ATOM_join,
JS_ATOM_concat,
JS_ATOM_split,
JS_ATOM_construct,
JS_ATOM_getPrototypeOf,
JS_ATOM_setPrototypeOf,
JS_ATOM_isExtensible,
JS_ATOM_preventExtensions,
JS_ATOM_has,
JS_ATOM_deleteProperty,
JS_ATOM_defineProperty,
JS_ATOM_getOwnPropertyDescriptor,
JS_ATOM_ownKeys,
JS_ATOM_add,
JS_ATOM_done,
JS_ATOM_next,
JS_ATOM_values,
JS_ATOM_source,
JS_ATOM_flags,
JS_ATOM_global,
JS_ATOM_unicode,
JS_ATOM_raw,
JS_ATOM_new_target,
JS_ATOM_this_active_func,
JS_ATOM_home_object,
JS_ATOM_computed_field,
JS_ATOM_static_computed_field,
JS_ATOM_class_fields_init,
JS_ATOM_brand,
JS_ATOM_hash_constructor,
JS_ATOM_as,
JS_ATOM_from,
JS_ATOM_meta,
JS_ATOM__default_,
JS_ATOM__star_,
JS_ATOM_Module,
JS_ATOM_then,
JS_ATOM_resolve,
JS_ATOM_reject,
JS_ATOM_promise,
JS_ATOM_proxy,
JS_ATOM_revoke,
JS_ATOM_async,
JS_ATOM_exec,
JS_ATOM_groups,
JS_ATOM_status,
JS_ATOM_reason,
JS_ATOM_globalThis,
JS_ATOM_bigint,
JS_ATOM_bigfloat,
JS_ATOM_bigdecimal,
JS_ATOM_roundingMode,
JS_ATOM_maximumSignificantDigits,
JS_ATOM_maximumFractionDigits,
JS_ATOM_toJSON,
JS_ATOM_Object,
JS_ATOM_Array,
JS_ATOM_Error,
JS_ATOM_Number,
JS_ATOM_String,
JS_ATOM_Boolean,
JS_ATOM_Symbol,
JS_ATOM_Arguments,
JS_ATOM_Math,
JS_ATOM_JSON,
JS_ATOM_Date,
JS_ATOM_Function,
JS_ATOM_GeneratorFunction,
JS_ATOM_ForInIterator,
JS_ATOM_RegExp,
JS_ATOM_ArrayBuffer,
JS_ATOM_SharedArrayBuffer,
JS_ATOM_Uint8ClampedArray,
JS_ATOM_Int8Array,
JS_ATOM_Uint8Array,
JS_ATOM_Int16Array,
JS_ATOM_Uint16Array,
JS_ATOM_Int32Array,
JS_ATOM_Uint32Array,
JS_ATOM_BigInt64Array,
JS_ATOM_BigUint64Array,
JS_ATOM_Float32Array,
JS_ATOM_Float64Array,
JS_ATOM_DataView,
JS_ATOM_BigInt,
JS_ATOM_BigFloat,
JS_ATOM_BigFloatEnv,
JS_ATOM_BigDecimal,
JS_ATOM_OperatorSet,
JS_ATOM_Operators,
JS_ATOM_JSX,
JS_ATOM_Map,
JS_ATOM_Set,
JS_ATOM_WeakMap,
JS_ATOM_WeakSet,
JS_ATOM_Map_Iterator,
JS_ATOM_Set_Iterator,
JS_ATOM_Array_Iterator,
JS_ATOM_String_Iterator,
JS_ATOM_RegExp_String_Iterator,
JS_ATOM_Generator,
JS_ATOM_Proxy,
JS_ATOM_Promise,
JS_ATOM_PromiseResolveFunction,
JS_ATOM_PromiseRejectFunction,
JS_ATOM_AsyncFunction,
JS_ATOM_AsyncFunctionResolve,
JS_ATOM_AsyncFunctionReject,
JS_ATOM_AsyncGeneratorFunction,
JS_ATOM_AsyncGenerator,
JS_ATOM_EvalError,
JS_ATOM_RangeError,
JS_ATOM_ReferenceError,
JS_ATOM_SyntaxError,
JS_ATOM_TypeError,
JS_ATOM_URIError,
JS_ATOM_InternalError,
JS_ATOM_Private_brand,
JS_ATOM_Symbol_toPrimitive,
JS_ATOM_Symbol_iterator,
JS_ATOM_Symbol_match,
JS_ATOM_Symbol_matchAll,
JS_ATOM_Symbol_replace,
JS_ATOM_Symbol_search,
JS_ATOM_Symbol_split,
JS_ATOM_Symbol_toStringTag,
JS_ATOM_Symbol_isConcatSpreadable,
JS_ATOM_Symbol_hasInstance,
JS_ATOM_Symbol_species,
JS_ATOM_Symbol_unscopables,
JS_ATOM_Symbol_asyncIterator,
JS_ATOM_Symbol_operatorSet,
JS_ATOM_Symbol_tag,
    JS_ATOM_END,
};
static const char js_atom_init[] =
"null" "\0"
"false" "\0"
"true" "\0"
"if" "\0"
"else" "\0"
"return" "\0"
"var" "\0"
"this" "\0"
"delete" "\0"
"void" "\0"
"typeof" "\0"
"new" "\0"
"in" "\0"
"instanceof" "\0"
"do" "\0"
"while" "\0"
"for" "\0"
"break" "\0"
"continue" "\0"
"switch" "\0"
"case" "\0"
"default" "\0"
"throw" "\0"
"try" "\0"
"catch" "\0"
"finally" "\0"
"function" "\0"
"debugger" "\0"
"with" "\0"
"__FILE__" "\0"
"__DIR__" "\0"
"class" "\0"
"const" "\0"
"enum" "\0"
"export" "\0"
"extends" "\0"
"import" "\0"
"super" "\0"
"implements" "\0"
"interface" "\0"
"let" "\0"
"package" "\0"
"private" "\0"
"protected" "\0"
"public" "\0"
"static" "\0"
"yield" "\0"
"await" "\0"
"" "\0"
"length" "\0"
"fileName" "\0"
"lineNumber" "\0"
"message" "\0"
"errors" "\0"
"stack" "\0"
"name" "\0"
"toString" "\0"
"toLocaleString" "\0"
"valueOf" "\0"
"eval" "\0"
"prototype" "\0"
"constructor" "\0"
"configurable" "\0"
"writable" "\0"
"enumerable" "\0"
"value" "\0"
"get" "\0"
"set" "\0"
"of" "\0"
"__proto__" "\0"
"undefined" "\0"
"number" "\0"
"boolean" "\0"
"string" "\0"
"object" "\0"
"symbol" "\0"
"integer" "\0"
"unknown" "\0"
"arguments" "\0"
"callee" "\0"
"caller" "\0"
"<eval>" "\0"
"<ret>" "\0"
"<var>" "\0"
"<arg_var>" "\0"
"<with>" "\0"
"lastIndex" "\0"
"target" "\0"
"index" "\0"
"input" "\0"
"defineProperties" "\0"
"apply" "\0"
"join" "\0"
"concat" "\0"
"split" "\0"
"construct" "\0"
"getPrototypeOf" "\0"
"setPrototypeOf" "\0"
"isExtensible" "\0"
"preventExtensions" "\0"
"has" "\0"
"deleteProperty" "\0"
"defineProperty" "\0"
"getOwnPropertyDescriptor" "\0"
"ownKeys" "\0"
"add" "\0"
"done" "\0"
"next" "\0"
"values" "\0"
"source" "\0"
"flags" "\0"
"global" "\0"
"unicode" "\0"
"raw" "\0"
"new.target" "\0"
"this.active_func" "\0"
"<home_object>" "\0"
"<computed_field>" "\0"
"<static_computed_field>" "\0"
"<class_fields_init>" "\0"
"<brand>" "\0"
"#constructor" "\0"
"as" "\0"
"from" "\0"
"meta" "\0"
"*default*" "\0"
"*" "\0"
"Module" "\0"
"then" "\0"
"resolve" "\0"
"reject" "\0"
"promise" "\0"
"proxy" "\0"
"revoke" "\0"
"async" "\0"
"exec" "\0"
"groups" "\0"
"status" "\0"
"reason" "\0"
"globalThis" "\0"
"bigint" "\0"
"bigfloat" "\0"
"bigdecimal" "\0"
"roundingMode" "\0"
"maximumSignificantDigits" "\0"
"maximumFractionDigits" "\0"
"toJSON" "\0"
"Object" "\0"
"Array" "\0"
"Error" "\0"
"Number" "\0"
"String" "\0"
"Boolean" "\0"
"Symbol" "\0"
"Arguments" "\0"
"Math" "\0"
"JSON" "\0"
"Date" "\0"
"Function" "\0"
"GeneratorFunction" "\0"
"ForInIterator" "\0"
"RegExp" "\0"
"ArrayBuffer" "\0"
"SharedArrayBuffer" "\0"
"Uint8ClampedArray" "\0"
"Int8Array" "\0"
"Uint8Array" "\0"
"Int16Array" "\0"
"Uint16Array" "\0"
"Int32Array" "\0"
"Uint32Array" "\0"
"BigInt64Array" "\0"
"BigUint64Array" "\0"
"Float32Array" "\0"
"Float64Array" "\0"
"DataView" "\0"
"BigInt" "\0"
"BigFloat" "\0"
"BigFloatEnv" "\0"
"BigDecimal" "\0"
"OperatorSet" "\0"
"Operators" "\0"
"JSX" "\0"
"Map" "\0"
"Set" "\0"
"WeakMap" "\0"
"WeakSet" "\0"
"Map Iterator" "\0"
"Set Iterator" "\0"
"Array Iterator" "\0"
"String Iterator" "\0"
"RegExp String Iterator" "\0"
"Generator" "\0"
"Proxy" "\0"
"Promise" "\0"
"PromiseResolveFunction" "\0"
"PromiseRejectFunction" "\0"
"AsyncFunction" "\0"
"AsyncFunctionResolve" "\0"
"AsyncFunctionReject" "\0"
"AsyncGeneratorFunction" "\0"
"AsyncGenerator" "\0"
"EvalError" "\0"
"RangeError" "\0"
"ReferenceError" "\0"
"SyntaxError" "\0"
"TypeError" "\0"
"URIError" "\0"
"InternalError" "\0"
"<brand>" "\0"
"Symbol.toPrimitive" "\0"
"Symbol.iterator" "\0"
"Symbol.match" "\0"
"Symbol.matchAll" "\0"
"Symbol.replace" "\0"
"Symbol.search" "\0"
"Symbol.split" "\0"
"Symbol.toStringTag" "\0"
"Symbol.isConcatSpreadable" "\0"
"Symbol.hasInstance" "\0"
"Symbol.species" "\0"
"Symbol.unscopables" "\0"
"Symbol.asyncIterator" "\0"
"Symbol.operatorSet" "\0"
"Symbol.tag" "\0"
;
typedef enum OPCodeFormat {
OP_FMT_none,
OP_FMT_none_int,
OP_FMT_none_loc,
OP_FMT_none_arg,
OP_FMT_none_var_ref,
OP_FMT_u8,
OP_FMT_i8,
OP_FMT_loc8,
OP_FMT_const8,
OP_FMT_label8,
OP_FMT_u16,
OP_FMT_i16,
OP_FMT_label16,
OP_FMT_npop,
OP_FMT_npopx,
OP_FMT_npop_u16,
OP_FMT_loc,
OP_FMT_arg,
OP_FMT_var_ref,
OP_FMT_u32,
OP_FMT_i32,
OP_FMT_const,
OP_FMT_label,
OP_FMT_atom,
OP_FMT_atom_u8,
OP_FMT_atom_u16,
OP_FMT_atom_label_u8,
OP_FMT_atom_label_u16,
OP_FMT_label_u16,
} OPCodeFormat;
enum OPCodeEnum {
OP_invalid,
OP_push_i32,
OP_push_const,
OP_fclosure,
OP_push_atom_value,
OP_private_symbol,
OP_undefined,
OP_null,
OP_push_this,
OP_push_false,
OP_push_true,
OP_object,
OP_special_object,
OP_rest,
OP_drop,
OP_nip,
OP_nip1,
OP_dup,
OP_dup1,
OP_dup2,
OP_dup3,
OP_insert2,
OP_insert3,
OP_insert4,
OP_perm3,
OP_perm4,
OP_perm5,
OP_swap,
OP_swap2,
OP_rot3l,
OP_rot3r,
OP_rot4l,
OP_rot5l,
OP_call_constructor,
OP_call,
OP_tail_call,
OP_call_method,
OP_tail_call_method,
OP_array_from,
OP_apply,
OP_return,
OP_return_undef,
OP_check_ctor_return,
OP_check_ctor,
OP_check_brand,
OP_add_brand,
OP_return_async,
OP_throw,
OP_throw_error,
OP_eval,
OP_apply_eval,
OP_regexp,
OP_get_super,
OP_import,
OP_check_var,
OP_get_var_undef,
OP_get_var,
OP_put_var,
OP_put_var_init,
OP_put_var_strict,
OP_get_ref_value,
OP_put_ref_value,
OP_define_var,
OP_check_define_var,
OP_define_func,
OP_get_field,
OP_get_field2,
OP_put_field,
OP_get_private_field,
OP_put_private_field,
OP_define_private_field,
OP_get_array_el,
OP_get_array_el2,
OP_put_array_el,
OP_get_super_value,
OP_put_super_value,
OP_define_field,
OP_set_name,
OP_set_name_computed,
OP_set_proto,
OP_set_home_object,
OP_define_array_el,
OP_append,
OP_copy_data_properties,
OP_define_method,
OP_define_method_computed,
OP_define_class,
OP_define_class_computed,
OP_get_loc,
OP_put_loc,
OP_set_loc,
OP_get_arg,
OP_put_arg,
OP_set_arg,
OP_get_var_ref,
OP_put_var_ref,
OP_set_var_ref,
OP_set_loc_uninitialized,
OP_get_loc_check,
OP_put_loc_check,
OP_put_loc_check_init,
OP_get_var_ref_check,
OP_put_var_ref_check,
OP_put_var_ref_check_init,
OP_close_loc,
OP_if_false,
OP_if_true,
OP_goto,
OP_catch,
OP_gosub,
OP_ret,
OP_to_object,
OP_to_propkey,
OP_to_propkey2,
OP_with_get_var,
OP_with_put_var,
OP_with_delete_var,
OP_with_make_ref,
OP_with_get_ref,
OP_with_get_ref_undef,
OP_make_loc_ref,
OP_make_arg_ref,
OP_make_var_ref_ref,
OP_make_var_ref,
OP_for_in_start,
OP_for_of_start,
OP_for_await_of_start,
OP_for_in_next,
OP_for_of_next,
OP_iterator_check_object,
OP_iterator_get_value_done,
OP_iterator_close,
OP_iterator_close_return,
OP_iterator_next,
OP_iterator_call,
OP_initial_yield,
OP_yield,
OP_yield_star,
OP_async_yield_star,
OP_await,
OP_neg,
OP_plus,
OP_dec,
OP_inc,
OP_post_dec,
OP_post_inc,
OP_dec_loc,
OP_inc_loc,
OP_add_loc,
OP_not,
OP_lnot,
OP_typeof,
OP_delete,
OP_delete_var,
OP_mul,
OP_div,
OP_mod,
OP_add,
OP_sub,
OP_pow,
OP_shl,
OP_sar,
OP_shr,
OP_lt,
OP_lte,
OP_gt,
OP_gte,
OP_instanceof,
OP_in,
OP_eq,
OP_neq,
OP_strict_eq,
OP_strict_neq,
OP_and,
OP_xor,
OP_or,
OP_is_undefined_or_null,
OP_mul_pow10,
OP_math_mod,
OP_nop,
OP_push_minus1,
OP_push_0,
OP_push_1,
OP_push_2,
OP_push_3,
OP_push_4,
OP_push_5,
OP_push_6,
OP_push_7,
OP_push_i8,
OP_push_i16,
OP_push_const8,
OP_fclosure8,
OP_push_empty_string,
OP_get_loc8,
OP_put_loc8,
OP_set_loc8,
OP_get_loc0,
OP_get_loc1,
OP_get_loc2,
OP_get_loc3,
OP_put_loc0,
OP_put_loc1,
OP_put_loc2,
OP_put_loc3,
OP_set_loc0,
OP_set_loc1,
OP_set_loc2,
OP_set_loc3,
OP_get_arg0,
OP_get_arg1,
OP_get_arg2,
OP_get_arg3,
OP_put_arg0,
OP_put_arg1,
OP_put_arg2,
OP_put_arg3,
OP_set_arg0,
OP_set_arg1,
OP_set_arg2,
OP_set_arg3,
OP_get_var_ref0,
OP_get_var_ref1,
OP_get_var_ref2,
OP_get_var_ref3,
OP_put_var_ref0,
OP_put_var_ref1,
OP_put_var_ref2,
OP_put_var_ref3,
OP_set_var_ref0,
OP_set_var_ref1,
OP_set_var_ref2,
OP_set_var_ref3,
OP_get_length,
OP_if_false8,
OP_if_true8,
OP_goto8,
OP_goto16,
OP_call0,
OP_call1,
OP_call2,
OP_call3,
OP_is_undefined,
OP_is_null,
OP_typeof_is_undefined,
OP_typeof_is_function,
    OP_COUNT,
    OP_TEMP_START = OP_nop + 1,
    OP___dummy = OP_TEMP_START - 1,
OP_enter_scope,
OP_leave_scope,
OP_label,
OP_scope_get_var_undef,
OP_scope_get_var,
OP_scope_put_var,
OP_scope_delete_var,
OP_scope_make_ref,
OP_scope_get_ref,
OP_scope_put_var_init,
OP_scope_get_private_field,
OP_scope_get_private_field2,
OP_scope_put_private_field,
OP_set_class_name,
OP_line_num,
    OP_TEMP_END,
};
