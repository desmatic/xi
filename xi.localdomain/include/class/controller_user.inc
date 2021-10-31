<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_user extends controller {
    
    private $_model;

    public function model() {
        if ($this->_model === null) {
            $this->_model = new user();
        }
        return $this->_model;
    }


    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "user/include/";
    }
    
    public function module_path() {
        return "/user/";
    }

    public function module_menu_title() {
        return "Welcome";
    }
    
    public function can_action_all() {
        return session::agent_can_do(cando::AGENT);
    }

    public function can_view_all() {
        return session::agent_can_do(cando::AGENT);
    }
    
    /* any links that should consider their parent link user */
    public function menu_user() {
        $i = 0;
        $links = array();
        $links[$i] = new html_a();
        $links[$i]->href = $this->href("add_user");
        
        return $links;
    }

    public function menu() {
        $i = 0;
        $links = array();
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "Search";
        $links[$i]->view = "index";
        $links[$i]->controller = $this;
        $i++;
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "User";
        $links[$i]->view = "user";
        $links[$i]->controller = $this;
//         $links[$i] = new html_a();
//         $links[$i]->controller = $this;
//         $links[$i]->view = "index";
//         $links[$i]->innerHTML = "Users";
//         $links[$i]->title = "Users";
//         /* check child links */
//         foreach ($this->menu_user() as $link) {
//             if ($link->is_active()) {
//                 $links[$i]->set_active_function(function () { return true; });
//             }
//         }


        
        return $links;
    }
    
    protected function bootstrap() {
        if ($id = $this->model()->request_key($_REQUEST)) {
            $this->model()->load($id);
        }
        $this->model()->attributes($_REQUEST);

        return true;
    }

    public function action_save() {
        if ($this->model()->recursive_save()) {
            return $this->model()->reload();
        }
        
        return false;
    }
    
    public function view_user() {
        $page = $this->page();
        $page['title'] = "Forms";
        $page['widget'] = "<form id=\"userform\" novalidate=\"novalidate\" method=\"post\" autocomplete=\"off\">";
        $page['widget'] .= $this->view_widget("Contact Information", "user.inc", array("nopadding" => true, "lightborder" => true));
        $page['widget'] .= $this->view_widget("Addresses", "address.inc", array("nopadding" => true, "lightborder" => true));
        $page['widget'] .= "</form>";
        echo $this->template($page);
    }

    public function view_index() {
        $page = $this->page();
        $page['title'] = "Users";
        $page['widget'] = "<form id=\"userform\" novalidate=\"novalidate\" method=\"post\" autocomplete=\"off\">";
        $page['widget'] .= $this->view_widget("Contact Information", "search.inc", array("nopadding" => true, "lightborder" => true));
        $page['widget'] .= "</form>";
        echo $this->template($page);
    }
    

    
}
