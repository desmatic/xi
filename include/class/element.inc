<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/


class element implements IteratorAggregate,Countable {

    private $_initialise;
    private $_data;
    private $_history;
    private $_error;
    private $_readonly;
    
    private function _initialise() {
        if (!$this->_initialise) {
            $this->_initialise = true;
            $this->_data = array();
            $this->_history = array();
            $this->_error = array();
            $this->_readonly = false;
        }
        
        return true;
    }

    protected function set($fieldname, $data) {
        $this->_initialise();
        if (!$this->_readonly) {
            /* save original */
            if (!in_array($fieldname, array_keys($this->_history))) {
                $this->_history[$fieldname] = $data;
            }
            /* set data */
            $this->_data[$fieldname] = $data;
        }
    }
    
    public function __set($fieldname, $data) {
        $set='dset_'.$fieldname;
        $setCamelCase='dset'.$fieldname;
        if (!$this->_readonly) {
            if (method_exists($this, $set)) {
                $this->$set($data);
            } else if (method_exists($this, $setCamelCase)) {
                $this->$setCamelCase($data);
            } else {
                $this->set($fieldname, $data);
            }
        }
    }

    protected function get($fieldname) {
        $this->_initialise();
        if (isset($this->_data[$fieldname])) {
            return $this->_data[$fieldname];
        }
    
        return null;
    }
    
    public function __get($fieldname) {
        $get='dget_'.$fieldname;
        $getCamelCase='dget'.$fieldname;
        if (method_exists($this, $get)) {
            return $this->$get($fieldname);
        } else if (method_exists($this, $getCamelCase)) {
            return $this->$getCamelCase($fieldname);
        } else {
            return $this->get($fieldname);
        }
    
        return null;
    }

    protected function history($reset = null) {
        $this->_initialise();
        if ($reset !== null) {
            $this->_history = array();
            foreach($this->_data as $key => $value) {
                $this->_history[$key] = $value;
            }
        }

        return $this->_history;
    }
    
    public function changed() {
        $this->_initialise();
        $changed = array();
        foreach($this->_history as $key => $value) {
            $new_value = $this->get($key);
            if (($value != $new_value) || ((!$new_value) && ($value !== $new_value))) {
                /* skip numeric zero and string zero changes */
                if (
                    (($value === "0") && ($new_value === 0)) || 
                    (($value === 0) && ($new_value === "0"))
                ) {
                    continue;
                }

                $changed[$key] = $value;
            }
        }
        
        return $changed;
    }

    public function __isset($fieldname) {
        $this->_initialise();
        
        return isset($this->_data[$fieldname]);
    }
    
    public function count() {
        $this->_initialise();
        
        return count($this->_data);
    }
    
    public function getIterator() {
        $this->_initialise();
        
        return new element_iterator($this->_data);
    }
    
    public function properties() {
        $this->_initialise();
        $variables = array();
        foreach($this as $key => $value) {
            $variables[$key] = true;
        }
        $class = new ReflectionClass(get_class($this));
        foreach($class->getMethods(ReflectionMethod::IS_PUBLIC) as $method) {
            foreach(array("dget_", "dget") as $prefix) {
                $name = $method->name;
                $substr = substr($name, 0, strlen($prefix));
                $variable = substr($name, strlen($prefix));
                if ($substr == $prefix) {
                    $variables[$variable] = true;
                    break;
                }
            }
        }
        
        return array_keys($variables);
    }
    
    public function clean_attribute($value) {
        return $value;
    }
    
    public function attributes($values, $allowed = null) {
        $return = false;
        if (!empty($values[$this->alias()]) && is_array($values[$this->alias()])) {
            foreach($values[$this->alias()] as $key => $value) {
                if (!$allowed || in_array($key, $allowed)) {
                    $value = $this->clean_attribute($value);
                    if ($value === '') {
                        $value = null;
                    }
                    $this->$key = $value;
                    $return = true;
                }
            }
        }

        return $return;
    }
    
    public function alias() {
        return get_class($this);
    }
    
    protected function get_error($fieldname = null) {
        $this->_initialise();
        if (!$fieldname) {
            return $this->_error;
        } else if (isset($this->_error[$fieldname])) {
            return $this->_error[$fieldname];
        }
        
        return null;
    }
    
    protected function error($fieldname, $data) {
        $this->_initialise();
        if ($data === null) {
            unset($this->_error[$fieldname]);
        } else {
            $this->_error[$fieldname] = $data;
        }
    }
    
    protected function error_reset() {
        $this->_error = array();
    }
    
    /* recurse through model arrays and xml each one */
    private function _array_xml($models, $tabindent) {
        $tab = "";
        $output = "";
        for ($i = 0; $i < $tabindent; $i++) {
            $tab .= "\t";
        }
        foreach ($models as $model) {
            $name = $model->alias();
            $classname = get_class($model);
            $output .= "{$tab}<{$name} model=\"{$classname}\">\n";
            foreach($model->properties() as $property) {
                $value = $model->$property;
                if (is_array($value)) {
                    $output .= "{$tab}\t<{$property} model=\"array()\">\n";
                    $output .= $this->_array_xml($value, $tabindent + 2);
                    $output .= "{$tab}\t</{$property}>\n";
                } else {
                    if (($value === null) || ($value === false)) {
                        $output .= "{$tab}\t<{$property} />\n";
                    } else {
                        $output .= "{$tab}\t<{$property}>" . htmlspecialchars($value) . "</{$property}>\n";
                    }
                }
            }
            $output .= "{$tab}</{$name}>\n";
        }
        
        return $output;
    }
    
    public function xml() {
        return $this->_array_xml(array($this), 0);
    }
    
    /* recurse through model arrays and hash each one */
    private function _array_hash($models) {
        $hashes = array();
        foreach ($models as $model) {
            if ($model instanceof element) {
                $hash = $model->hash();
                foreach ($model->properties() as $property) {
                    if (is_array($property)) {
                        $hash[$property] = $this->_array_hash($property);
                    }
                }
                $hashes[] = $hash;
            }
        }
        
        return $hashes;
    }
    
    public function hash() {
        $key = $this->alias();
        $properties = array();
        foreach($this->properties() as $property) {
            if (!empty($this->_error[$property])) {
                $properties[$key][$property]['error'] = $this->_error[$property];
            } else {
                $properties[$key][$property]['error'] = null;
            }
            if (is_array($this->$property)) {
                $properties[$key][$property]['value'] = $this->_array_hash($this->$property);
            } else {
                $properties[$key][$property]['value'] = $this->$property;
            }
        }
        
        return $properties;
    }
    
    public function json() { 
        return json_encode($this->hash());
    }
    
    public function is_readonly() {
        return $this->_readonly;
    }
    
    public function readonly() {
        $this->_readonly = true;
        
        return $this->_readonly;
    }
}