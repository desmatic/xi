/********************
 *
 * Author:      Des Jones
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.00
 *
 *****************************************/

var my = my || {};
my.ajax = {};

my.ajax.object2UrlString = function(objectPostValues) {
    var urlString = '';
    for (property in objectPostValues) {
        if (objectPostValues.hasOwnProperty(property)) {
            urlString += encodeURIComponent(property) + '=' + encodeURIComponent(objectPostValues[property]) + '&';
        }
    }
    if (urlString) {
        urlString = urlString.substr(0, urlString.length - 1);    
    }
    
    return urlString;
}

/*
 * objectPostValues, callBackSuccess, callBackFail are not required.
 */
my.ajax.html = function(url, objectPostValues, callBackSuccess, callBackFail) {
    if (!url) {
        return false;
    }
    var objectPostValues = objectPostValues || {};
    
    request = new XMLHttpRequest();
    request.onload = function() {
        if (this.status == 200) {
            my.debug('XMLHttpRequest: ' + url + '?' + my.ajax.object2UrlString(objectPostValues));
            if (callBackSuccess) {
                callBackSuccess(this.responseText);
            }
        } else {
            my.debug('XMLHttpRequest Failed: ' + url + '?' + my.ajax.object2UrlString(objectPostValues));
            if (callBackFail) {
                callBackFail();
            } else {
                alert("oops! something went wrong.");
            }
        }
    };
    request.open("POST", url, true);
    request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    request.send(my.ajax.object2UrlString(objectPostValues));
            
    return true;
},

/*
 * objectPostValues, callBackSuccess, callBackFail are not required.
 */
my.ajax.json = function(url, objectPostValues, callBackSuccess, callBackFail) {
    var jsonCallBack = function(responseText) {
        if (callBackSuccess) {
            callBackSuccess(JSON.parse(responseText) || {});
        }
    }
    
    return my.ajax.html(url, objectPostValues, jsonCallBack, callBackFail);
}

/*
 * objectPostValues, callBackFail are not required.
 */
my.ajax.innerHTML = function(elementNode, url, objectPostValues, callBackFail) {
    my.ajax.html(url, objectPostValues, function(responseText) { elementNode.innerHTML = responseText; my.html.windowInit(elementNode); }, callBackFail);

    return elementNode;
}

/*
 * url is all that's required.
 */
my.ajax.insertHTML = function(url, objectPostValues, callBackFail, myParentNode, boolAppend, objectAttributes, HTMLTag) {
    if (!url) {
        return null;
    }
    var objectPostValues = objectPostValues || {};
    var newNode = my.html.insert('', myParentNode, boolAppend, objectAttributes, HTMLTag);
    my.ajax.html(url, objectPostValues, function(responseText) { newNode.innerHTML = responseText; my.html.windowInit(newNode); }, callBackFail);

    return newNode;
}

/*
 * objectPostValues override form values
 * callBackFail is not required
 */
my.ajax.formInnerHtml = function(url, objectPostValues, myParentNode, innerHtmlNode, callBackFail) {
    if ((!url) || (!myParentNode) || (!innerHtmlNode)) {
        return {};
    }
    objectPostValues = my.html.getFormValues(myParentNode, objectPostValues);
    my.ajax.innerHTML(innerHtmlNode, url, objectPostValues, callBackFail);
   
    return innerHtmlNode;
}

/*
 * objectPostValues override form values
 * callBackFail is not required
 */
my.ajax.formInsertHtml = function(url, objectPostValues, callBackFail, myParentNode, myInsertNode, boolAppend, objectAttributes, HTMLTag) {
    if ((!url) || (!myParentNode) || (!innerHtmlNode)) {
        return {};
    }
    objectPostValues = my.html.getFormValues(myParentNode, objectPostValues);

    return my.ajax.insertHTML(url, objectPostValues, callBackFail, myInsertNode, boolAppend, objectAttributes, HTMLTag);
}

/*
 * objectPostValues override form values
 * callBackFail is not required
 */
my.ajax.formJson = function(url, objectPostValues, myParentNode, jsonFunction, callBackFail) {
    if ((!url) || (!myParentNode) || (!innerHtmlNode)) {
        return {};
    }
    objectPostValues = my.html.getFormValues(myParentNode, objectPostValues);
    my.ajax.json(url, objectPostValues, jsonFunction, callBackFail);
    
    return objectPostValues;
}

/*
 * url is all that's required.
 */
my.ajax.window = function(url, objectPostValues, title, positionNode) {
    var newNode = my.ajax.insertHTML(url, objectPostValues);
    my.html.window(newNode, title, positionNode);
    
    return newNode;
}
