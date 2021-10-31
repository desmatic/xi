<?php

abstract class mypdo extends dbpdo {

    public function niquote($value, &$dbh) {
        return "`" . $value . "`";
    }

    public function dummy_table() {
        return "dual";
    }
    
    public function nschema_sql($schema) {
        return "
            SELECT 
                table_name as table_name, 
                column_name as column_name,
                if(is_nullable = 'YES', 1, 0) as is_nullable,
                data_type as data_type, 
                character_maximum_length as character_maximum_length, 
                numeric_precision as numeric_precision,
                substring(column_key, 1, 1) as column_key
            
            FROM information_schema.columns as isc
            
            WHERE table_schema='{$schema}'
            
            ORDER BY table_name, ordinal_position
        ";
    }
    
    public function nreplace_sql($tablename, $fields, $values) {
        
    }
    
    public function sysdate($seconds = null) {
        $sql = "now()";
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
        $i = clean::integer($i);
        $j = clean::integer($j);
        $sql = "LIMIT";
        if ($j) {
            $sql .= " {$i}, {$j}";
        } else {
            $sql .= " {$i}";
        }
                    
        return $sql;
    }

}
