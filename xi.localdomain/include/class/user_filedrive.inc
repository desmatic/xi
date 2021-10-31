<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class user_filedrive extends model {

    private $_mimetype;
    private $_file;
    private $_thumb128x128;
    private $_thumb504;
    private $_md5sum;
    private $_bytes;

    public function tablename() {
        return "user_filedrive";
    }
    
    public function dset_mimetype($input) {
        $this->_mimetype = $input;
    }
    
    public function dset_file($input) {
        $this->_file = $input;
    }
    
    public function dset_thumb128x128($input) {
        $this->_thumb128x128 = $input;
    }
    
    public function dset_thumb504($input) {
        $this->_thumb504 = $input;
    }
    
    public function dset_md5sum($input) {
        $this->_md5sum = $input;
    }
    
    public function dset_bytes($input) {
        $this->_bytes = $input;
    }
    
    public function dget_mimetype() {
        return $this->_mimetype;
    }
    
    public function dget_file() {
        return $this->_file;
    }

    public function dget_thumb128x128() {
        return $this->_thumb128x128;
    }
    
    public function dget_thumb504() {
        return $this->_thumb504;
    }
    
    public function dget_md5sum() {
        return $this->_md5sum;
    }
    
    public function dget_bytes() {
        return $this->_bytes;
    }

    public function save() {
        $f = new filedrive();
        $f->mimetype = $this->_mimetype;
        $f->file = $this->_file;
        $f->thumb128x128 = $this->_thumb128x128;
        $f->thumb504 = $this->_thumb504;
        $f->md5sum = $this->_md5sum;
        $f->bytes = $this->_bytes;
        $sql = $this->db()->sql()->
            select()->
                field($f, "filedriveid")->
            from($f)->
            where()->field($f, "md5sum")->eq()->quote($f->md5sum)->
            a()->field($f, "bytes")->eq()->quote($f->bytes)->
            limit(1);
        if ($f->save()) {
            $this->filedriveid = $f->filedriveid;
        } else if ($rows = $sql->fetch_all()) {
            $this->filedriveid = $rows[0]['filedriveid'];
        } else {
            return false;
        }
        
        return parent::save();
    }
    
    public function load($id) {
        $f = new filedrive();
        if (parent::load($id) && $f->load($this->filedriveid)) {
            $this->_file = $f->file;
            $this->_mimetype = $f->mimetype;
            $this->_thumb128x128 = $f->thumb128x128;
            $this->_thumb504 = $f->thumb504;
            $this->_md5sum = $f->md5sum;
            $this->_bytes = $f->bytes;
            
            return true;
        }
        
        return false;
    }
    
    public static function save_upload_file($index = "file") {
        $f = null;
        $thumb128x128 = null;
        $thumb504 = null;
        if (
            (isset($_FILES[$index]['tmp_name'])) &&
            (is_uploaded_file($_FILES[$index]['tmp_name'])) &&
            ($fhdl = fopen($_FILES[$index]['tmp_name'], "r")) &&
            ($file = fread($fhdl, $_FILES[$index]['size']))
        ) {
            fclose($fhdl);
            $f = new user_filedrive();
            $f->mimetype = filedrive::get_file_mimetype($_FILES[$index]['tmp_name']);
            $f->filename = $_FILES[$index]['name'];
            $f->bytes = strlen($file);
            $f->md5sum = md5($file);
            $f->file = base64_encode($file);
            if (filedrive::is_image($f->mimetype)) {
                if ($thumb128x128 = filedrive::square_thumbnail($_FILES[$index]['tmp_name'], 128)) {
                    $f->thumb128x128 = base64_encode($thumb128x128);
                }
                if ($thumb504 = filedrive::slider_thumbnail($_FILES[$index]['tmp_name'])) {
                    $f->thumb504 = base64_encode($thumb504);
                }
            }
            $f->userid = session::userid();
            $f->save();
        }
        
        return $f;
    }
    
    public static function save_upload_picture($index = "file") {
        $f = null;
        $mimetype = null;
        $thumb128x128 = null;
        $thumb504 = null;
        if (
            (isset($_FILES[$index]['tmp_name'])) &&
            (is_uploaded_file($_FILES[$index]['tmp_name'])) &&
            ($mimetype = filedrive::get_file_mimetype($_FILES[$index]['tmp_name'])) &&
            (filedrive::is_image($mimetype)) &&
            ($thumb128x128 = filedrive::square_thumbnail($_FILES[$index]['tmp_name'], 128)) &&
            ($thumb504 = filedrive::slider_thumbnail($_FILES[$index]['tmp_name'])) &&
            ($fhdl = fopen($_FILES[$index]['tmp_name'], "r")) &&
            ($file = fread($fhdl, $_FILES[$index]['size']))
        ) {
            fclose($fhdl);
            $f = new user_filedrive();
            $f->mimetype = $mimetype;
            $f->filename = $_FILES[$index]['name'];
            $f->bytes = strlen($file);
            $f->md5sum = md5($file);
            $f->file = base64_encode($file);
            $f->thumb128x128 = base64_encode($thumb128x128);
            $f->thumb504 = base64_encode($thumb504);
            $f->userid = session::userid();
            $f->save();
        }
        
        return $f;
    }
    
    public function json_description() {
        $json = new stdClass();
        $json->user_filedriveid = $this->user_filedriveid;
        $json->md5sum = $this->md5sum;
        $json->bytes = $this->bytes;
        $json->userid = $this->userid;
        $json->filename = $this->filename;
        $json->mimetype = $this->mimetype;
        $json->datecreated = $this->datecreated;
        $json->extension = filedrive::extension($this->mimetype);
        
        return json_encode($json);
    }
    
    public function user_files($userid) {

        $sql = $this->db()->sql()->
            select()->
                all("u")->
                field("f", "mimetype")->
                field("f", "bytes")->
                field("f", "md5sum")->
        
            from("user_filedrive", "u")->
            
            join("filedrive", "f")->
            on("u", "filedriveid", "f", "filedriveid")->
            
            where()->field("u", "userid")->eq()->quote($userid)->
            a()->field("u", "datetrashed")->is_null()->
            
            order_by()->field("u", "datecreated");
            
       return $sql->fetch_all();
    }
    
    public function user_files_trashed($userid) {

        $sql = $this->db()->sql()->
            select()->
                all("u")->
                field("f", "mimetype")->
                field("f", "bytes")->
                field("f", "md5sum")->
        
            from("user_filedrive", "u")->
            
            join("filedrive", "f")->
            on("u", "filedriveid", "f", "filedriveid")->
            
            where()->field("u", "userid")->eq()->quote($userid)->
            a()->field("u", "datetrashed")->is_not_null()->
            a()->field("u", "datedeleted")->is_null()->
            
            order_by()->field("u", "datecreated")->desc();
            
       return $sql->fetch_all();
    }
    
    public function delete_user_files($userid) {
        $sql = $this->db()->sql()->
            update("user_filedrive", "u")->
            set("u", "datedeleted")->eq()->sysdate()->
            where()->field("u", "datetrashed")->is_not_null()->
            a()->field("u", "datedeleted")->is_null()->
            a()->field("u", "userid")->eq()->quote($userid);   
        
        return $sql->update_all();
    }
    
    public function get_user_file($md5sum, $bytes, $userid) {
        $sql = $this->db()->sql()->
            select()->
                all("f")->
        
            from("user_filedrive", "u")->
            
            join("filedrive", "f")->
            on("u", "filedriveid", "f", "filedriveid")->
            
            where()->field("u", "userid")->eq()->quote($userid)->
            a()->field("f", "md5sum")->eq()->quote($md5sum)->
            a()->field("f", "bytes")->eq()->quote($bytes)->
            
            order_by()->field("u", "datecreated")->
            
            limit(1);
            
       return $sql->fetch_all();
    }
    
    public function get_user_file_details($user_filedriveid, $userid) {
        $sql = $this->db()->sql()->
            select()->
                field("uf", "filename", "Filname")->
                field("uf", "datecreated", "Date")->
                field("u", "forename", "From")->
                field("u", "surname", "Surname")->
                field("f", "bytes", "Bytes")->
                field("f", "md5sum", "MD5sum")->
        
            from("user_filedrive", "uf")->
            
            join("user", "u")->
            on("u", "userid", "uf", "userid")->
            
            join("filedrive", "f")->
            on("uf", "filedriveid", "f", "filedriveid")->
            
            where()->field("uf", "userid")->eq()->quote($userid)->
            a()->field("uf", "user_filedriveid")->eq()->quote($user_filedriveid);
            
        if ($rows = $sql->fetch_all()) {
            $rows[0]["From"] .= " " . $rows[0]["Surname"];
            unset($rows[0]["Surname"]);
        }
       
       return $rows;
    }
    
    public function get_user_file_image($md5sum, $bytes, $size = null) {
        switch($size) {
            case "thumb128x128":
            case "thumb504":
                break;
            default:
                $size = "thumb128x128";
        }
    
        $sql = $this->db()->sql()->
            select()->
                all("f")->
        
            from("user_filedrive", "u")->
            
            join("filedrive", "f")->
            on("f", "filedriveid", "f", "filedriveid")->
            
            where()->field("f", "md5sum")->eq()->quote($md5sum)->
            a()->field("f", "bytes")->eq()->quote($bytes)->
            a()->field("f", "thumb128x128")->e("is NOT NULL")->
            a()->field("f", "datedeleted")->e("is NULL")->
            
            order_by()->field("u", "datecreated")->
            
            limit(1);
        
        if ($rows = $sql->fetch_all_cached()) {
            return base64_decode($rows[0][$size]);
        }
            
        return null;
    }
}
