<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

abstract class html_abstract extends element {

    private $_active_function;
    private $_can_do;
    
    private static $_url_path;

    abstract protected function _string();
    
    public function __construct($active_function = null) {
        if ($active_function === null) {
            $active_function = "self::default_active_function";
        }
        $this->set_active_function($active_function);
    }
    
    public function can_do() {
        if ($this->_can_do === null) {
            if (!empty($this->can_do) && !session::user_can_do($this->can_do)) {
                $this->_can_do = false;
            } else {
                $this->_can_do = true;
            }
        }
        
        return $this->_can_do;
    }
    
    public function string() {
        if ($this->can_do()) {
            return $this->_string();
        }
        
        return "";
    }

    public function __toString() {
        return $this->string();
    }
    
    public static function get_url_path() {
        if (self::$_url_path === null) {
            $url = parse_url($_SERVER['REQUEST_URI']);
            $url = clean::file_no_extension($url['path']);
            self::$_url_path = str_replace("-", "_", $url);
        }
        
        return self::$_url_path;
    }
    
    public function relative_path_active_function() {
        $url = self::get_url_path();
        $href = clean::file_no_extension($this->href);
        if (
            ($href == $url) ||
            (!strpos($this->href, "/") && clean::basename($href) == clean::basename($url))
        ) {
            return true;
        }
        
        return false;
    }
    
    public function default_active_function() {
        $url = strtolower(self::get_url_path());
        $href = parse_url($this->href);
        $href = strtolower(str_replace("-", "_", $href['path']));
        if (
            ($href == $url) ||
            (clean::file_no_extension($href) == clean::file_no_extension($url))
        ) {
            return true;
        }
        
        return false;
    }

    public function set_active_function($function) {
        if (!empty($function) && is_callable($function)) {
            $this->_active_function = $function;
            return true;
        }
        
        return false;
    }
    
    public function is_active() {
        if (!empty($this->_active_function)) {
            return call_user_func($this->_active_function);
        }
        
        return false;
    }
    
    public static function array2url($input, $page = null) {
        $output = "";
        if (is_array($input)) {
            foreach ($input as $key => $value) {
                /* todo: make recursive */
                if (!is_array($value)) {
                    if ($output) {
                        $output .= "&amp;";
                    }
                    $output .=  urlencode($key) . "=" . urlencode($value);
                }
            }
        }
        if ($page) {
            $output = strpos($page, "?") ? $page . "?" . $output :  $page . "&amp;" . $output; 
        }

        return $output;
    }
    
}
