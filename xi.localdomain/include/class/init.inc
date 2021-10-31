<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/
 
class init {

    private static $_cli;
    private static $_cgi;
    private static $_start_time;
    
    public static function is_cli() {
        return self::$_cli;
    }
    
    public static function is_cgi() {
        return self::$_cgi;
    }
    
    public function cgi() {
        self::$_cgi = true;
        
        /* initiate php session */
        session::start();
        
        /* enable debugging for developers */
        if (session::user_can_do(cando::VIEW_DEBUG) || define::DEBUG) {
            dlog::enable(true);
//             register_shutdown_function("shutdown_function");
        }
        
        return true;
    }

    public function cli() {
        self::$_cli = true;
        
        return true;
    }
    
    public static function bootstrap() {
        self::$_start_time = microtime(true);
        $i = new init();
        $i->global_functions();
        if ($_SERVER['REQUEST_URI']) {
            $return = $i->cgi();
        } else {
            $return = $i->cli();
        }
        
        return $return;
    }

    protected function global_functions() {
        if (!function_exists("foo")) {
            function foo() {
                return "bar";
            }
        }

        if (!function_exists("shutdown_function")) {
            function shutdown_function() {
                $error = error_get_last();
                if ($error) {
                    echo "<h2>Oops! There's a boo boo in your code!!</h2>\n";
                    echo "<pre>\n " . print_r($error, true) . "\n</pre>";
                }
            }
        }

        return true;
    }

    public static function get_start_time() {
        return self::$_start_time;
    }
}
