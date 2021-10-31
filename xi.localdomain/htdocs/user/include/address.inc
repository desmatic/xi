<script type="text/javascript">

var quickAddress = quickAddress || {};

quickAddress.uniqueId = (function() {
    var _uniqueId = 7665412;

    return {
        next: function(prefix) {
            var prefix = prefix || 0;
            _uniqueId++;
            return prefix + _uniqueId;
        },
        current: function(prefix) {
            var prefix = prefix || 0;
            return prefix + _uniqueId;
        }
    }
})();

quickAddress.addressKeys = function() {

    return <?= address::get_json_form_fields(); ?>;
}

quickAddress.isValidAddress = function() {
    var street = ['addressflatdetails','addresshousename','addresshousenumber','addressstreet'];
    var city = ['addressdistrict','addresscity','addresscounty'];
    var isValidStreet = false;
    var isValidCity = false;
    var isValidAddress = true;
    var node = null;
    var i = 0;
    
    for (i = 0; i < street.length; i++) {
        node = document.getElementById(street[i]);
        if (node.value.trim().length > 2) {
            isValidStreet = true;
        }
    }
    for (i = 0; i < city.length; i++) {
        node = document.getElementById(city[i]);
        if (node.value.trim().length > 2) {
            isValidCity = true;
        }
    }
    node = document.getElementById('addresstimemonths');
    if (node.options[node.selectedIndex].value == '') {
        isValidAddress = false;
    }
    node = document.getElementById('addresstimeyears');
    if (node.options[node.selectedIndex].value == '') {
        isValidAddress = false;
    }
    node = document.getElementById('addresspostcode');
    if (node.value.trim().length < 5) {
        isValidAddress = false;
    }
    if (isValidStreet && isValidCity && isValidAddress) {
        jQuery('#addaddressbutton').removeClass('hidden'); 
        return true;
    } else {
        jQuery('#addaddressbutton').addClass('hidden'); 
    }
    
    return false;
}

quickAddress.clearAddressSearch = function() {
    jQuery('#addaddressform').addClass('hidden');
    jQuery('#searchaddressform').removeClass('hidden');
    document.getElementById('searchaddress').value = '';
    document.getElementById('searchaddress').focus();
    
    return false;
}

quickAddress.clearAddressAdd = function() {
    jQuery('#addaddressform').removeClass('hidden');
    jQuery('#searchaddressform').addClass('hidden');
    var addressKeys = quickAddress.addressKeys();
    for (var i = 0; i < addressKeys.length; i++) {
        for (var name in addressKeys[i]) {
            document.getElementById('address' + name)[addressKeys[i][name]] = '';
        }
    }
    document.getElementById('addressflatdetails').focus();
    quickAddress.isValidAddress();
    
    return false;
}

/* get address from the add address form */
quickAddress.getAddressFromForm = function() {
    if (!quickAddress.isValidAddress()) {
        return null;
    }
    /* add each address field */
    var address = [];
    var addressKeys = quickAddress.addressKeys();
    for (var i = 0; i < addressKeys.length; i++) {
        for (var name in addressKeys[i]) {
            /* if input type select, find the index value */
            if (addressKeys[i][name] == "selectedIndex") {
                address[i] = document.getElementById('address' + name).options[document.getElementById('address' + name).selectedIndex].value.substring(1);
            } else {
                address[i] = String(document.getElementById('address' + name)[addressKeys[i][name]]).trim();
            }
        }
    }
    
    return address;
}

/* 
 * add an address to our address list 
 * if no address is supplied, then grab the current
 * address from the add address form
 */
quickAddress.addAddress = function(address) {
    var address = address || quickAddress.getAddressFromForm();
    var addressListNode = document.getElementById('addresslist');
    if (!address || !addressListNode) {
        return null;
    }

    var liNode = null;
    var html = '', input = '', time = '', value='';
    var addressKeys = quickAddress.addressKeys();
    var addressid = addressListNode.getAttribute('data-addressid');
    if (addressid) {
        /* editing an existing node */
        liNode = document.getElementById(addressid);
    } else {
        /* create a unique id for new node */
        addressid = this.uniqueId.next();
    }
    html += '<h4><i class="fa fa-times fa-clickme pull-right" onclick="quickAddress.deleteAddress(' + addressid + '); return false;"></i></h4>';
    html += '<h4><i class="fa fa-edit fa-clickme pull-right" onclick="quickAddress.editAddress(' + addressid + '); return false;"></i></h4>';
    
    /* add each address field */
    for (var i = 0; i < addressKeys.length; i++) {
        for (var name in addressKeys[i]) {
            value = '';
            if (i < addressKeys.length - 2) {
                if (i > 1 && address[i] != null && address[i].length) {
                    if (i > 3 && i < 5) {
                        html += address[i] + ' ';
                    } else {
                        html += address[i] + '<br />';
                    }
                }
                if (address[i] != null) {
                    value = ' value="' + address[i] + '"';
                }
            } else {
                if (i == addressKeys.length - 2) {
                    html += '<br />' + address[i] + ' years, ';
                } else {
                    html += address[i] + ' months';
                }
                if (address[i] != null) {
                    value = ' value="i' + address[i] + '"';
                }
            }
            input += '<input type="hidden" name="user[address][' + addressid + '][' + name + ']"' + value + '/>';
        }
    }

    if (liNode) {
        /* if li node exists, update it */
        liNode.innerHTML = html + input;
        jQuery(liNode).removeClass('hidden');
    } else {
        /* add new li node to document */
        liNode = document.createElement('li');
        liNode.id=addressid;
        liNode.innerHTML = html + input;
        liNode.className = 'addresslist-entry alert-info';
        addressListNode.appendChild(liNode);
    }
    quickAddress.clearAddressSearch();
    
    return address;
}

quickAddress.editAddress = function(addressid) {
    var key = null, keys = null, html = '';
    var addressNode = document.getElementById(addressid);
    var addressListNode = document.getElementById('addresslist');
    var address = addressNode.getElementsByTagName('input');
    
    /* clear address form */
    quickAddress.clearAddressAdd();
    
    /* populate address form */
    for (var i = 0; i < address.length; i++) {
        /* parse the name of the input array */
        keys = address[i].name.split(']');
        key = keys[keys.length - 2].substring(1);
        document.getElementById('address' + key).value = address[i].value;
        /* 
         * we put back the addressid, in case they save the form while
         * editing an address. this prevents the address priority from getting goofed.
         */
        if (key == 'addressid') {
            html = '<input type="hidden" name="' + address[i].name + '" value="' + address[i].value + '"/>';
        }
    }
    /* validate address */
    quickAddress.isValidAddress();
    
    /* store the id of the address we're editing */
    addressListNode.setAttribute('data-addressid', addressid);
    
    /* update address node */
    addressNode.innerHTML = html;
    jQuery(addressNode).addClass('hidden');
}

quickAddress.deleteAddress = function(addressid) {
    var addressNode = document.getElementById(addressid);
    if (!addressNode) {
        return false;
    }
    var address = addressNode.getElementsByTagName('input');
    if (!address) {
        return false;
    }
    
    var key = null, keys = null;
    var inputNode = null;
    var inputKey = null;
    var primaryKey = null;
    /* search address for a primary key */
    for (var i = 0; i < address.length; i++) {
        /* parse the name of the input array */
        keys = address[i].name.split(']');
        key = keys[keys.length - 2].substring(1);
        if (key == "addressid" && address[i].value > 0) {
            primaryKey = address[i].value;
            inputKey = address[i].name.substring(0, address[i].name.lastIndexOf("[")) + '[datedeleted]';
        }
    }
    /* if we have a primary key, set datedeleted, else just remove the address */
    if (primaryKey) {
        inputNode = document.createElement('input');
        inputNode.type = "hidden";
        inputNode.name = inputKey;
        inputNode.value = "sysdate";
        addressNode.appendChild(inputNode);
        addressNode.className = "hidden";
    } else {
        addressNode.parentNode.removeChild(addressNode);
    }
}

/* callback function passed on to quickAddress.search */
quickAddress.populateAddressForm = function(address) {
    jQuery('#addaddressform').removeClass('hidden');
    jQuery('#searchaddressform').addClass('hidden');
    document.getElementById('addressmoniker').value = address.moniker;
    document.getElementById('addressflatdetails').value = address.organisation + ' ' + address.subbuildingname;
    document.getElementById('addresshousename').value = address.buildingname;
    document.getElementById('addresshousenumber').value = address.buildingnumber;
    document.getElementById('addressstreet').value = address.thoroughfare;
    document.getElementById('addressdistrict').value = address.dependentlocality;
    document.getElementById('addresscity').value = address.town;
    document.getElementById('addresscounty').value = address.county;
    document.getElementById('addresspostcode').value = address.postcode;
    document.getElementById('addresstimeyears').selectedIndex = '';
    document.getElementById('addresstimemonths').selectedIndex = '';
    quickAddress.isValidAddress();
    
    return false;
}

/* this function requires jQuery and the jsTree 3.0.6 plugin */
quickAddress.search = function search(searchnode, searchtreeid, callbackfunction) {
    /* don't bother searching short strings */
    if (searchnode.value.length < 4) {
        return;
    }
    jQuery('#' + searchtreeid).jstree("destroy");
    jQuery('#' + searchtreeid).jstree({
        'core' : {
            'data' : {
                'url' : function (node) {
                    return node.id === '#' ? '/address.html?action=picklist&address=' + searchnode.value : '/address.html?action=refine&moniker=' + node.id;
                }
            },
            'themes' : {
                'icons' : false
            }
        }
    }).on("changed.jstree", function (e, data) {
        var selected = data.instance.get_node(data.selected);
        /* we are only interested in end nodes (nodes that have no children) */
        if (selected.state.loaded == true && !selected.children.length && selected.parent != '#') {
            if (callbackfunction) {
                var myrequest = new XMLHttpRequest();
                myrequest.onreadystatechange = function() {
                    if (myrequest.readyState == 4) {
                        if (myrequest.status == 200) {
                            var json = JSON.parse(myrequest.responseText);
                            if (!json) {
                                json = {};
                            }
                            jQuery('#' + searchtreeid).jstree("destroy");
                            callbackfunction(json);
                        }
                    }
                }
                myrequest.open("GET", '/address.html?action=format&moniker=' + selected.id, true);
                myrequest.send();
            }
        } else if (!selected.state.opened) { /* else if it's a parent node and not opened, open it */
            jQuery('#' + searchtreeid).jstree("open_node", selected);
        }
    });
}

</script>
<div class="smart-form"><!-- smart form !-->
<fieldset>
    <div class="row">
        <section class="col col-6">
            <footer>
                <button class="btn btn-default pull-left" onclick="quickAddress.clearAddressAdd(); return false;" title="Manually add an address">
                    Blank
                </button>
                <button class="btn btn-default pull-left" onclick="quickAddress.clearAddressSearch(); return false;" title="Search for a new address">
                    Search
                </button>
            </footer>
            <div id="searchaddressform">
                <section>
                    <label class="label" for="searchaddress">Enter postcode or place</label>
                    <label class="input">
                        <input type="text" name="searchaddress" id="searchaddress" placeholder="Postcode or Place" title="Search for a postcode or place name" onkeyup="quickAddress.search(this, 'addresssearch', quickAddress.populateAddressForm);">
                    </label>
                </section>
                <div id="addresssearch" class="scrollbox addressbox"></div>
            </div>
            <div id="addaddressform" class="hidden">
                <input type="hidden" name="addressaddressid" id="addressaddressid">
                <input type="hidden" name="addressmoniker" id="addressmoniker">
                <section>
                    <label class="label hidden toggle-label" for="addressflatdetails">Flat Details</label>
                    <label class="input"> 
                        <i class="icon-prepend fa fa-envelope-o fa-clickme" onclick="jQuery('#addaddressform label.toggle-label').toggleClass('hidden');"></i>
                        <input type="text" name="addressflatdetails" id="addressflatdetails" placeholder="Flat Details" title="Flat Details" onchange="quickAddress.isValidAddress();">
                    </label>
                </section>
                <section>
                    <label class="label hidden toggle-label" for="addresshousename">House Name</label>
                    <label class="input"> 
                        <i class="icon-prepend fa fa-envelope-o fa-clickme" onclick="jQuery('#addaddressform label.toggle-label').toggleClass('hidden');"></i>
                        <input type="text" name="addresshousename" id="addresshousename" placeholder="House Name" title="House Name" onchange="quickAddress.isValidAddress();">
                    </label>
                </section>
                <section>
                    <label class="label hidden toggle-label" for="addresshousenumber">House Number</label>
                    <label class="input"> 
                        <i class="icon-prepend fa fa-envelope-o fa-clickme" onclick="jQuery('#addaddressform label.toggle-label').toggleClass('hidden');"></i>
                        <input type="text" name="addresshousenumber" id="addresshousenumber" placeholder="House Number" title="House Number" onchange="quickAddress.isValidAddress();">
                    </label>
                </section>
                <section>
                    <label class="label hidden toggle-label" for="addressstreet">Street</label>
                    <label class="input"> 
                        <i class="icon-prepend fa fa-envelope-o fa-clickme" onclick="jQuery('#addaddressform label.toggle-label').toggleClass('hidden');"></i>
                        <input type="text" name="addressstreet" id="addressstreet" placeholder="Street" title="Street" onchange="quickAddress.isValidAddress();">
                    </label>
                </section>
                <section>
                    <label class="label hidden toggle-label" for="addressdistrict">District</label>
                    <label class="input"> 
                        <i class="icon-prepend fa fa-envelope-o fa-clickme" onclick="jQuery('#addaddressform label.toggle-label').toggleClass('hidden');"></i>
                        <input type="text" name="addressdistrict" id="addressdistrict" placeholder="District" title="District" onchange="quickAddress.isValidAddress();">
                    </label>
                </section>
                <section>
                    <label class="label hidden toggle-label" for="addresscity">Town / City</label>
                    <label class="input"> 
                        <i class="icon-prepend fa fa-envelope-o fa-clickme" onclick="jQuery('#addaddressform label.toggle-label').toggleClass('hidden');"></i>
                        <input type="text" name="addresscity" id="addresscity" placeholder="Town / City" title="Town / City" onchange="quickAddress.isValidAddress();">
                    </label>
                </section>
                <section>
                    <label class="label hidden toggle-label" for="addresscounty">County</label>
                    <label class="input"> 
                        <i class="icon-prepend fa fa-envelope-o fa-clickme" onclick="jQuery('#addaddressform label.toggle-label').toggleClass('hidden');"></i>
                        <input type="text" name="addresscounty" id="addresscounty" placeholder="County" title="County" onchange="quickAddress.isValidAddress();">
                    </label>
                </section>
                <section>
                    <label class="label hidden toggle-label" for="addresspostcode">Postcode</label>
                    <label class="input"> 
                        <i class="icon-prepend fa fa-envelope-o fa-clickme" onclick="jQuery('#addaddressform label.toggle-label').toggleClass('hidden');"></i>
                        <input type="text" name="addresspostcode" id="addresspostcode" placeholder="Postcode" title="Postcode" onchange="quickAddress.isValidAddress();">
                    </label>
                </section>
                <div class="row">
                    <section class="col col-6">
                        <label class="label">Years at Address</label>
                        <label class="select" for="addresstimeyears">
                        <select name="addresstimeyears" id="addresstimeyears" title="Years at Address" onchange="quickAddress.isValidAddress();">
                            <?php
                                $options = array("" => "Years");
                                for ($i = 0; $i < 30; $i++) {
                                    $options["i" . $i] = $i;
                                }
                                $name = "timeyears";
                                $disabled = " disabled=\"disabled\"";
                                $optionlist = "";
                                foreach ($options as $key => $value) {
                                    $optionlist .= "<option value=\"{$key}\"{$disabled}>{$value}</option>\n";
                                    $disabled = "";
                                }
                                echo $optionlist;
                            ?>
                        </select> <i></i> </label>
                    </section>
                    <section class="col col-6">
                        <label class="label">Months at Address</label>
                        <label class="select" for="addresstimemonths">
                            <select name="addresstimemonths" id="addresstimemonths" title="Months at Address" onchange="quickAddress.isValidAddress();">
                                <?php
                                    $options = array("" => "Months");
                                    for ($i = 0; $i < 12; $i++) {
                                        $options["i" . $i] = $i;
                                    }
                                    $name = "timemonths";
                                    $disabled = " disabled=\"disabled\"";
                                    $optionlist = "";
                                    foreach ($options as $key => $value) {
                                        $optionlist .= "<option value=\"{$key}\"{$disabled}>{$value}</option>\n";
                                        $disabled = "";
                                    }
                                    echo $optionlist;
                                ?>
                            </select> <i></i> </label>
                    </section>
                </div><!-- row -->
                <button id="addaddressbutton" type="submit" class="btn btn-sm btn-success hidden" onclick="quickAddress.addAddress(); return false;">
                    Add Address
                </button>
            </div><!-- addaddressform -->
        </section>
        <ul id="addresslist" class="scrollbox height50"></ul>
    </div><!-- row -->
</fieldset>
<footer>
    <button type="submit" class="btn btn-primary">
        Save Addresses
    </button>
</footer>
</div><!-- smart form !-->
<script type="text/javascript">
jQuery("#addresslist").sortable();
jQuery("#addresslist").disableSelection();
<?php
    foreach ($model['user']['address']['value'] as $address) {
        if (!$address['address']['datedeleted']['value']) {
            echo "quickAddress.addAddress(" . address::json_array($address) . ");\n";
        }
    }
?>
</script>