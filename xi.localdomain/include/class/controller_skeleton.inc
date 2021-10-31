<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_skeleton extends controller {
    
    private $_model;

    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "skeleton/include/";
    }

    public function can_action_all() {
        return session::agent_can_do(cando::DEVELOPER);
    }

    public function can_view_all() {
        return session::agent_can_do(cando::DEVELOPER);
    }
    
    public function module_path() {
        return "/skeleton/";
    }

    public function module_menu_title() {
        return "Skeleton";
    }
    
    public function model() {
        if ($this->_model === null) {
            $this->_model = new skeleton();
        }
        
        return $this->_model;
    }

    protected function bootstrap() {
        if ($id = $this->model()->request_key($_REQUEST)) {
            $this->model()->load($id);
        }
        $this->model()->attributes($_REQUEST);
    }
    
    function can_savestuff() {
        return false;
    }
    
    function action_savestuff() {
        // save stuff
    }
    
    public function action_save() {
        //return $this->model()->save_reload();
    }
    
    public function view_hello_world() {
        echo "hello world\n";
    }
    
    /* any links that should consider their parent link foobar */
    public function menu_foobar() {
        $i = 0;
        $links = array();
        $links[$i] = new html_a();
        $links[$i]->href = $this->href("foo");
        $i++;
        $links[$i] = new html_a();
        $links[$i]->href = $this->href("bar");
        
        return $links;
    }

    public function menu() {
        $i = 0;
        $links = array();
        $links[$i] = new html_a();
        $links[$i]->controller = $this;
        $links[$i]->view = "index";
        $links[$i]->innerHTML = "Hello World";
        $links[$i]->title = "Hello World";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = $this;
        $links[$i]->view = "foobar";
        $links[$i]->innerHTML = "Foobar";
        $links[$i]->title = "Foobar";
        /* check child links */
        foreach ($this->menu_foobar() as $link) {
            if ($link->is_active()) {
                $links[$i]->set_active_function(function () { return true; });
            }
        }
        
        return $links;
    }
    
    public function view_foo() {
        $page = $this->page();
        $page['title'] = "foo";
        $page['widget'] = $this->view_widget("foo of foobar", "foo.inc");
        echo $this->template($page);
    }
    
    public function view_bar() {
        $page = $this->page();
        $page['title'] = "bar";
        $page['widget'] = $this->view_widget("bar of foobar", "bar.inc");
        echo $this->template($page);
    }
    
    public function view_foobar() {
        $page = $this->page();
        $page['title'] = "FooBar";
        $page['widget'] = $this->view_widget_form("FooBar", "foobar.inc");
        echo $this->template($page);
    }

    public function view_index() {
        $page = $this->page();
        $page['title'] = "Hello World";
        $page['widget'] = $this->view_widget("Hello World", "index.inc");
        echo $this->template($page);
    }

    
}
