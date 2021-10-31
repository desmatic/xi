<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_development extends controller {
    
    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "development/include/";
    }
    
    public function module_path() {
        return "/development/";
    }
    
    public function can_action_all() {
        return session::agent_can_do(cando::DEVELOPER);
    }

    public function can_view_all() {
        return session::agent_can_do(cando::DEVELOPER);
    }
    
    public function menu() {
        $i = 0;
        $links = array();
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "Forms";
        $links[$i]->view = "form";
        $links[$i]->controller = $this;
        $i++;
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "Des Workspace";
        $links[$i]->view = "desworkspace";
        $links[$i]->controller = $this;
        $i++;
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "Abhi";
        $links[$i]->view = "abhi";
        $links[$i]->controller = $this;
        
        return $links;
    }

    public function widget_widgettest() {
        return "this is a simple widget";
    }
    
    public function action_logrequest() {
        dlog::dblog(print_r($_REQUEST, true) . print_r($_FILES, true), true);
    }
    
    public function view_form() {
        $page = $this->page();
        $page['title'] = "Forms";
//         $page['widget'] = "";
        echo $this->template($page);
    }
    
    public function view_abhi() {
        $page = $this->page();
        $page['title'] = "abhi";
        $page['widget'] = $this->view_widget("Abhi", "abhi.inc");
        echo $this->template($page);
    }
    
    public function view_desworkspace() {
        $page = $this->page();
        $page['title'] = "Des Workspace";
        $page['widget'] = $this->view_widget("test", "desworkspace.inc", array("fullwidth" => true, "nopadding" => true));
        echo $this->template($page);
    }
    
    public function view_index() {
        $page = $this->page();
        $page['title'] = "Development";
        $page['widget'] = $this->view_widget("Development", "index.inc");
        echo $this->template($page);
    }
    
}
