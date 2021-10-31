<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_foobar extends controller {
    
    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "development/foobar/include/";
    }
    
    public function module_path() {
        return "/development/foobar/";
    }
    
    public function can_action_all() {
        return session::agent_can_do(cando::DEVELOPER);
    }

    public function can_view_all() {
        return session::agent_can_do(cando::DEVELOPER);
    }

    public function view_index() {
        $page['title'] = "Foobar";
        $page['content'] = $this->view("index.inc");
        echo $this->template($page);
    }
    
}
