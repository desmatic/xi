<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class sql_statement {

    private $_db;
    private $_sql;
    private $_select_clause;
    private $_select_clause_new;
    private $_select_clause_count;
    private $_comma_clause;
    private $_comma_clause_new;
    private $_order_by_clause;
    private $_order_by_clause_new;
    private $_set_clause_new;
    private $_loaded;
    private $_ifields;
    private $_fields;
    private $_models;
    private $_join_on;
    
    public function __construct($db) {
        $this->_sql = "";
        $this->_db = &$db;
        $this->_select_clause = null;
        $this->_select_clause_new = null;
        $this->_select_clause_first = null;
        $this->_comma_clause = null;
        $this->_comma_clause_new = null;
        $this->_order_by_clause = null;
        $this->_order_by_clause_new = null;
        $this->_set_clause_new = true;
        $this->_loaded = null;
        $this->_fields = array();
        $this->_ifields = 0;
    }
    
    public function __toString() {
        return $this->_sql . "\n";
    }
    
    public function select() {
        $this->_sql .= "SELECT";
        $this->_comma_clause = true;
        $this->_comma_clause_new = true;
        /* earmark the first select statement */
        if ($this->_select_clause_first === null) {
            $this->_select_clause_first = true;
        } else {
            $this->_select_clause_first = false;
        }
        
        return $this;
    }
    
    public function field($alias, $field, $as = null) {
        $aliasname = $alias instanceof model ? $alias->alias() : $alias;
        /* select clauses only */
        if ($this->_comma_clause) {
            if ($this->_comma_clause_new)  {
                $this->_comma_clause_new = false;
            } else {
                $this->_sql .= ",";
            }
            $this->_sql .= "\n\t";
        }
        if ($aliasname && !$field instanceof expression) {
            $this->_sql .= $this->_db->iquote($aliasname) . ".";
        }
        if ($field instanceof expression) {
            $this->_sql .= $field;
        } else {
            $this->_sql .= $this->_db->iquote($field);
        }
        if ($as) {
            $this->_sql .=  " AS `{$as}`";
        }
        /* track first select clause for loading fields later */
        if ($alias instanceof model && $this->_select_clause_first) {
            $this->_fields[$this->_ifields]["model"] = get_class($alias);
            $this->_fields[$this->_ifields]["alias"] = $alias->alias();
            $this->_fields[$this->_ifields]["field"] = $as ? $as : $field;
            $this->_ifields++;
        }
        
        return $this;
    }
    
    public function all($alias = null) {
        /* track first select clause for loading fields later */
        if ($alias instanceof model && $this->_select_clause_first) {
            foreach($alias->fields() as $field => $type) {
                $this->_fields[$this->_ifields]["model"] = get_class($alias);
                $this->_fields[$this->_ifields]["alias"] = $alias->alias();
                $this->_fields[$this->_ifields]["field"] = $field;
                $this->_ifields++;
            }
        }
        
        $prefix = $alias instanceof model ? $alias->alias() : $alias;
        if ($prefix) {
            if ($this->_comma_clause_new)  {
                $this->_comma_clause_new = false;
            } else {
                $this->_sql .= ",";
            }
            $this->_sql .= "\n\t";
            $this->_sql .= $this->_db->iquote($prefix) . ".*";
        } else {
            $this->_sql .= " *";
        }
        
        if ($alias instanceof model) {
            $expressions = $alias->load_expressions();
            if (is_array($expressions)) {
                foreach ($expressions as $key => $value) {
                    if ($value instanceof expression) {
                        $this->field($alias, $value, $key);
                    }
                }
            }
        }

        return $this;
    }
    
    public function expression($expression, $alias = null) {
        if ($this->_comma_clause) {
            if ($this->_comma_clause_new)  {
                $this->_comma_clause_new = false;
            } else {
                $this->_sql .= ",";
            }
            $this->_sql .= "\n\t{$expression}";
            
            if ($alias) {
                $this->_sql .= " AS " . $this->_db->iquote($alias);
            }
        } else {
            $this->_sql .= " {$expression} ";
        }

        return $this;
    }
    
    public function e($expression, $alias = null) {
        return $this->expression($expression, $alias);
    }
    
    public function eq() {
        $this->_sql .= "=";

        return $this;
    }
    
    public function in($in) {
        if (!is_array($in)) {
            $in = explode(",", $in);
        }
        $sql = "";
        foreach($in as $value) {
            if ($value instanceof expression) {
                $sql .= $value . ",";
            } else {
                $sql .= $this->_db->quote($value) . ",";
            }
        }
        $this->_sql .= " in (" . substr($sql, 0, -1) . ")";

        return $this;
    }

    public function is_null() {
        $this->_sql .= " is null";

        return $this;
    }
    
    public function is_not_null() {
        $this->_sql .= " is not null";

        return $this;
    }

    public function false() {
        $this->_sql .= " false";
        
        return $this;
    }
    
    public function p() {
        $this->_sql .= "(";

        return $this;
    }
    
    public function q() {
        $this->_sql .= ")";

        return $this;
    }

    public function quote($value) {
        $this->_sql .= $this->_db->quote($value);
        
        return $this;
    }
    
    private function _set_model_info($tablealias, $table, $readonly) {
        if (!isset($this->_models[$tablealias])) {
            $this->_models[$tablealias]['model'] = get_class($table);
            $this->_models[$tablealias]['readonly'] = $readonly;
            if ($this->_models[$tablealias]['primary_keys'] = $table->primary_key()) {
                $primary_key_indexes = array();
                foreach ($this->_models[$tablealias]['primary_keys'] as $primary_key) {
                    foreach ($this->_fields as $index => $field) {
                        if ($field['alias'] == $tablealias && $field['field'] == $primary_key) {
                            $primary_key_indexes[] = $index;
                        }
                    }
                }
                $this->_models[$tablealias]['primary_key_indexes'] = $primary_key_indexes;
            } 
        }
    }
    
    public function from($table, $alias = null) {
        $this->_select_clause_first = false;
        $this->_comma_clause = false;
        if ($table instanceof model) {
            $tablealias = $table->alias();
            $tablename = $table->tablename();
            $readonly = $table->is_readonly();
            $this->_set_model_info($tablealias, $table, $readonly);
        } else {
            $tablename = $table;
        }
        $tablealias = $alias === null ? $tablename : $alias;
        $this->_sql .= "\n\nFROM " . $this->_db->iquote($tablename) . " AS " . $this->_db->iquote($tablealias);
        
        return $this;
    }
    
    public function join($table, $alias = null) {
        $this->_select_clause_first = false;
        $this->_comma_clause = false;
        if ($table instanceof model) {
            $tablealias = $table->alias();
            $tablename = $table->tablename();
            $readonly = $table->is_readonly();
            $this->_join_on = $tablealias;
            $this->_set_model_info($tablealias, $table, $readonly);
        } else {
            $tablename = $table;
        }
        $tablealias = $alias === null ? $tablealias : $alias;
        $this->_sql .= "\n\nJOIN " . $this->_db->iquote($tablename) . " AS " . $this->_db->iquote($tablealias);
        
        return $this;
    }
    
    public function left_join($table, $alias = null) {
        $this->_select_clause_first = false;
        $this->_comma_clause = false;
        if ($table instanceof model) {
            $tablealias = $table->alias();
            $tablename = $table->tablename();
            $readonly = $table->is_readonly();
            $this->_join_on = $tablealias;
            $this->_set_model_info($tablealias, $table, $readonly);
        } else {
            $tablename = $table;
        }
        $tablealias = $alias === null ? $tablealias : $alias;
        $this->_sql .= "\n\nLEFT JOIN " . $this->_db->iquote($tablename) . " AS " . $this->_db->iquote($tablealias);
        
        return $this;
    }
    
    public function using($field) {
        $this->_sql .= "\nUSING(" . $this->_db->iquote($field) . ")";
        
        return $this;
    }
    
    public function on($alias1 = null, $field1 = null, $alias2 = null, $field2 = null) {
        $this->_sql .= "\nON ";
        $models = 0;
        if ($alias1 !== null && $field1 !== null) {
            $tablename1 = $alias1;
            if ($alias1 instanceof model) {
                $tablename1 = $alias1->alias();
                $models++;
            }
            $this->_sql .= $this->_db->iquote($tablename1) . "." . $this->_db->iquote($field1);
            if ($alias2 !== null && $field2 !== null) {
                $tablename2 = $alias2;
                if ($alias2 instanceof model) {
                    $tablename2 = $alias2->alias();
                    $models++;
                }
                $this->_sql .= "=" . $this->_db->iquote($tablename2) . "." . $this->_db->iquote($field2);
            }
        }
        if ($models == 2) {
            if ($tablename1 == $this->_join_on) {
                $this->_models[$tablename1]['parent'] = $tablename2;
                $this->_models[$tablename2]['children'][$tablename1][$field2] = $field1;
            } else {
                $this->_models[$tablename2]['parent'] = $tablename1;
                $this->_models[$tablename1]['children'][$tablename2][$field1] = $field2;
            }
        }
        $this->_join_on = null;
        
        return $this;
    }
    
    public function update($table, $alias = null) {
        $this->_select_clause_first = false;
        $this->_comma_clause = true;
        $this->_comma_clause_new = true;
        if ($table instanceof model) {
            $tablealias = $table->alias();
            $tablename = $table->tablename();
        } else {
            $tablename = $table;
        }
        $tablealias = $alias === null ? $tablename : $alias;
        $this->_sql .= "UPDATE " . $this->_db->iquote($tablename) . " AS " . $this->_db->iquote($tablealias);
        
        return $this;
    }
    
    public function set($alias, $field) {
        $this->_comma_clause = false;
        $aliasname = $alias instanceof model ? $alias->alias() : $alias;
        if ($this->_set_clause_new) {
            $this->_set_clause_new = false;
            $this->_sql .= " SET\n\t";
            $this->_sql .= $this->_db->iquote($aliasname) . ".";
            $this->_sql .= $this->_db->iquote($field);
        } else {
            $this->_sql .= ",\n\t";
            $this->_sql .= $this->_db->iquote($aliasname) . ".";
            $this->_sql .= $this->_db->iquote($field);
        }
        
        return $this;
    }
    
    private function _and($model = null, $alias = null) {
        $and = "";
        if ($model instanceof model) {
            $alias = $alias === null ? $model->tablename() : $alias;
            foreach ($model->changed() as $key => $old_value) {
                if (!$model->is_field($key)) {
                    continue;
                }
                $value = $model->$key;
                if ($value instanceof expression) {
                    $value = " " . $value;
                } else if ($value === null) {
                    $value = " is NULL";
                } else {
                    $value = "=" . $this->_db->quote($value);
                }
                $and .= $this->_db->iquote($alias) . "." . $this->_db->iquote($key) . $value . " AND ";
            }
        } else if (is_array($model)) {
            foreach ($model as $alias => $field) {
                foreach ($field as $name => $value) {
                    if ($value instanceof expression) {
                        $value = " " . $value;
                    } else if ($value === null) {
                        $value = " is NULL";
                    } else {
                        $value = "=" . $this->_db->quote($value);
                    }
                    $and .= $this->_db->iquote($alias) . "." . $this->_db->iquote($name) . $value  . " AND ";
                }
            }
        }
        $and = "(" . substr($and, 0, -5) . ")";
        
        return $and;
    }
    
    /*
     * $model = array(
     *   "user" => array("userid" => 4),
     *   "user" => array("email" => "nobody@email.com"),
     *   "group" => array("groupid" => "6"),
     * );
     */
    public function where($model = null, $alias = null) {
        $this->_select_clause_first = false;
        $this->_comma_clause = false;
        $this->_sql .= "\n\nWHERE ";
        if ($model || is_array($model)) {
            $this->_sql .= $this->_and($model, $alias);
        }
        
        return $this;
    }

    public function a($model = null, $alias = null) {
        $this->_sql .= "\nAND ";
        if ($model) {
            $this->_sql .= $this->_and($model, $alias);
        }
    
        return $this;
    }
    
    public function o() {
        $this->_sql .= "\nOR "; 
    
        return $this;
    }
    
    public function order_by() {
        $this->_select_clause_first = false;
        $this->_comma_clause = true;
        $this->_comma_clause_new = true;
        $this->_sql .= "\n\nORDER BY ";
    
        return $this;
    }
    
    public function group_by() {
        $this->_select_clause_first = false;
        $this->_comma_clause = true;
        $this->_comma_clause_new = true;
        $this->_sql .= "\n\nGROUP BY ";
    
        return $this;
    }
    
    public function desc() {
        $this->_sql .= " DESC"; 
    
        return $this;
    }
    
    public function limit($i, $j = null) {
        $this->_sql .= "\n" . $this->_db->limit($i, $j);
        
        return $this;
    }
    
    public function fetch_all_numeric(&$error = false) {
        return $this->_db->fetch_all_numeric($this->_sql, $error);
    }
    
    public function fetch_all(&$error = false) {
        return $this->_db->fetch_all($this->_sql, $error);
    }
    
    public function update_all(&$error = false) {
        return $this->_db->write_affected_rows($this->_sql, $error);
    }
    
    public function is_cached() {
        return $this->_db->is_cached($this->_sql);
    }

    public function fetch_all_cached($time = sql::QUERY_CACHE_TIME, &$error = false) {
        return $this->_db->fetch_all_cached($this->_sql, $time, $error);
    }
    
    public function sysdate($seconds = null) {
        $this->_sql .= $this->_db->sysdate($seconds);
        
        return $this;
    }
    
    public static function hash_array_push(array &$array, array $hash_key_array, &$value = null) {
        $key = "";
        foreach ($hash_key_array as $i) {
            $key .= "['{$i}']";
        }
        $eval = "\$hdl=&\$array{$key}; return true;";
        if ($key && eval($eval)) {
            $hdl = $value;
        } else {
            return 0;
        }
        
        return count($array);
    }
    
    /* 
        get the key path by recursing through all the models till you reach
        the root model
                
        $foo['user'][4]['userid'] = 4;
        $foo['user'][4]['forename'] = 'des';
        $foo['user'][4]['surname'] = 'jones';
        $foo['user'][4]['user_usergroup']['4,1']['userid'] = 4;
        $foo['user'][4]['user_usergroup']['4,1']['usergroupid'] = 1;
        $foo['user'][4]['user_usergroup']['4,1']['usergroup'][1]['usergroupid'] = 1;
        $foo['user'][4]['user_usergroup']['4,1']['usergroup'][1]['name'] = 'development';
        $foo['user'][4]['user_usergroup']['4,2']['userid'] = 4;
        $foo['user'][4]['user_usergroup']['4,2']['usergroupid'] = 2;
        $foo['user'][4]['user_usergroup']['4,2']['usergroup'][2]['usergroupid'] = 2;
        $foo['user'][4]['user_usergroup']['4,2']['usergroup'][2]['name'] = 'guest';
    */
    private function _get_hash_key_array($alias, array $hashkeys, &$path) {
        $path[] = $hashkeys[$alias];
        $path[] = $alias . "," . $this->_models[$alias]['model'];
        if (isset($this->_models[$alias]['parent'])) {
            $this->_get_hash_key_array($this->_models[$alias]['parent'], $hashkeys, $path);
        }
    }
    
    private function _add_field(&$hash, $hashkeys, $alias, $field, $value, $foobar = null) {
        /* get the hash key array */
        $hash_key_array = array();
        $this->_get_hash_key_array($alias, $hashkeys, $hash_key_array);
        $hash_key_array = array_reverse($hash_key_array);
        $hash_key_array[] = $field;
        
        /* push the hash key value pair onto hash */
        $this->hash_array_push($hash, $hash_key_array, $value);
    }
    
    public function hash() {
        $output = array();
        $rows = $this->fetch_all_numeric($this->_sql);
        if (!$rows) {
            return $output;
        }
        foreach ($rows as $row) {
            $hashkeys = array();
            foreach ($this->_models as $alias => $model) {
                $hashkey = "";
                foreach ($model['primary_key_indexes'] as $primary_key) {
                    $hashkey .= "," . $row[$primary_key];
                }
                $hashkey = substr($hashkey, 1);
                $hashkeys[$alias] = $hashkey;
            }
            foreach ($this->_fields as $index => $field) {
                $this->_add_field($output, $hashkeys, $field['alias'], $field['field'], $row[$index]);
            }
        }
        
        return $output;
    }
    
    public function ancestry() {
        return $this->_models;
    }

}
