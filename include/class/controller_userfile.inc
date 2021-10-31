<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_userfile extends controller {

    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "userfile/include/";
    }

    public function can_action_all() {
        return session::user_can_do(cando::LOGIN);
    }

    public function can_view_all() {
        return session::user_can_do(cando::LOGIN);
    }
    
    public function module_path() {
        return "/userfile/";
    }

    public function module_menu_title() {
        return "User Files";
    }
    
    public function action_savefile() {
        $user_filedrive = new user_filedrive();
        $file = $user_filedrive->save_upload_file("file");

        $this->set_ajax_output($file->json_description());
    }
    
    public function action_deletefile() {
        $f = new user_filedrive();
        if ($f->load($_REQUEST['user_filedriveid'])) {
            if ($f->userid == session::userid()) {
                $f->datedeleted = new expression("now()");
                return $f->save();
            }
        }
    }
    
    public function action_deleteallfiles() {
        $f = new user_filedrive();
        
        return $f->delete_user_files(session::userid());
    }
    
    public function action_trashfile() {
        $f = new user_filedrive();
        if ($f->load($_REQUEST['user_filedriveid'])) {
            if ($f->userid == session::userid()) {
                $f->datetrashed = new expression("now()");
                return $f->save();
            }
        }
        
        return false;
    }
    
    public function action_restorefile() {
        $f = new user_filedrive();
        if ($f->load($_REQUEST['user_filedriveid'])) {
            if ($f->userid == session::userid()) {
                $f->datetrashed = null;
                return $f->save();
            }
        }
        
        return false;
    }
    
    public function action_filedetails() {
        $f = new user_filedrive();
        if ($rows = $f->get_user_file_details($_REQUEST['user_filedriveid'], session::userid())) {
            $html = "<p class=\"nobreakword\">";
            foreach ($rows[0] as $key => $value) {
                $html .= "{$key}:&nbsp;{$value}<br />";
            }
            $html .= "</p>";
            
            return $this->set_ajax_output($html);
        }
        
        return false;
    }
    
    public function menu() {
        $i = 0;
        $links = array();
        $links[$i] = new html_a();
        $links[$i]->controller = $this;
        $links[$i]->view = "index";
        $links[$i]->innerHTML = "Files";
        $links[$i]->title = "Files";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = $this;
        $links[$i]->view = "trash";
        $links[$i]->innerHTML = "Trash";
        $links[$i]->title = "Trash";
        
        return $links;
    }
    
    public function view_trash() {
        $page = $this->page();
        $page['title'] = "Trash";
        $page['widget'] = $this->view_widget("Trash", "trash.inc", array("fullwidth" => true));
        echo $this->template($page);
    }

    public function view_index() {
        $page = $this->page();
        $page['title'] = "Manage Files";
        $page['widget'] = $this->view_widget("My Files", "upload.inc", array("fullwidth" => true));
        echo $this->template($page);
    }

    
}
