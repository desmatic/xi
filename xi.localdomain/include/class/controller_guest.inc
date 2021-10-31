<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_guest extends controller {
    
    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "guest/include/";
    }
    
    public function module_path() {
        return "/guest/";
    }
    
    public function can_action_all() {
        return session::user_can_do(cando::GUEST);
    }

    public function can_view_all() {
        return session::user_can_do(cando::GUEST);
    }

    public function view_index() {
        $page = $this->page();
        $page['title'] = "Email";
        $page['widget'] = $this->view("index.inc");
        echo $this->template($page);
    }
    
}
