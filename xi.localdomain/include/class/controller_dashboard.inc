<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_dashboard extends controller {
    
    private $_model_password = null;

    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "dashboard/include/";
    }

    public function can_action_all() {
        return session::user_can_do(cando::LOGIN);
    }

    public function can_view_all() {
        return session::user_can_do(cando::LOGIN);
    }
    
    public function module_path() {
        return "/dashboard/";
    }

    public function module_menu_title() {
        return "Dashboard";
    }
    
    public function model() {
        return session::user();
    }

    public function model_password() {
        if ($this->_model_password === null) {
            $this->_model_password = new user();
        }
        
        return $this->_model_password;
    }

    public function action_save() {
        session::user()->attributes($_REQUEST);
        if (session::user()->recursive_save()) {
            session::user()->reload();
            dlog::decho("saved user");
        } else {
            dlog::decho("could not save");
        }
        
        return false;
    }
    
    public function action_change_password() {
        if ($this->model_password()->load(session::user()->userid)) {
            $this->model_password()->attributes($_REQUEST, array("currentpassword", "newpassword", "newpasswordcheck"));
            if (self::$nook['password_updated'] = $this->model_password()->change_password()) {
                $this->_model_password = new user();
            }
        }

        return self::$nook['password_updated'];
    }
    
    public function menu() {
        $i = 0;
        $links = array();
        $links[$i] = new html_a();
        $links[$i]->controller = $this;
        $links[$i]->view = "calendar";
        $links[$i]->innerHTML = "Calendar";
        $links[$i]->title = "Calendar";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = $this;
        $links[$i]->view = "account";
        $links[$i]->innerHTML = "Account";
        $links[$i]->title = "Account";
        
        return $links;
    }
    
    public function can_view_calendar() {
        return session::agent_can_do(cando::DEVELOPER);
    }
    
    public function view_calendar() {
        $page = $this->page();
        $page['title'] = "Your Calendar";
        $page['widget'] = $this->view_widget("Calendar", "calendar.inc", array("fullwidth" => 1));
        echo $this->template($page);
    }
    
    public function view_account() {
        $page = $this->page();
        $page['title'] = "Account Information";
        $page['widget'] = "<form id=\"userpasswordform\" novalidate=\"novalidate\" method=\"post\" autocomplete=\"off\">";
        $page['widget'] .= $this->view_widget_shared("Password", "password.inc", array("nopadding" => true, "lightborder" => true, "model" => $this->model_password()->hash()));
        $page['widget'] .= "</form>";
        $page['widget'] .= "<form id=\"userform\" novalidate=\"novalidate\" method=\"post\" autocomplete=\"off\">";
        $page['widget'] .= $this->view_widget_shared("Contact Information", "user.inc", array("nopadding" => true, "lightborder" => true));
        $page['widget'] .= $this->view_widget_shared("Addresses", "address.inc", array("nopadding" => true, "lightborder" => true));
        $page['widget'] .= "</form>";
        echo $this->template($page);
    }

    public function view_index() {
        echo $this->view_account();
    }

    
}
