<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_print extends controller {

    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "print/include/";
    }
    
    public function module_path() {
        return "/print/";
    }
    
    public function can_action_all() {
        return session::agent_can_do(cando::LOGIN);
    }

    public function can_view_all() {
        return session::agent_can_do(cando::LOGIN);
    }

    public function view_index() {
        $page = $this->page();
        $page['title'] = "Printing";
        $page['content'] = $this->view("index.inc");
        echo $this->template($page);
    }
    
}
