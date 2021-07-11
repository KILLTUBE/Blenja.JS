/**
 * @example ```js
 * sum([1, 2, 3]);
 * ```
 * @param {number[]} arr 
 * @returns {number}
 */

function sum(arr) {
  return arr.reduce((acc, cur)=>acc+cur, 0);
}


/**
 * @example ```js
 * console.log(joinBeforeAfter([1,2,3], "  ", ",\n"))
 * ```
 * @param {[]} arr 
 * @param {string} before 
 * @param {string} after 
 * @returns {string}
 */
function joinBeforeAfter(arr, before, after) {
  var ret = '';
  var i;
  var n;
  // #########################
  n = arr.length;
  for (i=0; i<n; i++) {
    ret += before;
    ret += arr[i];
    if (i + 1 < n) {
      ret += after;
    }
  }
  return ret;
}
