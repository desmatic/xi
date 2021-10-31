/********************
 *
 * Author:      Des Jones
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.00
 *
 *****************************************/

var my = my || {};
my.is = {};

my.is.type = function(input) {
    return Object.prototype.toString.apply(input);
}

my.is.array = function(input) {
    input = input || '';

    if (Object.prototype.toString.apply(input) === '[object Array]') {
        return true;
    }

    return false;
}

my.is.decimal = function(input) {
    input = input || '';

    var i = 0;
    var c = '';
    var foundNumber = false;
    var hasDecimal = false;
    var output = '';

    input = input.trim();
    for(i = 0; i < input.length; i++) {
        c = input.substr(i, 1);
        if (c == ".") {
            if (hasDecimal) {
                return false;
            } else {
                output += c;
                hasDecimal = true;
            }
        } else if ((c >= "0") && (c <= "9")) {
            output += c;
            foundNumber = true;
        } else if (c == ",") {
            // do nothing
        } else {
            return false;
        }
    }
    if (!foundNumber) {
        return false;
    }

    return true;
}

my.is.money = function(input) {
    input = input || '';

    var i = 0;
    var c = '';

    input = input.trim();
    for(i = 0; i < input.length; i++) {
        c = input.substr(i, 1);
        if (
            ((c >= "0") && (c <= "9")) ||
            (c == "$") || (c == "£") || (c == ",") || (c == ".")
        ) {
            // do nothing
        } else {
            return false;
        }
    }

    return true;
}