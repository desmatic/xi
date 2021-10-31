<?php

abstract class mysql extends sql implements nsql {

    public function nconnect($host, $user, $pass, $schema = null) {
        return mysql_connect($host, $user, $pass);
    }

    public function nclose(&$dbh) {
        return mysql_close($dbh);
    }
    
    public function nselect_schema($name, &$dbh) {
        return mysql_select_db($name, $dbh);
    }
    
    public function niquote($value, &$dbh) {
        return "`" . $value . "`";
    }
    
    public function nquote($value, &$dbh) {
        return "'" . mysql_real_escape_string($value, $dbh) . "'";
    }
    
    public function nquery($sql, &$dbh) {
        return mysql_query($sql, $dbh);
    }
    
    public function naffected_rows(&$sth, &$dbh = null) {
        return mysql_affected_rows($dbh);
    }
    
    public function ninsert_id(&$dbh, $tablename) {
        return mysql_insert_id($dbh);
    }
    
    public function nfetch_array(&$sth) {
        return mysql_fetch_assoc($sth);
    }
    
    public function nfetch_array_numeric(&$sth) {
        return mysql_fetch_row($sth);
    }
    
    public function nerror(&$dbh) {
        return mysql_error($dbh);
    }
    
    public function dummy_table() {
        return "dual";
    }
    
    public function nschema_sql($schema) {
        return "
            select 
                table_name, 
                column_name,
                if(is_nullable = 'YES', 1, 0) as is_nullable,
                data_type, 
                character_maximum_length,
                numeric_precision,
                substring(column_key, 1, 1) as column_key
            
            from information_schema.columns as isc
            
            where table_schema='{$schema}'
            
            order by table_name, ordinal_position
        ";
    }
    
    public function nreplace_sql($tablename, $fields, $values) {
        
    }
    
    public function sysdate($seconds = null) {
        $sql = "sysdate()";
        if ($seconds = clean::negative_integer($seconds)) {
            $sign = "+";
            if ($seconds < 0) {
                $sign = "-";
                $seconds = clean::integer($seconds);
            }
                
            $sql .= " {$sign} INTERVAL {$seconds} SECOND";
        }
                    
        return $sql;
    }
    
    public function limit($i, $j = null) {
        $sql = "";
        if ($i = clean::integer($i))
            $sql = "LIMIT {$i}";
        
        if ($j = clean::integer($j)) 
            $sql .= ", {$j}";
                    
        return $sql;
    }

}

?>