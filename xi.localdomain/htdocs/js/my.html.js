/********************
 *
 * Author:      Des Jones
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.00
 *
 *****************************************/

var my = my || {};
my.html = {};
my.html.datatables = { "sorttables": {}, "scrolltables": {} };

/* 
 * apply markup and formatting to a new window
 */
my.html.windowInit = function(elementNode) {
    
    /* jquery data table with pagination and responsive sizing */
    var sorttables = jQuery(".sorttable");
    for (var i = 0; i < sorttables.length; i++) {
        my.html.datatables.sorttables.i = { "responsiveHelper": null, "node": sorttables[i] };
        jQuery(sorttables[i]).dataTable({
            "sDom": "<'hidden-xs'CT><'dt-toolbar'<'col-xs-12 col-sm-6'f><'col-sm-6 col-xs-12 hidden-xs'l>r>"+
                    "t"+
                    "<'dt-toolbar-footer'<'col-sm-6 col-xs-12 hidden-xs'i><'col-xs-12 col-sm-6'p>>",
            "oLanguage": { "sSearch": '&nbsp;&nbsp;<i class="glyphicon glyphicon-search"></i>' },
            "lengthMenu": [ 20, 50, 75, 100, 250 ],
            "tableTools": { "sSwfPath": "/images/copy_csv_xls_pdf.swf" },
            "preDrawCallback" : function() {
                // Initialize the responsive datatables helper once.
                if (!my.html.datatables.sorttables.i.responsiveHelper) {
                    my.html.datatables.sorttables.i.responsiveHelper = 
                        new ResponsiveDatatablesHelper(jQuery(my.html.datatables.sorttables.i.node), { tablet: 1024, phone: 480 });
                }
            },
            "rowCallback" : function(nRow) {
                    my.html.datatables.sorttables.i.responsiveHelper.createExpandIcon(nRow);
            },
            "drawCallback" : function(oSettings) {
                    my.html.datatables.sorttables.i.responsiveHelper.respond();
            }
        });
    }
    
    /* jquery data table with no pagination, but scrollable */
    var scrolltables = jQuery(".scrolltable");
    for (var i = 0; i < scrolltables.length; i++) {
        my.html.datatables.scrolltables.i = { "responsiveHelper": null, "node": scrolltables[i] };
        jQuery(".scrolltable").dataTable({ 
            "sDom": "R<'dt-toolbar'<'col-xs-12 col-sm-6'f><'hidden-xs'C>T<'col-sm-6 col-xs-12 hidden-xs'l>r>"+
                    "t"+
                    "<'dt-toolbar-footer'<'col-sm-6 col-xs-12 hidden-xs'i><'col-xs-12 col-sm-6'p>>",
            "oLanguage": { "sSearch": '&nbsp;&nbsp;<i class="glyphicon glyphicon-search"></i>' },
            "bPaginate": false,
            "sScrollY": "600",
            "tableTools": { "sSwfPath": "/images/copy_csv_xls_pdf.swf" },
            "preDrawCallback" : function() {
                // Initialize the responsive datatables helper once.
                if (!my.html.datatables.scrolltables.i.responsiveHelper) {
                    my.html.datatables.scrolltables.i.responsiveHelper = 
                        new ResponsiveDatatablesHelper(jQuery(my.html.datatables.scrolltables.i.node), { tablet: 1024, phone: 480 });
                }
            },
            "rowCallback" : function(nRow) {
                    my.html.datatables.scrolltables.i.responsiveHelper.createExpandIcon(nRow);
            },
            "drawCallback" : function(oSettings) {
                    my.html.datatables.scrolltables.i.responsiveHelper.respond();
            }
        });
    }

    /* jquery data table with fixed columns */
    var fixedtables = jQuery(".fixedtable").dataTable({ 
        "sDom": "R<'dt-toolbar'<'col-xs-12 col-sm-6'f>T<'col-sm-6 col-xs-12 hidden-xs'l>r>"+
                "t"+
                "<'dt-toolbar-footer'<'col-sm-6 col-xs-12 hidden-xs'i><'col-xs-12 col-sm-6'p>>",
        "oLanguage": { "sSearch": '&nbsp;&nbsp;<i class="glyphicon glyphicon-search"></i>' },
        "bPaginate": false,
        "sScrollY": "600",
        "sScrollX": true,
        "tableTools": { "sSwfPath": "/images/copy_csv_xls_pdf.swf" }
    });
    for (var i = 0; i < fixedtables.length; i++) {
        new jQuery.fn.dataTable.FixedColumns(fixedtables[i]);
    }

    /* jquery date picker */
    jQuery(".date").datepicker({ dateFormat: 'yy-mm-dd', changeMonth: true, changeYear: true});
    jQuery(".dateofbirth").datepicker({ dateFormat: 'yy-mm-dd', changeMonth: true, changeYear: true, yearRange: '-100Y:-17Y'});

    /* clock picker */
    jQuery(".clockpicker").clockpicker({ autoclose: true });

    tinymce.init({
        menubar: false,
        plugins: 'charmap print preview code save wordcount',
        toolbar: "undo redo | styleselect | bullist numlist outdent indent | charmap preview code",
        selector: "textarea.tinymce",
    });
    
    /* jquery accordion */
    //     jQuery(".jaccordion").accordion({ collapsible: true, heightStyle: "content" });
    
    /* smart admin page setup */
    if (window.pageSetUp) {
        window.pageSetUp();
    }
    
    return elementNode;
}

/*
 * innerHTML is all that's required.
 */
my.html.insert = function(innerHTML, myParentNode, boolAppend, objectAttributes, HTMLTag) {
    var innerHTML = innerHTML || '';
    var HTMLTag = HTMLTag || 'div';
    var objectAttributes = objectAttributes || {};
    var myParentNode = myParentNode || document.body;
    var boolAppend = boolAppend || false;
    var newNode = null;
    var id = null;

    newNode = document.createElement(HTMLTag);
    newNode.innerHTML = innerHTML;
    for (var property in objectAttributes) {
        if (objectAttributes.hasOwnProperty(property)) {
            newNode.setAttribute(property, objectAttributes[property]);
            if (property.toLowerCase() == "id") {
                id = objectAttributes[property];
            }
        }
    }
    /* assign a unique id if one isn't provided */
    if (!id) {
        id = my.uniqueId();
    }
    newNode.setAttribute('id', id);
    /* insert node at top or append to bottom */
    if (boolAppend) {
        myParentNode.appendChild(newNode);
    } else {
        myParentNode.insertBefore(newNode, myParentNode.firstChild);
    }

    return newNode;
}

my.html.window = function(elementNode, title, positionNode) {
    var options = { title: title };
    if (positionNode) {
        options = {
            title: title,
            position: {
                my: "center bottom",
                at: "center top",
                collision: "flipfit flip",
                of: positionNode
            }
        }
    }
    jQuery(elementNode).dialog(options);
    
    return elementNode;
}

my.html.wait = function(innerHTML) {
    var defaultHTML = '<p>Processing your request...</p>';

    node = document.getElementById('myhtmlwait');
    if (node) {
        if (innerHTML) {
            node.innerHTML = innerHTML;
        }
        jQuery(node).dialog('open');
    } else {
        if (innerHTML) {
            node = my.html.insert(innerHTML);
        } else {
            node = my.html.insert(defaultHTML);
        }
        jQuery(node).dialog({
            autoOpen: true,
            draggable: false,
            resizable: false,
            dialogClass: 'tooltip',
            modal: true
        });
    }

    return true;
}

/*
 * limit input characters, example below
 * <span id="mycounter"></span><textarea onkeypress="my.html.characterLimit(this, document.getElementById('mycounter'), 300)"></textarea>
 */
my.html.characterLimit = function(elementNode, counterNode, maxLength) {
    if (!elementNode || !counterNode || !maxLength) {
        return false;
    }
    var lengthLeft = maxLength - elementNode.value.length;
    counterNode.innerHTML = lengthLeft;

    if (lengthLeft < 1){
        elementNode.value = elementNode.value.substring(0, maxLength - 1);
    }

    return elementNode;
}

/*
 * limit input characters, example below
 * <span id="mycounter"></span><textarea onkeypress="my.html.lineLimit(this, document.getElementById('mycounter'), 90, 10)"></textarea>
 */
my.html.lineLimit = function(elementNode, counterNode, lineLength, maxLines) {
    if (!elementNode || !counterNode || !lineLength || !maxLines) {
        return false;
    }
    var i = 0, j = 0;
    var line;
    var lineWrap;
    var linesLeft = maxLines + 1;
    var lines = elementNode.value.split('\n');

    for (i = 0; i < lines.length; i++) {
        if (lines[i].length) {
            lineWrap = Math.ceil(lines[i].length / lineLength);
        } else {
            lineWrap = 1;
        }
        if (linesLeft - lineWrap < 0) {
            linesLeft = 0;
            j += linesLeft * lineLength + 1;
        } else {
            linesLeft -= lineWrap;
            j += lines[i].length + 1;
        }
        if (linesLeft < 1) {
            elementNode.value = elementNode.value.substring(0, j - 2);
            break;
        }
    }
    counterNode.innerHTML =  linesLeft ? linesLeft - 1 : 0;

    return elementNode;
}

/* click all clickable form elements with a given class name */
my.html.clickForm = function(formName, className) {
    if (!document.forms[formName]) {
        my.html.debug('error clickForm: could not find form ' + formName);
        return false;
    }
    var i;
    for (i = 0; i < document.forms[formName].elements.length; i++) {
        if (
            (document.forms[formName].elements[i].className.match(new RegExp("(?:^|\\s)" + className + "(?!\\S)"))) &&
            (document.forms[formName].elements[i].hasAttribute('onclick')) &&
            (document.forms[formName].elements[i].checked == true)
        ) {
            try {
                document.forms[formName].elements[i].onclick.apply(document.forms[formName].elements[i]);
            } catch (e) {
                my.html.debug('error clickForm: could not click ' + document.forms[formName].elements[i]["name"]);
            }
        }
    }

    return true;
}

/*
 * objectPostValues override form values
 * only myParentNode is required
 */
my.html.getFormValues = function(myParentNode, objectPostValues) {
    if (!myParentNode) {
        return {};
    }
    var objectPostValues = objectPostValues || {};
    var i = null;
    var j = null;
    var elements = null;
    var tags = ["input", "select", "textarea", "checkbox", "radio", "hidden"];
    for (i = 0; i < tags.length; i++) {
        elements = myParentNode.getElementsByTagName(tags[i]);
        for (j = 0; j < elements.length; j++) {
            /* objectPostValues override form values */
            if (!objectPostValues[elements[j]["name"]]) {
                switch (elements[j].type.toLowerCase()) {
                    case "checkbox":
                    case "radio":
                        if (elements[j].checked) {
                            objectPostValues[elements[j]["name"]] = elements[j]["value"];
                        }
                        break;
                    default:
                        objectPostValues[elements[j]["name"]] = elements[j]["value"];
                        break;
                }
            }
        }
    }
    
    return objectPostValues;
}