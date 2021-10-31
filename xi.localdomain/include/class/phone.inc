<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class phone extends model {

    public function tablename() {
        return "phone";
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
    
    public function dset_phone($input) {
        $input = clean::integer($input);
        if (strlen($input) < 1) {
            $this->error("phone", "you must supply a valid phone number");
        } else {
            $this->error("phone", null);
        }
        
        $this->set("phone", $input);
    }
}
