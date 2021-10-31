<?php

abstract class odbc extends isql implements insql {

    public function nconnect($host, $user, $pass) {
        return odbc_connect($host, $user, $pass);
    }

    public function nclose(&$dbh) {
        return odbc_close($dbh);
    }
    
    public function nselect_schema($name, &$dbh) {
        return true;
    }
    
    public function nescape(&$value, &$dbh) {
        return mysql_escape_string($value);
    }
    
    public function nquery($sql, &$dbh) {
        return odbc_exec($dbh, $sql);
    }
    
    public function ninsert_id(&$dbh, $tablename) {
        return null;
    }
    
    public function nfetch_array(&$sth) {
        return odbc_fetch_array($sth);
    }
    
    public function nerror(&$dbh) {
        return odbc_errormsg($dbh);
    }
    
    public function dummy_table() {
        return "";
    }
    
    public function nschema_sql($schema) {
        return "";
    }
    
    public function nreplace_sql($tablename, $fields, $values) {
        return "";
    }
    
    public function sysdate($seconds = null) {
        return "";
    }
    
    public function limit($i, $j = null) {
        return "";
    }
    
    public function top($i) {           
        return "";
    }
    
}

?>