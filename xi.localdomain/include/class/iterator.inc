<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/
 
class iterator implements Iterator {

    private $_i;
    private $_data;
    private $_index;
    private $_count;
    
    public function __construct(&$data) {
        $this->_i = 0;
        $this->_data = &$data;
        $this->_index = array_keys($this->_data);
        $this->_count = count($this->_index);
    }

    public function rewind() {
        $this->_i = 0;
    }

    public function key() {
        return $this->_index[$this->_i];
    }

    public function current() {
        return $this->_data[$this->_index[$this->_i]];
    }
    
    public function next() {
        $this->_i++;
    }

    public function valid() {
        return isset($this->_index[$this->_i]);
    }
    
}
