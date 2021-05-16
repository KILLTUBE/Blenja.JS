console.clear()
console.log(1)
console.log(Math.random())




try {
    //console.log(include)
    
    var dir = exedir() + "/src_duktape";
    // If this file exists, use the files from the repo directly.
    // No need to mess around with copying files back and forth.
    var dirgit = file_get_contents(exedir() + "/src_duktape.txt");
    if (dirgit) {
        dir = dirgit;
    }
    console.log("Dir", dir);
    
    var ts = file_get_contents(dir + "/TypeSpirit.js");
    
    tslines = ts.split("\r\n");
    //lines.push.apply(lines, tslines);
    eval(ts)
    console.log("TypeSpirit", TypeSpirit)
} catch (e) {
    console.log("Exception", e);
}
//try 