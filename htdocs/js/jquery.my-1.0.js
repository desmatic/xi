/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.00
 *
 *****************************************/

(function(jQuery) {
jQuery.fn.my = (function(jQuery) {
    var _uniqueId = 10000;
    var _debug = 'jQuery().my.debug()\n';
    var _debugNode = null;

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

        object2UrlString: function(objectPostValues) {
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
        },
        
        /*
         * objectPostValues, callBackSuccess, callBackFail are not required.
         */
        ajax: function(url, objectPostValues, callBackSuccess, callBackFail) {
            if (!url) {
                return false;
            }
            var objectPostValues = objectPostValues || {};
            
            request = new XMLHttpRequest();
            request.onload = function() {
                if (this.status == 200) {
                    jQuery.my.debug('XMLHttpRequest: ' + url + '?' + jQuery.my.object2UrlString(objectPostValues));
                    if (callBackSuccess) {
                        callBackSuccess();
                    }
                } else {
                    jQuery.my.debug('XMLHttpRequest Failed: ' + url + '?' + jQuery.my.object2UrlString(objectPostValues));
                    if (callBackFail) {
                        callBackFail();
                    } else {
                        alert("oops! something went wrong.");
                    }
                }
            };
            request.open("POST", url, true);
            request.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
            request.send(jQuery.my.object2UrlString(objectPostValues));
                    
            return true;
        },
        
        ajaxJSON: function(url, objectPostValues, callBackSuccess, callBackFail) {
            var jsonCallBack = function {
                if (callBackSuccess) {
                    var json = JSON.parse(myrequest.responseText) || {};
                    callBackSuccess(json);
                }
            }
            
            return jQuery.my.ajax(url, objectPostValues, jsonCallBack, callBackFail);
        }

    };
    
})(jQuery);
}(jQuery));
