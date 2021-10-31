<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_article extends controller {
    
    private $_model = null;

    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "article/include/";
    }

    public function can_action_all() {
        return session::agent_can_do(cando::ADD_ARTICLE_CONTROLLER_ROOT, $this->userid) || session::agent_can_do(cando::EDIT_ARTICLE_CONTROLLER_ROOT);
    }

    public function can_view_all() {
        return session::agent_can_do(cando::ADD_ARTICLE_CONTROLLER_ROOT) || session::agent_can_do(cando::EDIT_ARTICLE_CONTROLLER_ROOT);
    }
    
    public function module_path() {
        return "/article/";
    }

    public function module_menu_title() {
        return "Article";
    }
    
    public function model() {
        if ($this->_model === null) {
            $this->_model = new article();
        }
        
        return $this->_model;
    }

    protected function bootstrap() {
        if ($id = $this->model()->request_key($_REQUEST)) {
            $this->model()->load($id);
        }
        
        return $this->model()->attributes($_REQUEST);
    }
    
    public function action_delete() {
        if (!empty($_REQUEST['delete']) && $this->model()->loaded()) {
            $this->model()->delete = $_REQUEST['delete'];
            return $this->model()->save();
        }
        
        return false;
    }
    
    public function action_pause() {
        if (!empty($_REQUEST['pause']) && $this->model()->loaded()) {
            $this->model()->pause = $_REQUEST['pause'];
            return $this->model()->save();
        }
        
        return false;
    }
    
    public function can_action_pin() {
        return session::agent_can_do(cando::PIN_ARTICLE_CONTROLLER_ROOT);
    }
    
    public function action_pin() {
        if (!empty($_REQUEST['pin']) && $this->model()->loaded()) {
            $this->model()->pin = $_REQUEST['pin'];
            return $this->model()->save();
        }
        
        return false;
    }

    public function action_save() {
        self::$nook['controller_article.save'] = false;
        if ($this->model()->recursive_save()) {
            $this->model()->save_upload_picture();
            self::$nook['controller_article.save'] = $this->model()->reload();
        }

        return self::$nook['controller_article.save'];
    }
    
    public function can_action_savepoll() {
        return session::agent_can_do(cando::AGENT);
    }
    
    public function action_savepoll() {
        $a = new article_user_answer();
        if (
            $this->model()->loaded() && 
            ($answer = article::parse_answers($this->model()->answers, $_REQUEST['answer']))
        ) {
            $id = array("articleid" => $this->model()->articleid, "userid" => session::userid());
            if (!$a->load($id)) {
                $a->articleid = $this->model()->articleid;
                $a->userid = session::userid();
            }
            $a->answer = $answer;
            if ($a->save()) {
                $this->set_ajax_output(template::success("Success!", "You voted for {$answer}."));
                return true;
            }
        }
        $this->set_ajax_output(template::danger("Oops!", "Sorry, your vote couldn't be saved."));
        
        return false;
    }
    
    public function menu() {
        $i = 0;
        $links = array();
        $links[$i] = new html_a();
        $links[$i]->controller = $this;
        $links[$i]->view = "article";
        $links[$i]->innerHTML = "Post Article";
        $links[$i]->title = "Post Welcome Article";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = $this;
        $links[$i]->view = "search";
        $links[$i]->innerHTML = "Search Articles";
        $links[$i]->title = "Search Articles";
        
        return $links;
    }
    
    public function can_view_article() {
        return session::agent_can_do(cando::AGENT);
    }
    
    public function view_article() {
        $title = "New Article";
        if (!empty($this->model()->title)) {
            $title = "ARTICLE" . $this->model()->id() . " " . session::local_datetime($this->model()->datecreated) . " - " . $this->model()->title;
        }
        $page = $this->page();
        $page['title'] = "Post Welcome Article";
        $page['widget'] = $this->view_widget($title, "article.inc", array("fullwidth" => 1));
        echo $this->template($page);
    }
    
    public function can_view_search() {
        return session::agent_can_do(cando::AGENT);
    }
    
    public function view_search() {
        $page = $this->page();
        $page['title'] = "Search Articles";
        $page['widget'] = $this->view("search.inc");
        echo $this->template($page);
    }

    public function view_index() {
        echo $this->view_article();
    }

    
}
