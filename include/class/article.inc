<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class article extends model {

    private $_date_start = null;
    private $_time_start = "00:00";
    private $_date_end = null;
    private $_time_end = "23:59";

    public function tablename() {
        return "article";
    }
    
    public function sql_load($id) {
        $article_company = new article_company();
        $user = new user();
        $user->readonly();
        $user_filedrive = new user_filedrive();
        $user_filedrive->readonly();
        $filedrive = new filedrive();
        $filedrive->readonly();
        $sql = $this->sql()->
            select()->
                all($this)->
                all($article_company)->
                all($user_filedrive)->
                all($filedrive)->
                field($user, "userid")->
                field($user, "nickname")->
                field($filedrive, "filedriveid")->
                field($filedrive, "md5sum")->
                field($filedrive, "bytes")->
            from($this)->
            left_join($article_company)->
            on($this, "articleid", $article_company, "articleid")->
            join($user)->
            on($this, "userid", $user, "userid")->
            
            left_join($user_filedrive)->
            on($this, "user_filedriveid", $user_filedrive, "user_filedriveid")->
            
            left_join($filedrive)->
            on($user_filedrive, "filedriveid", $filedrive, "filedriveid")->
            
            expression($this->sql_where($id))->
            order_by()->field($article_company, "companyid")->field($this, "articleid");

        return $sql;
    }
    
    /* override parent */
    public function clean_attribute($key, $value) {
        if ($key == "article") {
            return clean::note($value);
        }
        
        return clean::html($value);
    }
    
    public function dset_datetimestart($input) {
        $this->_time_start = clean::sql_time($input);
        $this->_date_start = clean::sql_date($input);
        if ($this->_date_start && $this->_time_start) {
            $this->set("datetimestart", $this->_date_start . " " . $this->_time_start);
        }
    }

    public function dset_datestart($input) {
        if (strlen($input) > 10) {
            $this->_time_start = clean::sql_time($input);
        }
        $this->_date_start = clean::sql_date($input);
        if ($this->_date_start && $this->_time_start) {
            $this->set("datetimestart", $this->_date_start . " " . $this->_time_start);
        }
    }
    
    public function dset_timestart($input) {  
        $this->_time_start = clean::sql_time($input);
        if ($this->_date_start && $this->_time_start) {
            $this->set("datetimestart", $this->_date_start . " " . $this->_time_start);
        }
    }
    
    public function dset_datetimeend($input) {
        $this->_time_end = clean::sql_time($input);
        $this->_date_end = clean::sql_date($input);
        if ($this->_date_end && $this->_time_end) {
            $this->set("datetimeend", $this->_date_end . " " . $this->_time_end);
        }
    }
    
    public function dset_dateend($input) {
        if (strlen($input) > 10) {
            $this->_time_end = clean::sql_time($input);
        }
        $this->_date_end = clean::sql_date($input);
        if ($this->_date_end && $this->_time_end) {
            $this->set("datetimeend", $this->_date_end . " " . $this->_time_end);
        }
    }
    
    public function dset_timeend($input) {  
        $this->_time_end = clean::sql_time($input);
        if ($this->_date_end && $this->_time_end) {
            $this->set("datetimeend", $this->_date_end . " " . $this->_time_end);
        }
    }
    
    public function dset_delete($input) {
        /* we can't delete the first article */
        if (strtolower(trim($input)) == "delete" && $this->articleid > 1) {
            $this->set("datedeleted", new expression($this->db()->sysdate()));
        } else {
            $this->set("datedeleted", null);
        }
    }
    
    public function dset_pause($input) {
        if (strtolower(trim($input)) == "pause") {
            $this->set("datepaused", new expression($this->db()->sysdate()));
        } else {
            $this->set("datepaused", null);
        }
    }
    
    public function dset_pin($input) {
        if (strtolower(trim($input)) == "pin") {
            $this->set("datepinned", new expression($this->db()->sysdate()));
        } else {
            $this->set("datepinned", null);
        }
    }

    public function dget_datestart() {
        return $this->_date_start;
    }
    
    public function dget_timestart() {
        return clean::time($this->_time_start);
    }
    
    public function dget_dateend() {
        return $this->_date_end;
    }
    
    public function dget_timeend() {
        return clean::time($this->_time_end);
    }

    public function invalid() {
        $return = false;
        if (!$this->title) {
            $this->error("title", "please enter a unique title");
            $return = true;
        }
        if (!$this->datestart) {
            $this->error("datestart", "please enter a start date");
            $return = true;
        }
        if (!$this->dateend) {
            $this->error("dateend", "please enter an end date");
            $return = true;
        }
        
        return $return;
    }
    
    public function save() {
        if ($this->loaded() && !$this->changed()) { /* nothing to do, just return */
            return true;
        }
        if ($this->invalid()) {
            return false;
        }
        $this->userid = session::userid();
        $this->controllerid = 1;
        
        return parent::save();
    }
    
    public function load($id) {
        if ($return = parent::load($id)) {
            $this->_date_start = clean::sql_date($this->get("datetimestart"));
            $this->_time_start = clean::sql_time($this->get("datetimestart"));
            $this->_date_end = clean::sql_date($this->get("datetimeend"));
            $this->_time_end = clean::sql_time($this->get("datetimeend"));
        }
        
        return $return;
    }
    
    public function save_upload_picture($file = "file") {
        if ($this->loaded() && isset($_FILES[$file])) {
            if ($f = user_filedrive::save_upload_picture($file)) {
                $this->user_filedriveid = $f->user_filedriveid;
                return $this->save();
            }
        }
        
        return false;
    }
    
    public function search_list() {
        $user = new user();
        $sql = $this->sql()->
            select()->
                field($this, "articleid", "ID")->
                field($user, "nickname", "Author")->
                field($this, "title", "Title")->
                field($this, "datecreated", "Created")->
                field($this, "datemodified", "Modified")->
                field($this, "datestart", "Starts")->
                field($this, "dateend", "Ends")->
            from($this)->
            join($user)->
            on($this, "userid", $user, "userid")->
            order_by()->field($this, "datemodified")->desc()->field($this, "title")->
            limit(500);

        return $sql->fetch_all();
    }

    public static function parse_answers($answers, $is_answer = null) {
        $answers_array = array();
        if ($answers = explode("\n", $answers)) {
            foreach($answers as $answer) {
                $answer = trim($answer);
                if ($answer) {
                    $answers_array[] = $answer;
                }
            }
        }
        
        if (!empty($is_answer) && $answers_array) {
            $is_answer = trim($is_answer);
            if (in_array($is_answer, $answers_array)) {
                return $is_answer;
            } else {
                return null;
            } 
        }
        
        return $answers_array;
    }
    
}
