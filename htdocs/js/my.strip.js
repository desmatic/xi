/********************
 *
 * Author:      Des Jones
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.00
 *
 *****************************************/

var my = my || {};
my.strip = {};

my.strip.integer = function(input) {
    input = input || '';

    var i = 0;
    var c = '';
    var output = '';

    input = input.trim();
    for(i = 0; i < input.length; i++) {
        c = input.substr(i, 1);
        if ((c >= "0") && (c <= "9")) {
            output += c;
        }
    }

    return output;
}

my.strip.decimal = function(input) {
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
                break;
            } else { 
                output += c;
                hasDecimal = true;
            }
        } else if ((c >= "0") && (c <= "9")) {
            output += c;
            foundNumber = true;
        }
    }
    if (!foundNumber) {
        output = '';
    }

    return output;
}

my.strip.negativeDecimal = function(input) {
    input = input || '';

    var i = 0;
    var c = '';
    var foundNumber = false;
    var hasDecimal = false;
    var hasDash = false;
    var output = '';

    input = input.trim();
    for(i = 0; i < input.length; i++) {
        c = input.substr(i, 1);
        if (c == ".") {
            if (hasDecimal) {
                break;
            } else { 
                output += c;
                hasDecimal = true;
            }
        } else if ((c >= "0") && (c <= "9")) {
            output += c;
            foundNumber = true;
        } else if ((!foundNumber) && (c == "-")) {
            if (hasDash) {
                break;
            } else {
                output += c;
                hasDash = true;
            }
        }
    }
    if (!foundNumber) {
        output = '';
    }

    return output;
}

my.strip.alpha = function(input) {
    input = input || '';

    var i = 0;
    var c = '';
    var output = '';

    input = input.trim();
    for(i = 0; i < input.length; i++) {
        c = input.substr(i, 1);
        if (
            ((c >= "a") && (c <= "z")) || 
            ((c >= "A") && (c <= "Z"))
        ) {
            output += c;
        }
    }

    return output;
}

my.strip.alphaNumeric = function(input) {
    input = input || '';
    
    var i = 0;
    var c = '';
    var output = '';

    input = input.trim();
    for(i = 0; i < input.length; i++) {
        c = input.substr(i, 1);
        if (
            ((c >= "a") && (c <= "z")) || 
            ((c >= "A") && (c <= "Z")) ||
            ((c >= "0") && (c <= "9"))
        ) {
            output += c;
        }
    }

    return output;
}

my.strip.money = function(input) {
    input = input || '';

    var i = 0;
    var c = '';
    var output = '';

    input = input.trim();
    for(i = 0; i < input.length; i++) {
        c = input.substr(i, 1);
        if (
            ((c >= "0") && (c <= "9")) ||
            (c == "$") || (c == "£") || (c == ",") || (c == ".")
        ) {
            output += c;
        }
    }

    return output;
}
