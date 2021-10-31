<?php

abstract class dbpdo extends sql implements nsql {

    private $_exception = null;

    public function nconnect($host, $user, $pass, $schema) {
        try {
            $dbh = new PDO("mysql:host={$host};dbname={$schema}", $user, $pass);
        } catch(PDOException $e) {
            $this->_exception = $e;
            $dbh = null;
        }
        
        return $dbh;
    }

    public function nclose(&$dbh) {
        $dbh = null;
        return true;
    }
    
    public function nselect_schema($name, &$dbh) {
        return true;
    }
    
    public function nquote($value, &$dbh) {
        return $dbh->quote($value);
    }
    
    public function nquery($sql, &$dbh) {
        return $dbh->query($sql);
    }
    
    public function naffected_rows(&$sth, &$dbh = null) {
        return $sth->rowCount();
    }
    
    public function ninsert_id(&$dbh, $sequence = null) {
        return $dbh->lastInsertId($sequence);
    }
    
    public function nfetch_array(&$sth) {
        return $sth->fetch(PDO::FETCH_ASSOC);
    }
    
    public function nfetch_array_numeric(&$sth) {
        return $sth->fetch(PDO::FETCH_NUM);
    }
    
    public function nerror(&$dbh) {
        if ($dbh) {
            return print_r($dbh->errorInfo(), true);
        } else if ($this->_exception) {
            return $this->_exception->getMessage();
        }
        
        return "";
    }

}
