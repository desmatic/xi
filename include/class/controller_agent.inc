<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_agent extends controller {
    
    private $_model;

    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "agent/include/";
    }

    public function can_action_all() {
        return session::agent_can_do(cando::DEVELOPER);
    }

    public function can_view_all() {
        return session::agent_can_do(cando::DEVELOPER);
    }
    
    public function module_path() {
        return "/agent/";
    }

    public function module_menu_title() {
        return "Agent";
    }
    
    public function model() {
        return $this->_model;
    }

    protected function bootstrap() {
//         $this->_model = new skeleton();
//         if ($id = $this->_model->request_key($_REQUEST)) {
//             $this->_model->load($id);
//         }
//         $this->_model->attributes($_REQUEST);
    }

    public function menu() {
        $i = 0;
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = $this;
        $links[$i]->view = "index";
        $links[$i]->innerHTML = "Agent Management";
        $links[$i]->title = "Agent Management";
        
        return $links;
    }

    public function view_index() {
        $page = $this->page();
        $page['title'] = "Agent Management";
        $page['widget'] = $this->view_widget("Agents", "index.inc");
        echo $this->template($page);
    }

    
}
