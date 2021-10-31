<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_smartadmin extends controller {
    
    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "smartadmin/include/";
    }
    
    public function module_path() {
        return "/smartadmin/";
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
        $links[$i]->innerHTML = "Basics";
        $links[$i]->view = "index";
        $links[$i]->controller = $this;
        $i++;
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "Tables";
        $links[$i]->view = "tables";
        $links[$i]->controller = $this;
        $i++;
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "Windows";
        $links[$i]->view = "windows";
        $links[$i]->controller = $this;
        $i++;
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "Documentation";
        $links[$i]->href = $this->href("documentation/index");
//         $links[$i]->can_do = array(cando::DEVELOPER);
        $links[$i]->target = "_blank";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "SmartAdmin Source 1.52";
        $links[$i]->href = $this->href("smartadmin-1.52/index");
//         $links[$i]->can_do = array(cando::DEVELOPER);
        $links[$i]->target = "_blank";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->innerHTML = "SmartAdmin Source 1.7.0";
        $links[$i]->href = $this->href("smartadmin-1.7.0/index");
//         $links[$i]->can_do = array(cando::DEVELOPER);
        $links[$i]->target = "_blank";
        
        return $links;
    }

    public function action_modal() {
        $this->set_ajax_output($this->view("modal.inc"));
    }
    
    public function action_window() {
        $variables['footer'] = '<button type="button" class="btn btn-default" data-dismiss="modal">Cancel</button>';
        $html = template::modal("mymodal", "my title", "my body", $variables);
        
        return $this->set_ajax_output($html);
    }
    
    public function view_windows() {
        $page = $this->page();
        $page['title'] = "Windows";
        $page['widget'] = $this->view_widget("Windows", "windows.inc");
        echo $this->template($page);
    }

    public function view_tables() {
        $page = $this->page();
        $page['title'] = "Tables";
        $page['widget'] = $this->view("tables.inc");
        echo $this->template($page);
    }
    
    public function view_datatables() {
        $page = $this->page();
        $page['title'] = "Data Tables";
        $page['widget'] = $this->view("datatables.inc");
        echo $this->template($page);
    }

    public function view_index() {
        $page = $this->page();
        $page['title'] = "Development";
        $page['widget'] = $this->view("index.inc");
        echo $this->template($page);
    }
    
}
