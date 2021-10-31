<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class article_company extends model {

    public function tablename() {
        return "article_company";
    }
    
    public function dset_datedeleted($input) {
        if ($input == "sysdate") {
            $this->set("datedeleted", new expression($this->db()->sysdate()));
        } else {
            $this->set("datedeleted", clean::sql_date_time($input));
        }
    }

}
