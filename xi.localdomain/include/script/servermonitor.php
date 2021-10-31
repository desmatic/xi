#!/usr/bin/php
<?php

/*
 * do a write, check all slaves the write cleared
 * show full processlist for all databases
 * show cpu and disk usage for all major servers
 * test all servers for disk write and read
 */

function ssh($server, $cmd) {
    $output = "";
    if ($hdl = popen("ssh {$server} \"{$cmd}\"", "r")) {
        while (!feof($hdl)) {
            $output .= fgets($hdl) . "\n";
        }
    } else {
        return false;
    }
    
    return $output;
}

function disk_usage($server) {
    $cmd = "df -h";
    return ssh($server, $cmd);
}

function cpu_usage($server) {
    $cmd = "vmstat";
    return ssh($server, $cmd);
}

function mysql_usage($server) {
    $cmd = "mysql -e \\\"show full processlist\\\"";
    return ssh($server, $cmd);
}

function mysql_slave_status($server) {
    $cmd = "mysql -e \\\"show slave status\\\"";
    $tab_delimited = ssh($server, $cmd);
    $tab_array = explode("\t", $tab_delimited);
    $output = array();
    $j = count($tab_array) / 2;
    for($i = 0; $i < $j; $i++) {
        $output[$tab_array[$i]] = $tab_array[$j + $i];
    }
    
    return print_r($output, true);
}

function mysql_check_slave_write($master, $slaves) {
    return ssh($server, $cmd);
}

$server = "root@slave2";
// echo ssh($server, $cmd);
echo mysql_usage($server);