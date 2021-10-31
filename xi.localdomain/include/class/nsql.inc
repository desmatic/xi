<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

interface nsql {
    
    public function nconnect($host, $user, $pass, $schema);
    public function nclose(&$dbh);
    public function nerror(&$dbh);
    public function nquote($value, &$dbh);
    public function niquote($value, &$dbh);
    public function nfetch_array(&$sth);
    public function nfetch_array_numeric(&$sth);
    public function ninsert_id(&$dbh, $sequence);
    public function nquery($sql, &$dbh);
    public function nstart_transaction(&$dbh);
    public function ncommit(&$dbh);
    public function nrollback(&$dbh);
    public function naffected_rows(&$sth, &$dbh);
    public function nselect_schema($name, &$dbh);
    public function nschema_sql($schema);
    public function nreplace_sql($tablename, $fields, $values);
    public function dummy_table();
    public function sysdate($interval);
    
}
