<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class vuser extends model {

    private $_can_do;

    public function tablename() {
        return "vuser";
    }
    
    public function sql_load($id) {
        $phone = new phone();
        $address = new address();
        $agent = new agent();
        $sql = $this->sql()->
            select()->
                field($this, new expression("now()"), "today")->
                all($this)->
                all($agent)->
                all($phone)->
                all($address)->
            from($this)->
            left_join($agent)->
            on($this, "userid", $agent, "userid")->
            left_join($phone)->
            on($this, "userid", $phone, "userid")->
            left_join($address)->
            on($this, "userid", $address, "userid")->
            expression($this->sql_where($id))->
            order_by()->field($address, "priority")->field($phone, "phoneid")->desc();

        return $sql;
    }
    
    public function dset_title($input) {
        $this->set("title", clean::alpha($input));
    }

    public function dset_forename($input) {
        $this->set("forename", clean::alpha_numeric($input));
    }
    
    public function dset_surname($input) {
        $this->set("surname", clean::alpha_numeric($input));
    }
    
    public function dset_dateofbirth($input) {
        $input = clean::sql_date($input);
        if ($input) {
            $input .= " 00:00:00";
        }
        $this->set("dateofbirth", $input);
    }
    
    public function dget_dateofbirth() {
        return clean::sql_date($this->get("dateofbirth"));
    }
    
    public function dset_email($input) {
        $this->set("email", clean::email($input));
    }
    
    public function dset_mobile($input) {
        $this->set("mobile", clean::phone($input));
    }

    private function _get_can_do() {
        $trustlevelid = session::trustlevelid();
        $sql = "
            select {$this->db->iquote('candoid')}
            from {$this->db()->iquote('vusercando')}
            where {$this->db()->iquote('userid')}={$this->db()->quote($this->userid)}
            and {$this->db()->iquote('trustlevelid')}<={$trustlevelid}"
            ;
        $rows = array();
        if ($sth = $this->db()->select($sql)) {
            while($row = $this->db()->fetch_array($sth)) {
                $rows[$row['candoid']] = true;
            }
        }
        
        return array_keys($rows);
    }

    public function can_do($candoid, $userid = null) {
        if (!is_numeric($candoid)) {
            return false;
        }
        if ($candoid == cando::GUEST) {
            return true;
        }
        if (!$this->loaded()) {
            return false;
        }
        if ($this->userid == define::SYSTEM_SUPERUSERID) {
            return true;
        }
        if ($userid && $this->userid != $userid) {
            return false;
        }
        if ($this->_can_do === null) {
            $this->_can_do = $this->_get_can_do();
        }
        if (is_array($candoid)) {
            foreach ($candoid as $id) {
                if (in_array($id, $this->_can_do)) {
                    return true;
                }
            }
        } else {
            return in_array($candoid, $this->_can_do);
        }

        return false;
    }
    
    public function agent_can_do($candoid, $userid = null) {
    
        return $this->can_do($candoid, $userid) && isset($this->agent) && $this->agent[0]->agentid;
    }
    
    public function reset_can_do() {
        $this->_can_do = $this->_get_can_do();
    }

    public function save() {
        if (!$this->changed()) { /* nothing to do, just return */
            return true;
        }
        if (!$this->loaded()) { /* only updates allowed */
            return false;
        }
        
        return parent::save();
    }

}
