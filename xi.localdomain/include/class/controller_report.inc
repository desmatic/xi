<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_report extends controller {

    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "report/include/";
    }
    
    public function module_path() {
        return "/report/";
    }
    
    public function can_action_all() {
        return session::agent_can_do(cando::LOGIN);
    }

    public function can_view_all() {
        return session::agent_can_do(cando::LOGIN);
    }

    public function view_index() {
        $page = $this->page();
        $page['title'] = "Reports";
        $page['content'] = $this->view("index.inc");
        echo $this->template($page);
    }
    
}
