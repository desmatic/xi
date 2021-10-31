<?php

/********************
 *
 * Author:      Des Jones
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class cache {

    private static $_hdl;

    public static function connect() {
        if (self::$_hdl === null) {
            try {
                if (class_exists('Memcached')) {
                    self::$_hdl = new Memcached();
                    self::$_hdl->addServer('localhost', 11211);
                }
            } catch (Exception $e) {
                self::$_hdl = null;
            }
        }

        return self::$_hdl;
    }

    public static function hashkey($input) {
        $input = trim($input);
        $len = strlen($input);
        if ($len < 168) {
            return base64_encode($input);
        }
        
        return  $len . sha1($input);
    }

    public static function set($key, $value, $seconds = 20) {
        if (self::connect()) {
            return self::$_hdl->set(self::hashkey($key), $value, $seconds);
        }

        return null;
    }

    public static function get($key) {
        if (self::connect()) {
            return self::$_hdl->get(self::hashkey($key));
        }

        return null;
    }
}

?>