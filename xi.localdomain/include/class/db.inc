<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/
 
class db extends mypdo {

    private static $_dbhandle;
    
    protected function dbhandle() {
        if (self::$_dbhandle === null) {
            self::$_dbhandle = new stdClass();
        }
        
        return self::$_dbhandle;
    }
    
    protected function hosts() {
    
        return array(
            array(
                "hostname" => "localhost",
                "username" => "xi",
                "password" => sql::encrypt("Toh9Ohv0Uev2eeth"),
                "schema" => "xi",
                "mode" => "rw"
            )
        );
    }
    
}
