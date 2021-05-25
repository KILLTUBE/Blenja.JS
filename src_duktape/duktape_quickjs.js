/**
 * @example
 * quickjs = new QuickJS();
 * console.log("quickjs", quickjs);
 * 
 * array = quickjs.eval("[5,6,7,8,'and a string']")
 * console.log(array);
 * 
 * o = quickjs.eval("var o = {f:1, s:2}; o;")
 * console.log("o is", JSON.stringify(o));
 * @summary
 * KungFooBlender.JS embeds Duktape and QuickJS, and Duktape exposes this API to create QuickJS instances
 */

function QuickJS() {
    this.runtime = JS_NewRuntime();
    this.context = JS_NewContext(this.runtime);
}

QuickJS.prototype.eval = function(code) {
    return JS_Eval(this.context, code);
}

QuickJS.prototype.toString = function() {
    return "Runtime=" + this.runtime + " Context=" + this.context;
}
