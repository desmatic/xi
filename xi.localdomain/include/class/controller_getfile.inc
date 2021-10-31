<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class controller_getfile extends controller {

    public function can_action_all() {
        return session::user_can_do(cando::LOGIN);
    }
    
    public function action_userfile() {
        $user_filedrive = new user_filedrive();
        $f = $user_filedrive->get_user_file($_REQUEST['md5sum'], $_REQUEST['bytes'], session::userid());
        if ($f) {
            header("Content-Type: " . filedrive::abbreviate_mimetype($f[0]["mimetype"]));
            $this->set_ajax_output(base64_decode($f[0]["file"]));
        }
    }
    
}
