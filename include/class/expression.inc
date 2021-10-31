<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/
 
class expression {
    private $e;
    
    public function __construct($e) {
        $this->e = $e;
    }
    
    public function __toString() {
        return $this->e;
    }

    public static function string2int($string){
        $value = 0;

        for($i = 0; $i < strlen($string); $i++){
            $value += intval(ord(substr($string, $i, 1)) << (strlen($string) - $i - 1) * 8);
        }

        return $value;
    }

    public static function string2hex($string){
        $value = "";
        $strlen = strlen($string);
        for($i = 0; $i < $strlen; $i++){
            $value .= dechex(ord(substr($string, $i, 1)));
        }

        return $value;
    }

    /* Bit set/unset public static functions */
    public static function bset($number, $bit){
        if (!$number) $number = 0;
        $n = gmp_init($number);
        gmp_setbit($n, $bit, true);
        return gmp_strval($n);
    }

    public static function bunset($number, $bit){
        if (!$number) $number = 0;
        $n = gmp_init($number);
        gmp_setbit($n, $bit, false);
        return gmp_strval($n);
    }

    public static function bisset($number, $bit){
        if (!$number) $number = 0;
        $n = gmp_init($number);
        return gmp_testbit($n, $bit);
    }

    public static function bits_set($number, $max_status_code, $j = 0, $min_status_code = 1, $text = "true") {
        $return = array();
        for($i = $min_status_code; $i <= $max_status_code; $i++) {
            $return[$j][$i] = (bisset($number, $i)) ? $text : "";
        }
        return $return;
    }

    public static function barray($number) {
        if (!$number) $number = 0;
        $n = gmp_init($number);
        $bits = array();
        for ($i = 0; $i < 64; $i++) {
            if (gmp_testbit($n, $i)) {
                $bit = "1";
            } else {
                $bit = "0";
            }
            $bits[$i] = $bit;
        }

        return $bits;
    }
}
