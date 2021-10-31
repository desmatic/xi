/********************
 *
 * Author:      Des Jones
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.00
 *
 *****************************************/

var my = (function() {
    var _uniqueId = 10000;
    var _debug = 'my.debug()\n';
    var _debugNode = '';
    var _elementTimer = {};
    var _mceRawHTML = false;

    return {
        uniqueId: function(prefix) {
            var prefix = prefix || 'uniqueElementId';
            _uniqueId++;
            return prefix + _uniqueId;
        },
        debug: function(message) {
            if (window.console) {
                console.log(message);
            }
            if (_debugNode) {
                _debugNode.innerHTML += message + '\n';
            } else {
                _debug += message + '\n';
            }
            return true;
        },
        debugInitNode: function(debugNode) {
            _debugNode = debugNode || null;
            if (_debugNode) {
                _debugNode.innerHTML = _debug;
                _debug = '';
            }
        },
        setElementTimer: function(elementId, myFunction, milliseconds) {
            var milliseconds = milliseconds || null;
            if (_elementTimer[elementId]) {
                window.clearTimeout(_elementTimer[elementId]);
            }
            if (milliseconds) {
                _elementTimer[elementId] = window.setTimeout(myFunction, milliseconds);
            }
        },
        consolelog: function(message) {
            if (window.console) {
                console.log(message);
            }
        },
        /* repeatString() returns a string which has been repeated a set number of times */ 
        repeatString: function(str, num) {
            out = '';
            for (var i = 0; i < num; i++) {
                out += str; 
            }
            return out;
        },
        /*
         *  dump() displays the contents of a variable like var_dump() does in PHP. dump() is
         *  better than typeof, because it can distinguish between array, null and object.  
         *  Parameters:
         *  v:              The variable
         *  howDisplay:     "none", "body", "alert" (default)
         *  recursionLevel: Number of times the function has recursed when entering nested
         *                  objects or arrays. Each level of recursion adds extra space to the 
         *                  output to indicate level. Set to 0 by default.
         *  Return Value:
         *  A string of the variable's contents 
         *  Limitations:
         *  Can't pass an undefined variable to dump(). 
         *  dump() can't distinguish between int and float.
         *  dump() can't tell the original variable type of a member variable of an object.
         *  These limitations can't be fixed because these are *features* of JS. However, dump()
         */
        dump: function(v, howDisplay, recursionLevel) {
            howDisplay = (typeof howDisplay === 'undefined') ? "alert" : howDisplay;
            recursionLevel = (typeof recursionLevel !== 'number') ? 0 : recursionLevel;


            var vType = typeof v;
            var out = vType;

            switch (vType) {
                case "number":
                    /* there is absolutely no way in JS to distinguish 2 from 2.0
                    so 'number' is the best that you can do. The following doesn't work:
                    var er = /^[0-9]+$/;
                    if (!isNaN(v) && v % 1 === 0 && er.test(3.0))
                        out = 'int';*/
                case "boolean":
                    out += ": " + v;
                    break;
                case "string":
                    out += "(" + v.length + '): "' + v + '"';
                    break;
                case "object":
                    //check if null
                    if (v === null) {
                        out = "null";

                    }
                    //If using jQuery: if ($.isArray(v))
                    //If using IE: if (isArray(v))
                    //this should work for all browsers according to the ECMAScript standard:
                    else if (Object.prototype.toString.call(v) === '[object Array]') {  
                        out = 'array(' + v.length + '): {\n';
                        for (var i = 0; i < v.length; i++) {
                            out += my.repeatString('   ', recursionLevel) + "   [" + i + "]:  " + 
                                my.dump(v[i], "none", recursionLevel + 1) + "\n";
                        }
                        out += my.repeatString('   ', recursionLevel) + "}";
                    }
                    else { //if object    
                        sContents = "{\n";
                        cnt = 0;
                        for (var member in v) {
                            //No way to know the original data type of member, since JS
                            //always converts it to a string and no other way to parse objects.
                            sContents += my.repeatString('   ', recursionLevel) + "   " + member +
                                ":  " + my.dump(v[member], "none", recursionLevel + 1) + "\n";
                            cnt++;
                        }
                        sContents += my.repeatString('   ', recursionLevel) + "}";
                        out += "(" + cnt + "): " + sContents;
                    }
                    break;
            }

            if (howDisplay == 'body') {
                var pre = document.createElement('pre');
                pre.innerHTML = out;
                document.body.appendChild(pre)
            }
            else if (howDisplay == 'alert') {
                alert(out);
            }

            return out;
        }
    }
})();