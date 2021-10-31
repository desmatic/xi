/********************
 * 
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

/* add support for formatted numbers */
jQuery.fn.dataTableExt.aTypes.unshift(
    function (sData)
    {
        var sValidChars = "0123456789$£€c,.%-";
        var Char;
        var bDecimal = false;
        
        /* Check the numeric part */
        for ( i=0 ; i<sData.length ; i++ )
        {
            Char = sData.charAt(i);
            if (sValidChars.indexOf(Char) == -1)
            {
                return null;
            }
            
            /* Only allowed one decimal place... */
            if ( Char == "." )
            {
                if ( bDecimal )
                {
                    return null;
                }
                bDecimal = true;
            }
        }
        
        return 'formatted-num';
    }
);

jQuery.fn.dataTableExt.oSort['formatted-num-asc'] = function(a,b) {
    var x = (a == "-") ? 0 : a.replace(/[$£€c,%]/g, "");
    var y = (b == "-") ? 0 : b.replace(/[$£€c,%]/g, "");
    x = parseFloat(x);
    y = parseFloat(y);
    return ((x < y) ? -1 : ((x > y) ?  1 : 0));
};

jQuery.fn.dataTableExt.oSort['formatted-num-desc'] = function(a,b) {
    var x = (a == "-") ? 0 : a.replace(/[$£€c,%]/g, "");
    var y = (b == "-") ? 0 : b.replace(/[$£€c,%]/g, "");
    x = parseFloat(x);
    y = parseFloat(y);
    return ((x < y) ?  1 : ((x > y) ? -1 : 0));
};

/* add support for uk dates */
jQuery.fn.dataTableExt.aTypes.unshift(
    function (sData)
    {
        if (sData !== null && sData.match(/^(0[1-9]|[12][0-9]|3[01])\/(0[1-9]|1[012])\/(19|20|21)\d\d$/) || jQuery.trim(sData) == '')
        {
            return 'date-uk';
        }
        
        return null;
    }
);

jQuery.fn.dataTableExt.oSort['date-uk-pre'] = function(a) {
    var ukDatea = a.split('/');
    if (jQuery.trim(a) == '') {
        return 0;
    }
    
    return (ukDatea[2] + ukDatea[1] + ukDatea[0]) * 1;
};

jQuery.fn.dataTableExt.oSort['date-uk-asc'] = function(a,b) {
    return ((a < b) ? -1 : ((a > b) ? 1 : 0));
};

jQuery.fn.dataTableExt.oSort['date-uk-desc'] = function(a,b) {
    return ((a < b) ? 1 : ((a > b) ? -1 : 0));
};

/* add support for uk datetime */
jQuery.fn.dataTableExt.aTypes.unshift(
    function (sData)
    {
        if (sData !== null && sData.match(/^(0[1-9]|[12][0-9]|3[01])\/(0[1-9]|1[012])\/(19|20|21)\d\d\s([012][0-9])\:([0-5][0-9])\:([0-5][0-9])$/) || jQuery.trim(sData) == '')
        {
            return 'datetime-uk';
        }
        
        return null;
    }
);

jQuery.fn.dataTableExt.oSort['datetime-uk-pre'] = function(a,b) {
    if (jQuery.trim(a) != '') {
        var frDatea = $.trim(a).split(' ');
        var frTimea = frDatea[1].split(':');
        var frDatea2 = frDatea[0].split('/');
        var x = (frDatea2[2] + frDatea2[1] + frDatea2[0] + frTimea[0] + frTimea[1] + frTimea[2]) * 1;
    } else {
        var x = 0;
    }
     
    return x;
};

jQuery.fn.dataTableExt.oSort['datetime-uk-asc'] = function(a,b) {
    return a - b;
};

jQuery.fn.dataTableExt.oSort['datetime-uk-desc'] = function(a,b) {
    return b - a;
};