<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

abstract class model_abstract extends element {

    private $_intialized = false;
    private $_loaded = false;
    private $_dedupe = false;
    private $_schema = null;
    private $_alias = null;
    private $_attributes_orderid = 1;
    private $_readonly = false;
    private $_ancestry = null;
    
    protected $db;
    
    abstract public function tablename();
    abstract public function db();
    
    /* override parent */
    public function alias($alias = null) {
        if ($alias) {
            $this->_alias = $alias;
        }
        if ($this->_alias) {
            return $this->_alias;
        }
        
        return $this->tablename();
    }
    
    /* override parent */
    public function clean_attribute($key, $values) {
        if (is_array($values)) {
            foreach ($values as $index => $value) {
                $values[$index] = $this->clean_attribute(null, $values);
            }
            return $values;
        }
        
        return clean::html($values);
    }

    public function before_insert() {
        // execute before insert
        return true;
    }
    
    public function after_insert() {
        // execute after successful insert
        return true;
    }
    
    public function before_update() {
        // execute before update
        return true;
    }
    
    public function after_update() {
        // execute after successful update
        return true;
    }
    
    public function before_load() {
        // execute before load
        return true;
    }
    
    public function after_load() {
        // execute after successful load
        return true;
    }
    
    public function load_expressions() {
        return array();
    }
    
    public function next_id() {
        return null;
    }
    
    protected function _id($typeid) {
        if (!$this->_initialize()) {
            return false;
        }
        $id = new id();
        $id->idtypeid = $typeid;
        if ($id->save()) {
            return $id->id;
        }
        
        return false;
    }
    
    private function _initialize() {
        if (!$this->_intialized) {
            /* get database handle */
            $db = $this->db();
            /* set data fields if table exists */
            if (
                ($db) &&
                ($db instanceof sql) &&
                ($this->db = $db) &&
                ($this->_schema = $this->db->table_definition($this->tablename()))
            ) {
                $this->_intialized = true;
                foreach ($this->_schema['field'] as $fieldname => $value) {
                    $this->set($fieldname, null);
                }
            } else {
                dlog::decho("Oops, could not _initialize model");
            }
        }
        
        return $this->_intialized;
    }
    
    public function __construct() {
        $this->_initialize();
    }

    public function loaded() {
        return $this->_loaded;
    }

    public function is_field($fieldname) {
        if ($this->_initialize()) {
            return isset($this->_schema['field'][$fieldname]);
        }
        
        return null;
    }
    
    public function is_primary_key($fieldname) {
        if ($this->_initialize()) {
            return isset($this->_schema['primary_key'][$fieldname]);
        }
        
        return null;
    }
    
    public function table_definition() {
        if ($this->_initialize()) {
            return $this->_schema;
        }
        
        return null;
    }
    
    public function fields() {
        return $this->_schema['field'];
    }

    public function primary_key() {
        if ($this->_initialize() && !empty($this->_schema['primary_key'])) {
            return array_keys($this->_schema['primary_key']);
        }
        
        return array();
    }
    
    public function id() {
        $id = array();
        if ($this->loaded()) {
            foreach ($this->primary_key() as $key) {
                $id[$key] = $this->$key;
            }
            if (count($id) == 1) {
                return $this->$key;
            } else {
                return $id;
            }
        }
        
        return null;
    }
    
    public function request_key($request) {
        $id = array();
        foreach ($this->primary_key() as $key) {
            if (!empty($request[$this->alias()][$key])) {
                $value = $request[$this->alias()][$key];
                $id[$key] = $value;
            } else {
                return null;
            }
        }
        if (count($id) == 1) {
            return $value;
        }
        
        return $id;
    }
    
    public function truncate($field, $value) {
        if (
            (isset($this->_schema[$field]['data_type'])) &&
            (in_array($this->_schema[$field]['data_type'], array("varchar", "varchar2", "char")))
        ) {
            $value = substr($value, 0, $this->_schema[$field]['data_type']['character_maximum_length']);
        }
        
        return $value;
    }

    /* override parent */
    protected function set($fieldname, $data) {
        if ($this->_initialize()) {
            parent::set($fieldname, $data);
        }
    }
    
    public function save() {
        if (!$this->_initialize()) {
            return false;
        }
        $insert = false;
        $update = false;
        $success = false;
        $changed = $this->changed();
        if ($this->_loaded && !$changed) { /* nothing to do, just return */
            return true;
        } else if ($this->_loaded && $changed) { /* if loaded and something has changed do update */
            if (!$this->before_update()) {
                return false;
            }
            $update = true;
            $sql = "";
            $sql_where = "";
            /* save fields that changed */
            foreach ($changed as $key => $old_value) {
                if (!$this->is_field($key)) {
                    continue;
                }
                $new_value = $this->get($key);
                if ($new_value === null) {
                    $sql_value = "NULL";
                } else if ($new_value instanceof expression) { 
                    $sql_value = $new_value;
                } else {
                    $sql_value = $this->db->quote($this->truncate($key, $new_value));
                }
                $sql .= $this->db->iquote($key)  . "=" . $sql_value . ",";
            }
            /* nothing to save, return true */
            if (!$sql) {
                return true;
            }
            /* use primary key to update record */
            foreach ($this->_schema['primary_key'] as $key => $value) {
                $new_value = $this->get($key);
                if ($new_value instanceof expression) {
                    $sql_value = $new_value;
                } else {
                    $sql_value = $this->db->quote($new_value);
                }
                if ($sql_where) {
                    $sql_where .= " AND ";
                } else { 
                    $sql_where = " WHERE ";
                }
                $sql_where .= $this->db->iquote($key)  . "=" . $sql_value;
            }
            if ($sql_where) {
                $sql = "UPDATE " . $this->db->top(1) . " " . $this->db->iquote($this->tablename()) . " SET " . substr($sql, 0, -1);
                $sql .= $sql_where . " " . $this->db->limit(1);
                if ($this->db->write($sql)) { 
                    $success = true;
                }
            }
        } else { /* do insert */
            if (!$this->before_insert()) {
                return false;
            }
            $sql_fields = "";
            $sql_values = "";
            $sql_dedupe = "";
            $sequence = null;
            /* do sequence stuff */
            if (
                (count($this->_schema['primary_key']) == 1) &&
                ($sequence = $this->next_id())
            ) {
                foreach ($this->_schema['primary_key'] as $key => $value) {
                    if ($this->get($key) === null) {
                        if ($sequence instanceof expression) {
                            $sql_value = $sequence;
                        } else {
                            $sql_value = $this->db->quote($sequence);
                        }

                        $sql_values .= $sql_value . ",";
                        $sql_fields .= $this->db->iquote($key) . ",";
                    }
                }
            } else if ($sequence === false) { /* if false, we failed to get a sequence value */
                return false;
            }
            /* loop through all the fields */
            foreach ($changed as $key => $value) {
                if (!$this->is_field($key)) {
                    continue;
                }
                $new_value = $this->get($key);
                if ($new_value === null) {
                    $sql_value = "NULL";
                } else if ($new_value instanceof expression) { 
                    $sql_value = $new_value;
                } else { 
                    $sql_value = $this->db->quote($this->truncate($key, $new_value));
                }
                $sql_values .= $sql_value . ",";
                $sql_fields .= $this->db->iquote($key) . ",";
            } 
            /* dedupe values */
            if ($this->_dedupe) {
                foreach ($this->_dedupe as $key => $value) {
                    if ($sql_dedupe) { 
                        $sql_dedupe .= ") AND (";
                    }
                    if ($value instanceof expression) { 
                        $value = " " . $value;
                    } else {
                        $value = "=" . $this->db->quote($this->get($key)) . "";
                    }
                    $sql_dedupe .= $this->db->iquote($key) . $value;
                }
            }
            /* do insert */
            if ($sql_fields) {
                $insert = true;
                $sql_fields = " (" . substr($sql_fields, 0, -1) . ")";
                if ($sql_dedupe) {
                    $sql = "INSERT INTO " . $this->db->iquote($this->tablename()) . $sql_fields . "\n";
                    $sql .= "SELECT " . substr($sql_values, 0, -1) . "\n";
                    if ($this->db->dummy_table()) {
                        $sql .= "FROM " . $this->db->dummy_table() . "\n";
                    }
                    $sql .= "WHERE NOT EXISTS (SELECT " . $this->db->top(1) . " '1' FROM ";
                    $sql .= $this->db->iquote($this->tablename()) . " WHERE (" . $sql_dedupe . ") " . $this->db->limit(1) . ")";
                } else {
                    $sql_values = " (" . substr($sql_values, 0, -1) . ")";
                    $sql = "INSERT INTO " . $this->db->iquote($this->tablename()) . $sql_fields . " VALUES" . $sql_values;
                }
                $id = $this->db->insert($sql, $this->tablename());
                if ($id || $id === 0 || $id === "0") {
                    if ($id || ($id = $sequence)) {
                        if (count($this->_schema['primary_key']) == 1) {
                            foreach ($this->_schema['primary_key'] as $key => $value) {
                                if ($this->get($key) === null) {
                                    $this->set($key, $id);
                                }
                            }
                        }
                    }
                    $success = true;
                }
            }
        }
        if ($success) {
            $this->history(false);
            $this->_loaded = true;
            if ($insert) {
                $this->after_insert();
            } else if ($update) {
                $this->after_update();
            }
        }
        
        return $success;
    }
    
    public function sql() {
        return $this->db()->sql();
    }
    
    public function unload() {
        foreach ($this as $key => $value) {
            $this->set($key, null);
        }
        $this->history(false);
        $this->_loaded = false;
    }
    
    public function sql_where($id) {
        $return = $this->sql()->where()->false();
        if (empty($id) && $id !== 0 && $id !== "0") {
            return $return;
        }
        $keys = array();
        $key_count = count($this->_schema['primary_key']);
        foreach ($this->_schema['primary_key'] as $key => $value) {
            if (is_array($id)) {
                /* check we have all the primary key fields */
                if (!isset($id[$key])) {
                    return $return;
                }
                $value = $id[$key];
            }
            if ($key_count == 1) {
                $value = $id;
            }
            $keys[$this->alias()][$key] = $value;
        }
        
        return $this->sql()->where($keys); 
    }
    
    public function sql_load($id) {
    
        $sql = $this->sql()->
            select()->
                all($this)->
            from($this)->
            expression($this->sql_where($id))->
            limit(1);

        return $sql;
    }
    
    private function _ancestry() {
        if ($this->_ancestry === null) {
            $this->_ancestry = $this->sql_load(null)->ancestry();
//             dlog::decho(print_r($this->_ancestry, true));
        }
        
        return $this->_ancestry;
    }
    
    private function _instantiate($alias_model, $key, $set_this = false) {
        $ancestry = $this->_ancestry();
        $models = array();
        foreach ($key as $primary_key => $instance) {
            /* skip any null entries caused by left joins */
            if (!$primary_key) {
                continue;
            }
            $attributes = array();
            foreach ($instance as $field => $value) {
                if (is_array($value)) {
                    $split = explode(",", $field);
                    $attributes[$split[0]] = $this->_instantiate($field, $value);
                } else {
                    $attributes[$field] = $value;
                }
            }
            $split = explode(",", $alias_model);
            if ($set_this) {
                $this->before_load();
                $return = $this->_load_child_attributes($attributes);
                if ($return) {
                    if ($ancestry[$split[0]]['readonly']) {
                        $this->readonly();
                    }
                    $this->after_load();
                }
                return $return;
            } else {
                $m = new $split[1]();
                $m->before_load();
                if ($m->_load_child_attributes($attributes)) {
                    if ($ancestry[$split[0]]['readonly']) {
                        $m->readonly();
                    }
                    $m->after_load();
                    $models[] = $m;
                }
            }
        }
        
        return $models;
    }
    
    /*
        $foo[4]->userid = 4;
        $foo[4]->forename = 'des';
        $foo[4]->surname = 'jones';
        $foo[4]->user_usergroup['4,1']->userid = 4;
        $foo[4]->user_usergroup['4,1']->usergroupid = 1;
        $foo[4]->user_usergroup['4,1']->usergroup[1]->usergroupid = 1;
        $foo[4]->user_usergroup['4,1']->usergroup[1]->name = 'development';
        $foo[4]->user_usergroup['4,2']->userid = 4;
        $foo[4]->user_usergroup['4,2']->usergroupid = 2;
        $foo[4]->user_usergroup['4,2']->usergroup[2]->usergroupid = 2;
        $foo[4]->user_usergroup['4,2']->usergroup[2]->name = 'guest';
    */
    public function load($id) {
        $this->_loaded = false;
        if (!$this->_initialize()) {
            return false;
        }

        $return = false;
        $sql = $this->sql_load($id);
        if (($sql) && ($rows = $sql->hash())) {
//             dlog::decho(print_r($rows, true));
            foreach ($rows as $alias_model => $key) {
                $return = $this->_instantiate($alias_model, $key, true);
                break;
            }
        }
        
        return $return;
    }

    public function delete($id) {
        $return = false;
        if (!$this->_initialize()) {
            return $return;
        }
        $sql_where = $this->sql_where($id);
        if (!$sql_where) {
            return $return;
        }
        $sql = "delete " . $this->db->top(1) . " FROM " . $this->db->iquote($this->tablename()) . 
               $sql_where . " " . $this->db->limit(1);
        if ($this->db->write($sql)) {
            foreach ($this->_schema['field'] as $key => $value) {
                $this->set($key, null);
            }
            $this->history(false);
            $this->_loaded = false;
            $return = true;
        }
        
        return $return;
    }
    
    public function update($id) {
        $return = false;
        if (!$this->_initialize()) {
            return $return;
        }
        /* if nothing to do, just return */
        $changed = $this->changed();
        if (!$changed) {
            return 0;
        }
        if (!$this->before_update()) {
            return false;
        }
        $sql_where = $this->sql_where($id);
        if (!$sql_where) {
            return $return;
        }
        $sql = "";
        $sql_update = "";
        /* save fields that changed */
        foreach ($changed as $key => $old_value) {
            if (!$this->is_field($key)) {
                continue;
            }
            $new_value = $this->get($key);
            if ($new_value === null) {
                $sql_value = "NULL";
            } else if ($new_value instanceof expression) {
                $sql_value = $new_value;
            } else {
                $sql_value = $this->db->quote($new_value);
            }
            $sql_update .= $this->db->iquote($key)  . "=" . $sql_value . ",";
        }
        /* update record */
        $error = true;
        $affected_rows = 0;
        if ($sql_update) {
            $sql = "UPDATE " . $this->db->top(1) . " " . $this->db->iquote($this->tablename()) . " SET " . substr($sql_update, 0, -1);
            $sql .= $sql_where . " " . $this->db->limit(1);
            $affected_rows = $this->db->write_affected_rows($sql, $error);
        }
        if ($error) {
            return false;
        }
        if ($this->_loaded) {
            $this->history(false);
        }
        $this->after_update();
            
        return $affected_rows;
    }

    public function insert() {
        $return = false;
        if (!$this->_initialize()) {
            return $return;
        }
        /* if nothing to do, just return */
        $changed = $this->changed();
        if (!$changed) {
            return 0;
        }
        if (!$this->before_insert()) {
            return false;
        }
        $return = false;
        $sql = "";
        $sql_fields = "";
        $sql_values = "";
        /* save fields that changed */
        foreach ($changed as $key => $old_value) {
            if (!$this->is_field($key)) {
                continue;
            }
            $new_value = $this->get($key);
            if ($new_value === null) {
                $sql_value = "NULL";
            } else if ($new_value instanceof expression) {
                $sql_value = $new_value;
            } else {
                $sql_value = $this->db->quote($new_value);
            }
            $sql_values .= $sql_value . ",";
            $sql_fields .= $this->db->iquote($key) . ",";
        }
        /* do insert */
        if ($sql_fields) {
            $sql_fields = " (" . substr($sql_fields, 0, -1) . ")";
            $sql_values = " (" . substr($sql_values, 0, -1) . ")";
            $sql = "INSERT INTO " . $this->db->iquote($this->tablename()) . $sql_fields . " VALUES" . $sql_values;
            if ($id = $this->db->insert($sql, $this->tablename())) {
                if (count($this->_schema['primary_key']) == 1) {
                    foreach ($this->_schema['primary_key'] as $key => $value) {
                        if ($this->get($key) === null) {
                            $this->set($key, $id);
                        }
                    }
                }
                $return = true;
            } else if ($id === 0) {
                $return = true;
            }
        }
        
        if ($return) {
            $this->history(false);
            $this->_loaded = true;
            $this->after_insert();
        }

        return $return;
    }
    
    private function _recursive_save($model, $parent_keys) {
        $return = true;
        $model->attributes(array($model->alias() => $parent_keys));
        if ($model->save()) {
            $parent_keys = array();
            foreach($model->primary_key() as $key) {
                $parent_keys[$key] = $this->$key;
            }
            foreach ($model as $property => $values) {
                if (is_array($values)) {
                    foreach ($values as $value) {
                        if ($value instanceof model_abstract) {
                            if (!$this->_recursive_save($value, $parent_keys)) {
                                $return = false;
                            }
                        }
                    }
                }
            }
        } else {
            dlog::decho(get_class($model) . "->save() returned false");
            $return = false;
        }
        
        return $return;
    }
    
    public function recursive_save() {
        $return = false;
        if (!$this->_initialize()) {
            return $return;
        }
        /* if there isn't a transaction in progress, start one */
        $transaction = false;
        if (!$this->db->is_transaction()) {
            $transaction = true;
            $this->db->start_transaction();
        }
        $return = $this->_recursive_save($this, array());
        /* if we started a transaction, do commit or rollback */
        if ($transaction) {
            if ($return) {
                $this->db->commit();
            } else {
                $this->db->rollback();
            }
        }
        
        return $return;
    }
    
    public function reload() {
        if (!$this->_initialize()) {
            return false;
        }
        $id = null;
        $primary_keys = $this->primary_key();
        if (count($primary_keys) == 1) {
            $id = $this->get($primary_keys[0]);
        } else {
            $id = array();
            foreach ($primary_keys as $key) {
                $id[$key] = $this->get($key);
            }
        }
        
        return $this->load($id);
    }
    
    public function dedupe($fieldname, $value = true) {
        if (!$this->_initialize()) {
            return false;
        }
        if (isset($this->_schema['field'][$fieldname])) {
            if ($value === false) {
                unset($this->_dedupe[$fieldname]);
            } else {
                $this->_dedupe[$fieldname] = $value;
            }
            return true;
        }
    
        return false;
    }

    public function attributes_orderid($orderid = null) {
        if ($orderid > 0) {
            $this->_attributes_orderid = $orderid;
        }
        
        return $this->_attributes_orderid;
    }
    
    /* this function should never be used */
    public function _load_child_attributes($attributes) {
        $this->_loaded = false;
        if (!$this->_initialize() || !is_array($attributes) || !count($attributes)) {
            return false;
        }
        foreach ($attributes as $key => $value) {
            parent::set($key, $value);
        }
        $this->history(false);
        $this->_loaded = true;
        
        return true;
    }
    
    /* 
     * todo: check join keys, don't just use primary keys
     */
    public function _attributes($parent, $children, $values) {
        $ancestry = $this->_ancestry();
        foreach ($ancestry as $alias => $ancestor) {
            if ($alias == $children && $ancestor['parent'] == $parent) {
                if (!is_array($this->$children)) {
                    $this->$children = array();
                }
                $i = 1;
                foreach ($values as $value) {
                    if (is_array($value)) {
                        $found = false;
                        foreach ($this->$children as $child) {
                            foreach ($ancestor['primary_keys'] as $primary_key) {
                                if (!empty($child->$primary_key) && $child->$primary_key == $value[$primary_key]) {
                                    $found = true;
                                    $child->attributes_orderid($i);
                                    $child->attributes(array($child->alias() => $value));
                                }
                            }
                        }
                        if (!$found) {
                            $c = new $ancestor['model']();
                            $c->attributes_orderid($i);
                            $c->attributes(array($c->alias() => $value));
                            $this->$children = array_merge($this->$children, array($c));
                        }
                        $i++;
                    }
                }
            }
        }
    }
    
    public function attributes($values, $allowed = null) {
        $return = false;
        if (!empty($values[$this->alias()]) && is_array($values[$this->alias()])) {
            foreach ($values[$this->alias()] as $key => $value) {
                if (!$allowed || in_array($key, $allowed)) {
                    if (is_array($value)) {
                        $this->_attributes($this->alias(), $key, $value);
                    } else {
                        $value = $this->clean_attribute($key, $value);
                        if ($value === '' || $value == "null") {
                            $value = null;
                        }
                        $this->$key = $value;
                        $return = true;
                    }
                }
            }
        }

        return $return;
    }
    
    public function lookup($keyalias = "id", $field = "name") {
        $rows = array();
        $fields = array_keys($this->fields());
        if (!in_array($field, $fields) || !in_array("datedeleted", $fields)) {
            return $rows;
        }
        $cando = new expression("null");
        if (in_array("candoid", $fields)) {
            $cando = "candoid";
        }
        
        $tmp = $this->sql()->
            select()->
                field($this, array_pop($this->primary_key()), $keyalias)->
                field($this, $field, $field)->
                field($this, $cando, "candoid")->
            from($this)->
            where()->field($this, "datedeleted")->expression("is null")->
            order_by()->field($this, $field)->
            fetch_all_cached();
            
        if ($tmp) {
            foreach ($tmp as $t) {
                $add = false;
                if ($cando instanceof expression || session::user_can_do($t["candoid"])) {
                    $add = true;
                }
                if ($add) {
                    $row = array();
                    $row[$keyalias] = $t[$keyalias];
                    $row[$field] = $t[$field];
                    $rows[] = $row;
                }
            }
        }

        return $rows;
    }

}
