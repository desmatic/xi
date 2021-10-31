<?php

/********************
 *
 * Author:      Des Jones
 * Project:     sms system
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.01
 *
 *****************************************/

class clean {
    private static $_taboo;
    
    public static function is_empty($input) {
        if (isset($input) == true) {
            if ((empty($input) === false) || ($input === 0) || ($input === '0')) {
                return false;
            } else {
                return true;
            }
        } else {
            return true;
        }
    }
    
    public static function basedir($input, $length = "0") {
        $dirfound = false;
        $input = trim($input);
        $stringlength = strlen($input);
        $output = "";
        $input[0] == "/" ? $j = 1 : $j = 0;
        for ($i=$j; $i < $stringlength; $i++) {
            if ($input[$i] == "/") {
                $dirfound = true;
                break;
            }
            if (
                    ($input[$i] >= " ") &&
                    ($input[$i] <= "~") &&
                    ($input[$i] != "?") &&
                    ($input[$i] != "*") &&
                    ($input[$i] != '"') &&
                    ($input[$i] != '|') &&
                    ($input[$i] != '<') &&
                    ($input[$i] != '>') &&
                    ($input[$i] != ';') &&
                    ($input[$i] != '\\')
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $dirfound ? $output : "/";
    }
    
    public static function moduledir($input, $length = "0") {
        $input = trim($input);
        if ($input == "/") {
            return "root";
        } else if (substr($input, -1) == "/") {
            $input == substr($input, 0, -1);
        }
        
        return self::basedir($input, $length);
    }
    
    public static function basename($input, $length = "0") {
        $dirfound = false;
        $input = trim($input);
        $stringlength = strlen($input);
        $output = "";
        $j = 0;
        for ($i=0; $i < $stringlength; $i++) {
            if ($input[$i] == "/") {
                $j = $i + 1;
            }
        }
        $output = substr($input, $j);

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output == "" ? null : $output;
    }
    
    public static function dir($input, $length = "0") {
        $dirfound = false;
        $input = trim($input);
        $stringlength = strlen($input);
        $output = "";
        $j = 0;
        for ($i=0; $i < $stringlength; $i++) {
            if ($input[$i] == "/") {
                $j = $i + 1;
            }
        }
        $output = substr($input, 0, $j);

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function file_no_extension($input, $length = "0") {
        $dirfound = false;
        $input = trim($input);
        $stringlength = strlen($input);
        $output = "";
        $j = 0;
        for ($i=0; $i < $stringlength; $i++) {
            if ($input[$i] == ".") {
                $j = $i;
            }
        }
        if ($j) {
            $output = substr($input, 0, $j - $stringlength);
        } else {
            $output = $input;
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function method($input = "", $length = "0") {
        $input = "{$input}";
        $input = str_replace("-", "_", $input);
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "_")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return strtolower($output);
    }
    
    public static function view($input = "", $length = "0") {
        $input = "{$input}";
        $input = str_replace("_", "-", $input);
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "-")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return strtolower($output);
    }

    public static function integer($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = null;
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function html($input){
        return strip_tags($input);
    }

    public static function negative_integer($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = null;
        $isnumber = false;
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
                $isnumber = true;
            } else if ((!$isnumber) && ($input[$i] == "-")) {
                $output .= $input[$i];
            }
        }

        if ($output == "-") {
            $output = "";
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function alpha_numeric($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z"))
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function yn($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    ($input[$i] == "y") || ($input[$i] == "Y") ||
                    ($input[$i] == "n") || ($input[$i] == "N")
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function yesno($input = "", $length = "0") {
        $input = "{$input}";
        $input = trim($input);
        if (in_array(strtolower($input), array("yes", "no"))) {
            return $input;
        }

        return null;
    }
    
    public static function address($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "-") || ($input[$i] == "'") || ($input[$i] == "~") || ($input[$i] == " ") ||
                    ($input[$i] == ",") || ($input[$i] == ".") || ($input[$i] == "#") || ($input[$i] == "/")
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function postcode($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ") || ($input[$i] == "+")
            ) {
                if($input[$i] == "+") {
                    $output .= " ";
                } else {
                    $output .= $input[$i];
                }
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function url($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            //http:://www.some-addres_here/my.html?var=val&var2=val%20val#anchor
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    (($input[$i] != "/") && ($input[$i + 1] != "/")) ||
                    (($input[$i] == "/") && ($input[$i + 1] != "*")) ||
                    (($input[$i] == "*") && ($input[$i + 1] != "/")) ||
                    (($input[$i] == "-") && ($input[$i + 1] != "-")) ||
                    ($input[$i] == "%") || ($input[$i] == "?") ||
                    ($input[$i] == "_") || ($input[$i] == ".") ||
                    ($input[$i] == "&") || ($input[$i] == "=") ||
                    ($input[$i] == "+")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha($input = "", $length = "0") {
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z"))
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_space($input = "", $length = "0") {
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_underscore($input = "", $length = "0") {
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "_")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function alpha_hyphen_space($input = "", $length = "0") {
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "-") || ($input[$i] == " ")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function numeric_comma($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    ($input[$i] == ",")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_underscore($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "_")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function alpha_numeric_underscore_space($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "_") || ($input[$i] == " ") || ($input[$i] == "|")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public function alpha_numeric_underscore_space_hyphen_comma($input, $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ") || ($input[$i] == "-") || ($input[$i] == "_") || ($input[$i] == ",")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public function alpha_numeric_underscore_space_hyphen_comma_equal($input, $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ") || ($input[$i] == "-") || ($input[$i] == "_") || ($input[$i] == ",") || ($input[$i] == "=")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public function alpha_numeric_comma($input, $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == ",")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public function alpha_numeric_underscore_bracket($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "_") || ($input[$i] == "[") || ($input[$i] == "]")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_decimal($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == ".")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function alpha_numeric_decimal_angle($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == ".") ||
                    ($input[$i] == "<") ||
                    ($input[$i] == ">")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_hyphen($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "-")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function numeric_hyphen($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    ($input[$i] == "-")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_space($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_hyphen_space($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ") || ($input[$i] == "-")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_hyphen_space_newline($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ") || 
                    ($input[$i] == "-") || 
                    ($input[$i] == "\n") || 
                    ($input[$i] == "\r")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_underscore_hyphen($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ") || ($input[$i] == "_") || ($input[$i] == "-")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_underscore_space_hypen_period($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ") || ($input[$i] == "_") || ($input[$i] == "-") || ($input[$i] == ".")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function bank_account_number($input = "", $length = "0") {
        $input = "{$input}";
        $input = trim($input);
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == " ") ||  ($input[$i] == "-") ||
                    ($input[$i] == ".") ||  ($input[$i] == "/") ||
                    ($input[$i] == "#") ||  ($input[$i] == "*")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function sql($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= " ") && ($input[$i] <= "~")) &&
                    ($input[$i] != "'") && ($input[$i] != "\"") && ($input[$i] != "`") && ($input[$i] != "\\")
                ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_underscore_hyphen_period($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == ".") || ($input[$i] == "_") || ($input[$i] == "-")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function alpha_numeric_underscore_hyphen_period_newline($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == ".") || ($input[$i] == "_") ||
                    ($input[$i] == "-") || ($input[$i] == "\n") || ($input[$i] == " ")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function numeric_space($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    ($input[$i] == " ")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function numeric($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9"))
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function numeric_period($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    ($input[$i] == ".")
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function uk_date($input = "", $length = "0") {
        $format = "d/m/Y";
        $input = trim($input);
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            } else if (($input[$i] == "/") || ($input[$i] == "-") || ($input[$i] == ":") || ($input[$i] == " ")) {
                $output .= "-";
            }
        }
        $date = array();
        if ($output) {
            $date = preg_split("/-/", $output);
        }

        $output = "";
        $icount = count($date);
        /* uk date format */
        if (
            ($icount == 3) && 
            ($date[2] >= 1900) && 
            (is_numeric($date[1])) && 
            (is_numeric($date[0])) && 
            (checkdate($date[1], $date[0], $date[2]))
        ) {
            $timestamp = strtotime($date[1] . "/" . $date[0] . "/" . $date[2]);
            if ($timestamp) {
                $output = date($format, $timestamp);
            }
        /* sql date format */
        } else if (
            (($icount == 3) || ($icount == 6)) && 
            ($date[0] >= 1900) && 
            (is_numeric($date[1])) && 
            (is_numeric($date[2])) && 
            (checkdate($date[1], $date[2], $date[0]))
        ) {
            $timestamp = strtotime($date[1] . "/" . $date[2] . "/" . $date[0]);
            if ($timestamp) {
                $output = date($format, $timestamp);
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function sql_date($input = "", $length = "0") {
        $format = "Y-m-d";
        $input = trim($input);
        $stringlength = strlen($input);
        $output = null;
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            } else if (($input[$i] == "/") || ($input[$i] == "-") || ($input[$i] == ":") || ($input[$i] == " ")) {
                $output .= "-";
            }
        }
        $date = array();
        if ($output) {
            if (strstr($output, "-")) {
                $date = preg_split("/-/", $output);
            } else if (strlen($output) == 8) {
                $date = array(substr($output, 0, 2), substr($output, 2, 2), substr($output, 4, 4));
            }
        }

        $output = null;
        $icount = count($date);
        /* uk date format */
        if (
                (($icount == 3) || ($icount == 5) || ($icount == 6)) && 
                ($date[2] >= 1900) && 
                (is_numeric($date[1])) && 
                (is_numeric($date[0])) && 
                (checkdate($date[1], $date[0], $date[2]))
        ) {
            $timestamp = strtotime($date[1] . "/" . $date[0] . "/" . $date[2]);
            if (($timestamp) || ($timestamp === 0)) {
                $output = date($format, $timestamp);
            }
        /* sql date format */
        } else if (
            (($icount == 3) || ($icount == 6)) && 
            ($date[0] >= 1900) &&
            (is_numeric($date[1])) && 
            (is_numeric($date[2])) &&
            (checkdate($date[1], $date[2], $date[0]))
        ) {
            $timestamp = strtotime($date[1] . "/" . $date[2] . "/" . $date[0]);
            if ($timestamp) {
                $output = date($format, $timestamp);
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function sql_date_time($input = "", $length = "0") {
        $format = "Y-m-d H:i:s";
        $input = trim($input);
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            } else if (($input[$i] == "/") || ($input[$i] == "-") || ($input[$i] == ":") || ($input[$i] == " ")) {
                $output .= "-";
            }
        }
        $date = array();
        if ($output) {
            if (strstr($output, "-")) {
                $date = preg_split("/-/", $output);
            } else if (strlen($output) == 8) {
                $date = array(substr($output, 0, 2), substr($output, 2, 2), substr($output, 4, 4));
            }
        }

        $output = null;
        $timestamp = null;
        $icount = count($date);
        /* uk date format */
        if (($icount == 3) && ($date[2] >= 1900) && (checkdate($date[1], $date[0], $date[2]))) {
            $timestamp = strtotime($date[1] . "/" . $date[0] . "/" . $date[2]);
        /* sql date format */
        } else if (($icount == 3) && ($date[0] >= 1900) && (checkdate($date[1], $date[2], $date[0]))) {
            $timestamp = strtotime($date[1] . "/" . $date[2] . "/" . $date[0]);
        /* sql date format */
        } else if (
                    ($icount == 6) &&
                    ($date[0] >= 1900) &&
                    (checkdate($date[1], $date[2], $date[0])) &&
                    ($date[3] >= 0) && ($date[3] <= 23) &&
                    ($date[4] >= 0) && ($date[4] <= 59) &&
                    ($date[5] >= 0) && ($date[5] <= 59)
        ) {
            $timestamp = strtotime($date[1] . "/" . $date[2] . "/" . $date[0] . " " . $date[3] . ":" . $date[4] . ":" . $date[5]);
        }

        if ($timestamp) {
            $output = date($format, $timestamp);
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function sql_time($input = "", $length = "0") {
        $stringlength = strlen($input);
        
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            } else if (($input[$i] == "/") || ($input[$i] == "-") || ($input[$i] == ":") || ($input[$i] == " ")) {
                $output .= ":";
            }
        }
        $time = array();
        if ($output) {
            $time = preg_split("/:/", $output);
        }

        $output = "";
        /* validate fields */
        if (
            (count($time) == 3) &&
            ($time[0] >= 0) && ($time[0] <= 24) &&
            ($time[1] >= 0) && ($time[1] < 60) &&
            ($time[2] >= 0) && ($time[2] < 60)
        ) {
            $output = sprintf("%02d:%02d:%02d", $time[0], $time[1], $time[2]);
        } else if (
                (count($time) == 2) &&
                ($time[0] >= 0) && ($time[0] <= 24) &&
                ($time[1] >= 0) && ($time[1] < 60)
        ) {
            $output = sprintf("%02d:%02d:%02d", $time[0], $time[1], 0);
        } else if (
                (count($time) == 6) &&
                ($time[3] >= 0) && ($time[3] <= 24) &&
                ($time[4] >= 0) && ($time[4] < 60) &&
                ($time[5] >= 0) && ($time[5] < 60)
        ) {
            $output = sprintf("%02d:%02d:%02d", $time[3], $time[4], $time[5]);
        }

        return $output;
    }

    public static function year($input = "", $length = "0") {
        $input = self::Integer($input);
        $output = "";
        if (($input > 1900) && ($input < 2100)) {
            $output = $input;
        }

        return $output;
    }

    public static function day_of_month($input = "", $length = "0") {
        $input = self::Integer($input);
        $output = "";
        if (($input > 0) && ($input < 32)) {
            $output = sprintf("%02d", $input);
        }

        return $output;
    }

    public static function month_of_year($input = "", $length = "0") {
        $input = self::Integer($input);
        $output = "";
        if (($input > 0) && ($input < 13)) {
            $output = sprintf("%02d", $input);
        }

        return $output;
    }

    public static function time($input = "", $length = "0") {
        $stringlength = strlen($input);
        
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            } else if (($input[$i] == "/") || ($input[$i] == "-") || ($input[$i] == ":") || ($input[$i] == " ")) {
                $output .= ":";
            }
        }
        $time = array();
        if ($output) {
            $time = preg_split("/:/", $output);
        }

        $output = "";
        /* validate fields */
        if (
            (count($time) == 3) &&
            ($time[0] >= 0) && ($time[0] <= 24) &&
            ($time[1] >= 0) && ($time[1] < 60) &&
            ($time[2] >= 0) && ($time[2] < 60)
        ) {
            $output = sprintf("%02d:%02d", $time[0], $time[1]);
        } else if (
                (count($time) == 2) &&
                ($time[0] >= 0) && ($time[0] <= 24) &&
                ($time[1] >= 0) && ($time[1] < 60)
        ) {
            $output = sprintf("%02d:%02d", $time[0], $time[1]);
        } else if (
                (count($time) == 6) &&
                ($time[3] >= 0) && ($time[3] <= 24) &&
                ($time[4] >= 0) && ($time[4] < 60) &&
                ($time[5] >= 0) && ($time[5] < 60)
        ) {
            $output = sprintf("%02d:%02d", $time[3], $time[4]);
        }

        return $output;
    }

    public static function decimal($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = null;
        $decimal = false;
        $isnumber = false;
        $hasnumber = false;
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] == ".") && ($decimal == false)) {
                $output .= $input[$i];
                $decimal = true;
                $isnumber = true;
            } elseif (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
                $isnumber = true;
                $hasnumber = true;
            } else if ((!$isnumber) && ($input[$i] == "-")) {
                $output .= $input[$i];
                $isnumber = true;
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $hasnumber ? $output : null;
    }

    public static function email($input = "", $length = "0") {
        $input = trim($input);
        $output = "";

        $email_parts = explode("@", $input);
        if (count($email_parts) != 2) {
            return $output;
        }

        $username = $email_parts[0];
        $domainname = $email_parts[1];
        $stringlength = strlen($username);
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($username[$i] >= "a") && ($username[$i] <= "z")) ||
                    (($username[$i] >= "A") && ($input[$i] <= "Z")) ||
                    (($username[$i] >= "0") && ($username[$i] <= "9")) ||
                    ($username[$i] == ".") || ($username[$i] == "-") ||
                    ($username[$i] == "_") || ($username[$i] == "+")
                ) {
                $output .= $username[$i];
            }
        }

        if (!$output) {
            return $output;
        }

        $output .= "@";
        $stringlength = strlen($domainname);
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($domainname[$i] >= "a") && ($domainname[$i] <= "z")) ||
                    (($domainname[$i] >= "A") && ($domainname[$i] <= "Z")) ||
                    (($domainname[$i] >= "0") && ($domainname[$i] <= "9")) ||
                    ($domainname[$i] == ".") || ($domainname[$i] == "-")
                ) {
                $output .= $domainname[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function filename($input = "", $length = "0") {
        $input = trim($input);
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    ($input[$i] >= " ") &&
                    ($input[$i] <= "~") &&
                    ($input[$i] != "?") &&
                    ($input[$i] != "*") &&
                    ($input[$i] != '"') &&
                    ($input[$i] != '|') &&
                    ($input[$i] != '<') &&
                    ($input[$i] != '>') &&
                    ($input[$i] != ';') &&
                    ($input[$i] != '\\')
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function ipaddress($input = "", $length = "0") {
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    ($input[$i] == ".") ||
                    ($input[$i] == "/")
            ) {
                $output .= $input[$i];
            }
        }

        $invalid = true;
        if ($output) {

            $address = split("/", $output);
            $octet = split("\.", $address[0]);

            $invalid = false;
            if ((count($address) <= 2) && (count($octet) == 4)) {
                for ($i = 0; $i < 4; $i++) {
                    if ($octet[$i] > 255) {
                        $invalid = true;
                    }
                }

                /* valid subnet */
                if (
                        (count($address) == 2) &&
                        ((count(split("\.", $address[1])) != 1) || ($address[1] > 30) || ($address[1] < 26))
                ) {
                    $invalid = true;
                }

                /* check for reserved ipaddresses */
                if (
                        ($octet[0] == 0) || ($octet[0] >= 224) || ($octet[0] == 127) || ($octet[0] == 10) ||
                        (($octet[0] == 192) && ($octet[1] == 168)) ||
                        (($octet[0] == 172) && ($octet[1] >= 16) && ($octet[1] <= 31))
                ) {
                    $invalid = true;
                }
            }
        }

        if ($invalid) {
            $output = "";
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function name($input = "", $length = "0") {
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "-") || ($input[$i] == "'") || ($input[$i] == " ")
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function numeric_name($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    ($input[$i] == "-") || ($input[$i] == "'") || ($input[$i] == " ")
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function company_name($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $input = trim($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    (($input[$i] == "/") && ($input[$i + 1] != "/")) ||
                    ($input[$i] == "-") || ($input[$i] == "'") || ($input[$i] == " ") ||
                    ($input[$i] == "(") || ($input[$i] == ")") || ($input[$i] == "&") ||
                    ($input[$i] == ".")
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function password($input = "", $length = "0") {
        $input = "{$input}";
        $input = trim($input);
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= " ") && ($input[$i] <= "~"))
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function buisness_phone($input = "", $length = "0") {
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            }

        }

        if (substr($output,0,2) == "44") {
            $output = "0" . substr($output,2);
        }

        if (substr($output,0,1) != "0") {
            $output = "0" . $output;
        }

        $number_of_digits = strlen($output);

        if ($number_of_digits > 11) {
            $output = substr($output,0,11);
            $number_of_digits = 11;
        }

        if (($number_of_digits == 10) && ((substr($output,0,3) == "011"))) {
            // do nothing
        } else if ($number_of_digits == 11) {
//             $prefix = substr($output,0,2);
//             if (($prefix != "01") && ($prefix != "02") && ($prefix != "07") && ($prefix != "08")) {
//                 $output = "";
//             }
        } else {
            $output = "";
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function phone($input = "", $length = "0") {
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            }

        }

        if (substr($output,0,2) == "44") {
            $output = "0" . substr($output,2);
        }

        if (substr($output,0,1) != "0") {
            $output = "0" . $output;
        }

        $number_of_digits = strlen($output);

        if ($number_of_digits > 11) {
            $output = substr($output,0,11);
            $number_of_digits = 11;
        }

        if ($number_of_digits == 10) {
            // do nothing
        } else if ($number_of_digits == 11) {
            $prefix = substr($output,0,2);
            if (($prefix != "01") && ($prefix != "02") && ($prefix != "07") && ($prefix != "08")) {
                $output = null;
            }
        } else {
            $output = null;
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function phone_international($input = "", $length = "0") {
        $input = "{$input}";
        $output = "";
        if ($input = self::Phone($input, $length)) {
            $output = "44" . substr($input, 1);
        }

        return $output;
    }

    public static function phone_prefix($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            }
        }

        $length = strlen($output);
        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        if ($length < 4) {
            $output = "";
        }

        return $output;
    }

    public static function phone_suffix($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (($input[$i] >= "0") && ($input[$i] <= "9")) {
                $output .= $input[$i];
            }
        }

        $length = strlen($output);
        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        if ($length < 4) {
            $output = "";
        }

        return $output;
    }

    public static function text($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= " ") && ($input[$i] <= "~")) ||
                    ($input[$i] == "\t") || ($input[$i] == "\n") || ($input[$i] == "\r")
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }
    
    public static function note($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= " ") && ($input[$i] <= "~")) ||
                    ($input[$i] == "\t") || ($input[$i] == "\n") || ($input[$i] == "\r")
               ) {
                $output .= $input[$i];
            }
        }
        $tags = array();
        $tags[] = "<p>";
        $tags[] = "<a>";
        $tags[] = "<h1>";
        $tags[] = "<h2>";
        $tags[] = "<h3>";
        $tags[] = "<h4>";
        $tags[] = "<h5>";
        $tags[] = "<h6>";
        $tags[] = "<ul>";
        $tags[] = "<ol>";
        $tags[] = "<li>";
        $tags[] = "<em>";
        $tags[] = "<sup>";
        $tags[] = "<sub>";
        $tags[] = "<code>";
        $tags[] = "<strong>";
        $tags[] = "<blockquote>";
        $tags[] = "<span>";
        $tags[] = "<div>";
        $tags[] = "<pre>";
        $tags[] = "<br>";
        $output = strip_tags($output, implode("", $tags));

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function username($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    (($input[$i] >= "A") && ($input[$i] <= "Z")) ||
                    (($input[$i] >= "0") && ($input[$i] <= "9")) ||
                    ($input[$i] == ".")
               ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function comments($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        $comment = false;
        $doubledash = false;
        $multiline = false;
        for ($i=0; $i < $stringlength; $i++) {
            if ($doubledash || $multiline) {
                if ($doubledash) {
                    if ($input[$i] == "\n") {
                        $doubledash = false;
                    }
                } else if ($multiline) {
                    if (($i + 1 < $stringlength) && ($input[$i] == "*") && ($input[$i + 1] == "/")) {
                        $multiline = false;
                        $i++;
                    }
                }
            } else if (
                    ($i + 1 < $stringlength) &&
                    (
                        (($input[$i] == "-") && ($input[$i + 1] == "-")) ||
                        (($input[$i] == "/") && ($input[$i + 1] == "*"))
                    )
            ) {
                if (($input[$i] == "-") && ($input[$i + 1] == "-")) {
                    $doubledash = true;
                } else if (($input[$i] == "/") && ($input[$i + 1] == "*")) {
                    $multiline = true;
                }
                $i++;
            } else {
                $output .= $input[$i];
            }

        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function sms($input = "", $length = "0") {
        $input = "{$input}";
        $stringlength = strlen($input);
        $output = "";
        for ($i=0; $i < $stringlength; $i++) {
            if (
                    (($input[$i] >= " ") && ($input[$i] <= "Z")) ||
                    (($input[$i] >= "a") && ($input[$i] <= "z")) ||
                    ($input[$i] == "\n") || ($input[$i] == "\r") ||
                    ((ord($input[$i]) == 194) && (ord($input[$i + 1]) == 163)) ||
                    ((ord($input[$i]) == 163) && (ord($input[$i - 1]) == 194))
            ) {
                $output .= $input[$i];
            }
        }

        if ($length > 0) {
            $output = substr($output, 0, $length);
        }

        return $output;
    }

    public static function originator($input = "", $length = "0") {
        $input = "{$input}";
        $output = "";
        $numeric = self::Integer($input);
        $alphanumeric = self::AlphaNumeric($input);
        if ((strlen($numeric) == strlen($alphanumeric))) {
            $output = substr($numeric, 0, 14);
        } else {
            $output = substr($alphanumeric, 0, 11);
        }

        return $output;
    }

    public static function luhn($input) {
        $number = self::Integer($input);
        # Double every second digit started at the righ
        $doubledNumber  = "";
        $odd            = false;
        for($i = strlen($number)-1; $i >=0; $i--)
        {
            $doubledNumber .= ($odd) ? $number[$i]*2 : $number[$i];
            $odd            = !$odd;
        }

        # Add up each 'single' digit
        $sum = 0;
        for($i = 0; $i < strlen($doubledNumber); $i++)
            $sum += (int)$doubledNumber[$i];

        # A valid number doesn't have a remainder after mod10\
        # or equal to 0
        if ((($sum % 10 ==0) && ($sum != 0))) {
            return $number;
        }

        return "";
    }

    public static function credit_card_issue($input) {
        $input = self::Integer($input);
        if (($input) && (strlen($input) < 3) && ($input > 0)) {
            return $input;
        }

        return "";
    }

    public static function credit_card_cvv2($input) {
        $input = self::Integer($input);
        $length = strlen($input);
        if (($length == 3) || ($length == 4)) {
            return $input;
        }

        return "";
    }

    public static function credit_card_expire_date($input) {
        $input = self::Integer($input);
        if (
            ((strlen($input) == 4) || (strlen($input) == 6)) &&
            ((substr($input, 0, 2) >= 1) && (substr($input, 0, 2) <= 12)) &&
            ((substr($input, -2) - date("y") > 0) || ((date("y") == substr($input, -2)) && (substr($input, 0, 2) >= date("m"))))
        ) {
            return $input;
        }

        return "";
    }

    public static function credit_card_start_date($input) {
        $input = self::Integer($input);
        if (
            ((strlen($input) == 4) || (strlen($input) == 6)) &&
            ((substr($input, 0, 2) >= 1) && (substr($input, 0, 2) <= 12)) &&
            ((date("y") - substr($input, -2) > 0) || ((date("y") == substr($input, -2)) && (date("m") >= substr($input, 0, 2))))
         ) {
            return $input;
        }

        return "";
    }

    public static function array2url($input) {
        $output = "";
        if ((isset($input)) && (is_array($input))) {
            foreach ($input as $key => $value) {
                if (!is_array($value)) {
                    if ($output) {
                        $output .= "&amp;";
                    } else {
                        $output .= "?";
                    }
                    $output .=  urlencode($key) . "=" . urlencode($value);
                }
            }
        }

        return $output;
    }
}
