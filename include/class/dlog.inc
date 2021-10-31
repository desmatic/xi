<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class dlog {

    private static $_debug;
    
    public static function enable($enable = true) {
        if ($enable) {
            self::$_debug = "!!Logging enabled!!\n";
        } else {
            self::$_debug = null;
        }
    }
    
    public static function is_enabled() {
        return self::$_debug !== null;
    }
        
    public static function decho($input, $error = false) {
        if (($error) || (self::$_debug)) {
            self::$_debug .= substr($input, 0, 32000) . "\n";
        }
    }

    public static function log($input, $error = false) {
        if (($error) || (self::$_debug !== null)) {
            openlog("adds", LOG_PID, LOG_LOCAL0);
            syslog(LOG_WARNING, substr($input, 0, 220));
            closelog();
        }
    }
        
    public static function dblog($input, $error = false) {
        if (($error) || (self::$_debug !== null)) {
            $l = new dblog();
            $l->message = $input;
            $l->userid = session::userid();
            $l->backtrace = print_r(debug_backtrace(), true);
            return $l->save();
        }
        
        return true;
    }

    public static function get_decho_buffer() {
        return self::$_debug;
    }
    
}
