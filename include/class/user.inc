<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class user extends vuser {

    private $_login;
    private $_logincheck;
    private $_password = null;
    private $_currentpassword = null;
    private $_newpassword = null;
    private $_newpasswordcheck = null;
    private $_terms = null;

    public function tablename() {
        return "user";
    }
    
    public function sql_load($id) {
        $phone = new phone();
        $address = new address();
        $agent = new agent();
        $user_filedrive = new user_filedrive();
        $user_filedrive->readonly();
        $filedrive = new filedrive();
        $filedrive->readonly();
        $sql = $this->sql()->
            select()->
                field($this, new expression("now()"), "today")->
                all($this)->
                all($agent)->
                all($phone)->
                all($address)->
                all($user_filedrive)->
                field($filedrive, "filedriveid")->
                field($filedrive, "md5sum")->
                field($filedrive, "bytes")->
            from($this)->
            left_join($agent)->
            on($this, "userid", $agent, "userid")->
            left_join($phone)->
            on($this, "userid", $phone, "userid")->
            left_join($address)->
            on($this, "userid", $address, "userid")->
            left_join($user_filedrive)->
            on($this, "user_filedriveid", $user_filedrive, "user_filedriveid")->
            left_join($filedrive)->
            on($user_filedrive, "filedriveid", $filedrive, "filedriveid")->
            expression($this->sql_where($id))->
            order_by()->field($address, "priority")->field($phone, "phoneid")->desc();

        return $sql;
    }

    public function dget_md5sum() {
        if (!empty($this->user_filedrive[0]->filedrive[0]->md5sum)) {
            return $this->user_filedrive[0]->filedrive[0]->md5sum;
        }
        
        return define::ANONYMOUS_THUMB_MD5SUM;
    }
    
    public function dget_bytes() {
        if (!empty($this->user_filedrive[0]->filedrive[0]->bytes)) {
            return $this->user_filedrive[0]->filedrive[0]->bytes;
        }
        
        return define::ANONYMOUS_THUMB_BYTES;
    }

    private static function _password_key() {
        return "eHuY8sP485jP6Yz8ulVxVj8AfQugW3xRv3t8S8no0I637Jl00ovyXcvXh3vgKkRt";
    }

    public function dset_password($input) {
        $input = clean::password($input);
        $this->_password = $input;
        $this->set("password", session::encrypt($input, self::_password_key()));
    }

    public function checkpassword() {
        $authenticated = session::authenticated();
        if (!$authenticated) {
            $this->error("password", "oops, wrong password");
        } else {
            $this->error("password", null);
        }
        
        return $authenticated;
    }
    
    public function dset_login($input) {
        $this->_login = $input;
        if ($input && clean::integer($input) == $input) {
            $this->mobile = $input;
        } else {
            $this->email = $input;
        }
    }
    
    public function dget_login() {
        return $this->_login;
    }
    
    public function dset_logincheck($input) {
        $this->_logincheck = $input;
    }
    
    public function dget_logincheck() {
        return $this->_logincheck;
    }
    
    public function dset_currentpassword($input) {
        $this->_currentpassword = clean::password($input);
    }
    
    public function dget_currentpassword() {
        return $this->_currentpassword;
    }
    
    public function dset_newpassword($input) {
        $this->_newpassword = clean::password($input);
    }
    
    public function dget_newpassword() {
        return $this->_newpassword;
    }
    
    public function dset_newpasswordcheck($input) {
        $this->_newpasswordcheck = clean::password($input);
    }
    
    public function dget_newpasswordcheck() {
        return $this->_newpasswordcheck;
    }
    
    public function dset_terms($input) {
        if ($input == "on") {
            $this->_terms = true;
        } else {
            $this->_terms = false;
        }
    }
    
    public function dget_terms() {
        return $this->_terms;
    }

    public function challenge_password($challenge) {
        $challenge = clean::password($challenge);
        $password = session::decrypt($this->password, self::_password_key());

        return strlen($challenge) ? $password == $challenge : false;
    }

    public function invalid() {
        $return = false;
        if ($this->login && $this->login != $this->logincheck) {
            if ($this->mobile) {
                $this->error("login", "oops, mobile numbers don't match");
            } else {
                $this->error("login", "oops, email addresses don't match");
            }
            $return = true;
        }
        if (!$this->title) {
            $this->error("title", "please select a title");
            $return = true;
        }
        if (strlen($this->forename) < 1) {
            $this->error("forename", "forename must have at least one character");
            $return = true;
        }
        if (strlen($this->surname) < 2) {
            $this->error("surname",  "surname must have at least two characters");
            $return = true;
        }
        if (!$this->dateofbirth) {
            $this->error("dateofbirth", "please enter a valid date of birth" . $this->dateofbirth);
            $return = true;
        }

        if ($this->login && $this->mobile && $this->login != $this->mobile) {
            $this->error("login", "invalid phone number");
            $return = true;
        } else if (!$this->mobile && !$this->email) {
            $this->error("login", "please enter a valid email or phone number");
            $return = true;
        }
        
        return $return;
    }
    
    public function invalid_signup() {
        $return = $this->invalid();
        if (strlen($this->_password) < 8) {
            $this->error("password", "password must have at least eight characters");
            $return = true;
        }
        if (!$this->terms) {
            $this->error("terms", "you must agree to the terms and conditions");
            $return = true;
        }
        
        return $return;
    }

    public function save() {
        if (!$this->changed()) { /* nothing to do, just return */
            return true;
        }
        if ($this->invalid()) {
            dlog::decho(get_class($this) . "->invalid() returned " . print_r($this->get_error(), true));
            return false;
        }
        if ($this->mobile) {
            $this->dedupe('mobile', $this->mobile);
        }
        if ($this->email) {
            $this->dedupe('email', $this->email);
        }
        
        return parent::save();
    }
    
    public function invalid_change_password() {
        $return = false;
        $this->error_reset();
        if (!$this->loaded()) {
            $this->error("newpassword", "oh dear, system error");
            return true;
        } else if (strlen($this->_newpassword) < 8) {
            $this->error("newpassword", "password must have at least eight characters");
            $return = true;
        } else if ($this->_newpassword !== $this->_newpasswordcheck) {
            $this->error("newpassword", "oops, your passwords do not match ");
            $return = true;
        } else if (!$this->challenge_password($this->_currentpassword)) {
            $this->error("currentpassword", "oops, wrong password");
            $return = true;
        }
        
        return $return;
    }
    
    public function change_password() {
        if ($this->invalid_change_password()) {
            dlog::decho(get_class($this) . "->invalid() returned " . print_r($this->get_error(), true));
            return false;
        }
        $this->password = $this->_newpassword;
        if (parent::save()) {
            return true;
        }
        
        return false;
    }
}
