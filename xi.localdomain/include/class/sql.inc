<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/
 
abstract class sql {

    const QUERY_CACHE_TIME = "20";
    
    private static $_total_query_time;
    
    abstract protected function dbhandle();

    private static function _base64key() {
        return base64_encode("Dc1gyFTWHb0Bmmw40ZWhSzU42p0KmMsk");
    }

    protected static function encrypt($input) {
        return session::encrypt($input, self::_base64key());
    }

    protected static function decrypt($input) {
        return session::decrypt($input, self::_base64key());
    }

    private function _array_randomize($input) {
        $keys = array();
        $output = array();
        if (
            (is_array($input)) &&
            ($keys = array_keys($input)) &&
            (shuffle($keys))
        ) {
            foreach ($keys as $key) {
                $output[$key] = $input[$key];
            }
        }
        
        return $output;
    }

    protected function log($input, $error = false) {
        dlog::decho($input, $error);
    }

    public function next_id($tablename) {
        return null;
    }
    
    public function top($i) {
        return "";
    }

    public function schema_definition() {
        if (empty($this->dbhandle()->schema)) {
            $this->dbhandle()->schema = $this->fetch_schema_definition();
        }
        
        return $this->dbhandle()->schema;
    }
    
    public function schema() {
        if (!$this->connect()) {
            return false;
        }
        
        return $this->dbhandle()->hosts[$this->dbhandle()->read_host]['schema'];
    }
    
    public function schema_sql() {
        return $this->nschema_sql($this->schema());
    }

    public function add_host($hostname = "localhost", $username = "root", $password = "", $schema = "system", $mode = "rw") {
        $hostname = trim($hostname);
        $username = trim($username);
        $host = base64_encode($username . "@" . $hostname);
        $this->dbhandle()->hosts[$host]["hostname"] = $hostname;
        $this->dbhandle()->hosts[$host]["username"] = $username;
        $this->dbhandle()->hosts[$host]["password"] = $password;
        $this->dbhandle()->hosts[$host]["schema"] = $schema;
        $this->dbhandle()->hosts[$host]["read"] = false;
        $this->dbhandle()->hosts[$host]["write"] = false;
        switch (strtolower(trim($mode))) {
	  case "rw":
	      $this->dbhandle()->hosts[$host]["read"] = true;
	      $this->dbhandle()->hosts[$host]["write"] = true;
	  case "w":
	      $this->dbhandle()->hosts[$host]["write"] = true;
	      break;
	  case "r":
	  default:
	      $this->dbhandle()->hosts[$host]["read"] = true;
        }

        return true;
    }

    public function del_host($hostname = "localhost", $username = "root", $mode = "rw") {
        $hostname = trim($hostname);
        $username = trim($username);
        $host = base64_encode($username . "@" . $hostname);
        if (in_array($host, $this->hosts) == false) { 
            return false;
        }
        switch (strtolower(trim($mode))) {
        case "w":
            if ($this->dbhandle()->hosts[$host]["write"] == false) {
                return false;
            }
            if ($this->dbhandle()->hosts[$host]["read"] == true) {
                $this->dbhandle()->hosts[$host]["write"] = false;
            } else {
                unset($this->dbhandle()->hosts[$host]);
            }
            break;
        case "r":
            if ($this->dbhandle()->hosts[$host]["read"] == false) {
                return false;
            }
            if ($this->dbhandle()->hosts[$host]["write"] == true) {
                $this->dbhandle()->hosts[$host]["read"] = false;
            } else {
                unset($this->dbhandle()->hosts[$host]);
            }
            break;
        case "rw":
        default:
            unset($this->dbhandle()->hosts[$host]);
        }
        
        return true;
    }

    public function connect($write = false) {
        if (empty($this->dbhandle()->hosts)) {
            foreach ($this->hosts() as $host) {
                $this->add_host($host['hostname'], $host['username'], $host['password'], $host['schema'], $host['mode']);
            }
        }
        if ($write) {
            if (!empty($this->dbhandle()->write_connection)) {
                return true;
            } else if (
                (!empty($this->dbhandle()->read_connection)) &&
                ($this->dbhandle()->hosts[$this->dbhandle()->read_host]['write'])
            ) {
                $this->dbhandle()->write_connection = $this->dbhandle()->read_connection;
                return true;
            }
        } else {
            if (!empty($this->dbhandle()->read_connection)) {
                return true;
            } else if (
                (!empty($this->dbhandle()->write_connection)) &&
                ($this->dbhandle()->hosts[$this->dbhandle()->write_host]['read'])
            ) {
                $this->dbhandle()->read_connection = $this->dbhandle()->write_connection;
                return true;
            }
        }
        $hosts = $this->_array_randomize($this->dbhandle()->hosts);
        foreach ($hosts as $key => $host) {
            if (($write) && ($host["write"])) {
                $this->dbhandle()->write_connection = $this->nconnect($host["hostname"], $host["username"], self::decrypt($host["password"]), $host['schema']);
                if ($this->dbhandle()->write_connection) {
                    $this->dbhandle()->write_host = $key;
                    if ($this->nselect_schema($host["schema"], $this->dbhandle()->write_connection)) {
                        return true;
                    } else {
                        $this->log($this->nerror($this->dbhandle()->write_connection), true);
                    }
                } else {
                    $this->log($this->nerror($this->dbhandle()->write_connection), true);
                }
            } else if ((!$write) && ($host["read"])) {
                $this->dbhandle()->read_connection = $this->nconnect($host["hostname"], $host["username"], self::decrypt($host["password"]), $host['schema']);
                if ($this->dbhandle()->read_connection) {
                    $this->dbhandle()->read_host = $key;
                    if ($this->nselect_schema($host["schema"], $this->dbhandle()->read_connection)) {
                        return true;
                    } else {
                        $this->log($this->nerror($this->dbhandle()->read_connection), true);
                    }
                } else {
                    $this->log($this->nerror($this->dbhandle()->read_connection), true);
                }
            }
        }
        
        return false;
    }

    public function close() {
        $w = true;
        $r = true;
        if ($this->dbhandle()->write_connection && ($w = $this->nclose($this->dbhandle()->write_connection))) {
            $this->dbhandle()->write_connection = null;
        }
        
        if ($this->dbhandle()->read_connection && ($r = $this->nclose($this->dbhandle()->read_connection))) {
            $this->dbhandle()->read_connection = null;
        }
        
        return $r && $w;
    }

    public function query($sql, $write = false, &$error = false) {
        if (!$this->connect($write)) {
            $error = true;
            return false;
        }
        
        if ($write) {
            $prefix = "sql write: ";
            $connection = "write_connection";
        } else {
            $prefix = "sql read: ";
            $connection = "read_connection";
        }
        $this->log($prefix . "\n" . $sql);
        $start_time = microtime(true);
        if (($sth = $this->nquery($sql, $this->dbhandle()->$connection))) {
            $error = false;
        } else {
            $this->log($this->nerror($this->dbhandle()->$connection), true);
            $this->log(print_r(debug_backtrace(), true), true);
            $error = true;
        }
        $end_time = microtime(true);
        $query_time = $end_time - $start_time;
        $this->log("execution time: {$query_time}\n");
        if (self::$_total_query_time === null) {
            self::$_total_query_time = $query_time;
        } else {
            self::$_total_query_time += $query_time;
        }
        
        return $sth;
    }

    public function select($sql, &$error = false) {
        if (!$this->connect(false)) {
            $error = true;
            return false;
        }

        return $this->query($sql, false, $error);
    }

    public function fetch_array(&$sth) {
        return $this->nfetch_array($sth);
    }
    
    public function fetch_array_numeric(&$sth) {
        return $this->nfetch_array_numeric($sth);
    }
    
    public function fetch_all_numeric($sql, &$error = false) {
        $rows = array();
        if ($sth = $this->query($sql)) {
            while ($row = $this->fetch_array_numeric($sth)) {
                $rows[] = $row;
            }
        } else {
            $error = true;
        }

        return $rows;
    }

    public function fetch_all($sql, &$error = false) {
        $rows = array();
        if ($sth = $this->query($sql)) {
            while ($row = $this->fetch_array($sth)) {
                $rows[] = $row;
            }
        } else {
            $error = true;
        }

        return $rows;
    }
    
    public function is_cached($sql) {
        if ($c = cache::get($sql)) {
            return $c;
        }
        
        return null;
    }

    public function fetch_all_cached($sql = "", $time = SELF::QUERY_CACHE_TIME, &$error = false) {
        if ($time > 0 && ($c = cache::get($sql))) {
            $this->log("SQL query cache {$time} seconds:\n {$sql}");
            return $c;
        }
        if ($rows = $this->fetch_all($sql, $error)) {
            if ($time > 0) {
                cache::set($sql, $rows, $time);
            }
        }

        return $rows;
    }
    
    public function quote($value, &$error = false) {
        if (!$this->connect(false)) {
            $error = true;
            return null;
        }
        
        return $this->nquote($value, $this->dbhandle()->read_connection);
    }
    
    public function iquote($value, &$error = false) {
        if (!$this->connect(false)) {
            $error = true;
            return null;
        }
        
        return $this->niquote($value, $this->dbhandle()->read_connection);
    }

    public function insert($sql, $table_name = null, &$error = false) {
        if ($this->query($sql, true)) {
            return $this->ninsert_id($this->dbhandle()->write_connection, $table_name);
        }
        $error = true;
        
        return false;
    }
    
    public function write($sql, &$error = false) {
        if (!$this->connect(true)) {
            $error = true;
            return false;
        }

        return $this->query($sql, true, $error);
    }
    
    public function write_affected_rows(&$sql, &$error = false) {
        if (!$this->connect(true)) {
            $error = true;
            return false;
        }
        $affected_rows = 0;
        if ($sth = $this->query($sql, true, $error)) {
            $affected_rows = $this->naffected_rows($sth, $this->dbhandle()->write_connection);
        }

        return $affected_rows;
    }
    
    public function fetch_schema_definition() {
        $definition = array();
        $rows = $this->fetch_all_cached($this->schema_sql());
        if ($rows) {
            foreach ($rows as $row) {
                $definition[$row['table_name']]['field'][$row['column_name']]['is_nullable'] = $row['is_nullable'];
                $definition[$row['table_name']]['field'][$row['column_name']]['data_type'] = $row['data_type'];
                $definition[$row['table_name']]['field'][$row['column_name']]['character_maximum_length'] = $row['character_maximum_length'];
                $definition[$row['table_name']]['field'][$row['column_name']]['numeric_precision'] = $row['numeric_precision'];
                $definition[$row['table_name']]['field'][$row['column_name']]['column_key'] = $row['column_key'];
                if (strtolower(substr($row['column_key'], 0, 1)) == "p") {
                    $definition[$row['table_name']]['primary_key'][$row['column_name']] = true;
                }
            }
        } else {
            dlog::decho("Oops, could not load schema!");
        }
        
        return $definition;
    }
    
    public function table_definition($tablename) {
        $schema = $this->schema_definition();
        
        $definition = array();
        if (!empty($schema[$tablename])) {
            $definition = $schema[$tablename];
        }
        
        return $definition;
    }
    
    public function sql() {
        return new sql_statement($this);
    }
    
    public static function get_total_query_time() {
        return self::$_total_query_time;
    }
    
    public function nstart_transaction(&$dbh = null) {
        $error = null;
        $this->query("start transaction", true, $error);
        
        return !$error;
    }
    
    public function ncommit(&$dbh = null) {
        $error = null;
        $this->query("commit", true, $error);
        
        return !$error;
    }
    
    public function nrollback(&$dbh = null) {
        $error = null;
        $this->query("rollback", true, $error);
        
        return !$error;
    }
    
    public function is_transaction() {
        return empty($this->dbhandle()->transaction) ? null : $this->dbhandle()->transaction;
    }
    
    public function start_transaction() {
        if (empty($this->dbhandle()->transaction)) {
            $this->dbhandle()->transaction = $this->nstart_transaction();
        }
        
        return $this->dbhandle()->transaction;
    }
    
    public function commit() {
        if ($this->dbhandle()->transaction) {
            if ($this->ncommit()) {
                $this->dbhandle()->transaction = false;
                
                return true;
            }
        }
        
        return false;
    }
    
    public function rollback() {
        if ($this->dbhandle()->transaction) {
            if ($this->nrollback()) {
                $this->dbhandle()->transaction = false;
                
                return true;
            }
        }
        
        return false;
    }
    
}
