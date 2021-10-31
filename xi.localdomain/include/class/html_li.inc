<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class html_li extends html_abstract {

    protected function _string() {
        /* add active class if requested url matches href */
        $url = self::get_url_path();
        $href = clean::file_no_extension($this->href);
        $class = $this->class;
        if ($this->innerHTML instanceof html_a && $this->innerHTML->is_active()) {
            $class = empty($class) ? "active" : $class . " active";
        }
        $output = "<li";
        $output .= $this->id ? " id=\"{$this->id}\"" : "";
        $output .= $class ? " class=\"{$class}\"" : "";
        $output .= ">";
        $output .= $this->innerHTML;
        $output .= "</li>";
        
        return $output;
    }

}
