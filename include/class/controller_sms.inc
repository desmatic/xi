<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_sms extends controller {

    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "sms/include/";
    }
    
    public function module_path() {
        return "/sms/";
    }
    
    public function can_action_all() {
        return true;
    }

    public function can_view_all() {
        return true;
    }

    public function view_index() {
        $page = $this->page();
        $page['title'] = "SMS Messaging";
        $page['widget'] = $this->view("index.inc");
        echo $this->template($page);
    }
    
}
