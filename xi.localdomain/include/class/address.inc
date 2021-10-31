<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class address extends model {

    private $_years;
    private $_months;

    public function tablename() {
        return "address";
    }

    public function dset_userid($input) {
        $input = clean::integer($input);
        if ($input < 1) {
            $this->error("userid", "you must supply a userid");
        } else {
            $this->error("userid", null);
        }
        
        $this->set("userid", $input);
    }
    
    public function dset_postcode($input) {
        $input = clean::alpha_numeric($input);
        if (strlen($input) < 1) {
            $this->error("postcode", "you must supply a valid postcode");
        } else {
            $this->error("postcode", null);
        }
        
        $this->set("postcode", $input);
    }
    
    public function dset_months($input) {
        $input = clean::integer($input);
        $this->set("months", $input);
    }
    
    public function dset_datedeleted($input) {
        if ($input == "sysdate") {
            $this->set("datedeleted", new expression($this->db()->sysdate()));
            $this->deletedby = session::userid();
        }
    }
    
    private function _dset_timeyears_timemonths() {
        if ($this->_years !== null && $this->_months !== null) {
            $this->months = $this->_years * 12 + $this->_months;
        }
    }
    
    public function dset_timeyears($input) {
        $input = clean::integer($input);
        $this->_years = $input;
        $this->_dset_timeyears_timemonths();
    }
    
    public function dset_timemonths($input) {
        $input = clean::integer($input);
        $this->_months = $input;
        $this->_dset_timeyears_timemonths();
    }
    
    public function dget_timemonths() {
        return $this->months % 12;
    }
    
    public function dget_timeyears() {
        return floor($this->months / 12);
    }
    
    public function save() {
        if (!$this->loaded()) {
            $this->createdby = session::userid();
        } else {
            $this->updatedby = session::userid();
        }
        $this->priority = $this->attributes_orderid();
        
        return parent::save();
    }
    
    public static function get_form_fields() {
        return array(
            'addressid' => 'value',
            'moniker' => 'value',
            'flatdetails' => 'value',
            'housename' => 'value',
            'housenumber' => 'value',
            'street' => 'value',
            'district' => 'value',
            'city' => 'value',
            'county' => 'value',
            'postcode' => 'value',
            'timeyears' => 'selectedIndex',
            'timemonths' => 'selectedIndex',
        );
    }
    
    public static function get_json_form_fields($model = null) {
        $fields = array();
        foreach (self::get_form_fields() as $key => $value) {
            $o = new stdClass();
            $o->$key = $value;
            $fields[] = $o;
        }
        
        return json_encode($fields);
    }
    
    public static function json_array($model) {
        $fields = array();
        foreach (self::get_form_fields() as $key => $value) {
            $fields[] = $model['address'][$key]['value'];
        }
        
        return json_encode($fields);
    }
}
