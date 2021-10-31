<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_root extends controller {

    public function can_view_thumb() {
        return true;
    }

    public function view_thumb() {
        $f = new user_filedrive();
        if (
            !empty($_REQUEST['md5sum']) &&
            !empty($_REQUEST['bytes']) &&
            ($image = $f->get_user_file_image($_REQUEST['md5sum'], $_REQUEST['bytes'], $_REQUEST['size']))
        ) {
            header('Content-Type: image/jpeg');
            echo $image;
            exit(0);
        } else {
            $page = $this->page();
            $page['title'] = "Welcome";
            $page['widget'] = "<h2>Welcome to xi</h2>";
            echo $this->template($page);
        }
    }

    public function can_view_index() {
        return true;
    }
    
    public function view_index() {
        if (!$this->authenticated()) {
            echo $this->view_login();
        } else {
            $page = $this->page();
            $page['title'] = "Welcome to ADDS";
            $a = new varticle();
            foreach ($a->article_list(session::companyid()) as $row) {
                $page['widget'] .= $this->view_widget_shared($row['title'], "article.inc", array("model" => $row));
            }
            echo $this->template($page);
        }
    }
    
}
