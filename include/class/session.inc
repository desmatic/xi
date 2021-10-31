<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *
 * CREATE TABLE `session` (
 *   `sessionid` varchar(255) NOT NULL DEFAULT '',
 *   `data` mediumblob,
 *   `ipaddress` varchar(15) DEFAULT NULL,
 *   `useragent` varchar(255) DEFAULT NULL,
 *   `datecreated` datetime DEFAULT NULL,
 *   `datemodified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
 *   `datedeleted` datetime DEFAULT NULL,
 *   `datelastrequest` datetime DEFAULT NULL,
 *   `dateexpires` datetime DEFAULT NULL,
 *   PRIMARY KEY (`sessionid`)
 * )
 *****************************************/

class session extends model {

    private static $_session;
    private static $_datetime;
    private static $_requestid;
    private static $_resetcando;
    private static $_uniqueid;
    
    public function tablename() {
        return "session";
    }
    
    public function load_expressions() {
        return array(
            "expired" => new expression(self::db()->sysdate() . " > " . self::db()->iquote("dateexpires")),
            "datetime" => new expression(self::db()->sysdate())
        );
    }
    
    public static function datetime() {
        return self::$_datetime;
    }
    
    public static function date() {
        return clean::sql_date(self::$_datetime);
    }
    
    public static function time() {
        return clean::sql_time(self::$_datetime);
    }
    
    /* todo: add support for different datetime formats */
    public static function local_datetime($datetime = null) {
        if (empty($datetime)) {
            $datetime = session::datetime();
        }
        
        return date("d/m/Y H:i", strtotime($datetime));
    }
    
    public static function local_calendar_day($datetime = null) {
        if (empty($datetime)) {
            $datetime = session::datetime();
        }
        
        return date("F j, Y", strtotime($datetime));
    }
    
    public static function local_time($datetime = null) {
        if (empty($datetime)) {
            $datetime = session::datetime();
        }
        
        return date("g:ia", strtotime($datetime));
    }

    public static function start() {
        /* don't do session stuff for cgi scripts */
        if (isset($_SERVER['argv'])) {
            return true;
        }
        // Register this object as the session handler
//         session_set_save_handler(
//             array(__CLASS__, "open"), 
//             array(__CLASS__, "close"),
//             array(__CLASS__, "read"),
//             array(__CLASS__, "write"),
//             array(__CLASS__, "destroy"),
//             array(__CLASS__, "gc")
//         );
        
        /* start the session */
        #ini_set('session.cookie_httponly', 1);
        if (!session_start()) {
            echo("could not start session");
        }
        
        /* reset can_dos if necessary */
        if (session::authenticated() && strtolower(self::$_resetcando) == "y") {
            session::user()->reset_can_do();
        }
    }

    public static function open($save_path, $session_name) {
        $GLOBALS['sess_save_path'] = $save_path;

        return true;
    }

    public static function close() {

        return true;
    }

    public static function read($sessionid) {
        $model = __CLASS__;
        $s = new $model();
        if ($s->load($sessionid)) {
            self::$_datetime = $s->datetime;
            self::$_resetcando = $s->resetcando;
            if ($s->expired || $s->ipaddress != $_SERVER['REMOTE_ADDR'] || $s->useragent != $_SERVER['HTTP_USER_AGENT']) {
                $s->delete($sessionid);
            } else {
                return $s->data;
            }
        }

        return null;
    }

    public static function write($sessionid, $data) {
        $model = __CLASS__;
        $s = new $model();
        $s->data = $data;
        $s->ipaddress = $_SERVER['REMOTE_ADDR'];
        $s->useragent = $_SERVER['HTTP_USER_AGENT'];
        $s->datelastrequest = new expression(self::db()->sysdate());
        $s->dateexpires = new expression(self::db()->sysdate(get_cfg_var("session.gc_maxlifetime")));
        $s->resetcan_do = "n";
        $return = $s->update($sessionid);
        if ($return === 0 || $return === "0") {
            $s->sessionid = $sessionid;
            $s->datecreated = new expression(self::db()->sysdate());
            $return = $s->insert();
        }
        $l = new requestlog();
        $l->datecreated = new expression(self::db()->sysdate());
        $l->clientaddress = $_SERVER['REMOTE_ADDR'];
        $l->hostaddress = $_SERVER['SERVER_ADDR'];
        $l->hostport = $_SERVER['SERVER_PORT'];
        $l->requestpage = controller::get_page_request_filename();
        $l->requestpath = controller::get_page_request_path();
        $l->requestquery = controller::get_page_request_variables();
        $l->userid = self::userid();
        if (!$l->save()) {
            dlog::decho("Oops, could not save session!");
        }
        
        return $return;
    }

    public static function destroy($sessionid) {
        $model = __CLASS__;
        $s = new $model();
        
        return $s->delete($sessionid);
    }

    public static function gc($maxlifetime) {

        return true;
    }
    
    public static function user() {
        if (self::$_session === null) {
            $key = define::USER_SESSION_KEY;
            if (!isset($_SESSION[$key]) || !$_SESSION[$key] instanceof user) {
                $_SESSION[$key] = new user();
            }
            
            self::$_session = &$_SESSION[$key];
        }
        
        return self::$_session;
    }
    
    public static function uniqueid() {
        if (init::is_cli()) {
            self::$_uniqueid++;
            
            return self::$_uniqueid;
        }
        $key = define::UNIQUEID_SESSION_KEY;
        if (!isset($_SESSION[$key])) {
            $_SESSION[$key] = 0;
        }
        $_SESSION[$key]++;
    
        return $_SESSION[$key];
    }
    
    public static function authenticate($email, $password, $companyid, $ipaddress) {
        $_SESSION[define::USER_SESSION_KEY] = new user();
        self::$_session = &$_SESSION[define::USER_SESSION_KEY];
        if (strlen(trim($email)) && strlen(trim($password))) {
            $u = new user();
            if (clean::phone($email) == trim($email)) {
                $u->mobile = $email;
            } else {
                $u->email = $email;
            }
            $u->companyid = $companyid;
            $u->datedeleted = new expression("is null");
            $rows = $u->sql()->select()->all($u)->from($u)->where($u)->fetch_all();
            if (
                count($rows) == 1 && 
                self::user()->load($rows[0]['userid']) && 
                self::user()->challenge_password($password)
            ) {
                session_regenerate_id(true); /* prevent session fixation */
                return true;
            } else {
                dlog::decho("Oops, function authenticate failed!");
            }
        }
        $_SESSION[define::USER_SESSION_KEY] = new user();
        self::$_session = &$_SESSION[$key];

        return false;
    }
    
    public static function is_localhost() {
        $dots = explode('.', $_SERVER['REMOTE_ADDR']);
        
        return in_array($dots[0], array("127"));
    }
    
    public static function is_trusted_ipaddress() {
        if (self::is_localhost()) {
            return true;
        }
        $dots = explode('.', $_SERVER['REMOTE_ADDR']);
        
        return in_array($dots[0], array("10"));
    }
    
    public static function trustlevelid() {
        if (self::is_localhost()) {
            return 6;
        } else if (self::is_trusted_ipaddress()) {
            return 4;
        }
        
        return 2;
    }
    
    public static function authenticated() {
        if (session::user()->userid) {
            return true;
        }
        
        return false;
    }
    
    public static function logout() {
        session_regenerate_id(true);
        $_SESSION[define::USER_SESSION_KEY] = null;
        self::$_session = null;
    }

    public static function requestid() {
        if (!self::$_requestid) {
            self::$_requestid = rand() . time();
        }
        
        return self::$_requestid;
    }
    
    public static function requestkey() {
        return define::REQUESTID_SESSION_KEY;
    }

    public static function doing_requestid($requestid = null) {
        if (!isset($_SESSION[self::requestkey()])) {
            $_SESSION[self::requestkey()] = array();
        }
        if ($requestid = clean::integer($requestid) && strlen($requestid) == 20) {
            $_SESSION[self::requestkey()][] = $requestid;
        }
    }
    
    public function done_requestid($requestid) {
        if (!isset($_SESSION[self::requestkey()])) {
            $_SESSION[self::requestkey()] = array();
        }
    
        return in_array($requestid, $_SESSION[self::requestkey()]);
    }
    
    public static function user_can_do($privilege, $userid = null) {
        if (init::is_cli()) {
            return false;
        }
        
        return session::user()->can_do($privilege, $userid);
    }
    
    public static function agent_can_do($privilege, $userid = null) {
        if (init::is_cli()) {
            return false;
        }
        
        return session::user()->agent_can_do($privilege, $userid);
    }

    public static function can_do($privilege) {
        if (init::is_cli()) {
            return true;
        }
        
        return session::user()->can_do($privilege);
    }

    public static function userid() {
        if (init::is_cli()) {
            return define::SYSTEM_USERID;
        }
        if (session::user()->userid === null) {
            return define::SYSTEM_GUESTID;
        }
    
        return session::user()->userid;
    }

    public static function companyid() {
        if (init::is_cli()) {
            return 0;
        }
        if (session::user()->userid === null) {
            return 0;
        }
    
        return session::user()->companyid;
    }

    public static function usernickname() {
        if (init::is_cli()) {
            return define::SYSTEM_USER_NICKNAME;
        }
        if (session::user()->userid === null) {
            return define::SYSTEM_GUEST_NICKNAME;
        }
        
        if (session::user()->nickname) {
            return session::user()->nickname;
        }
    
        return session::user()->forename . "." . session::user()->surname;
    }

    public static function encrypt($plaintext, $key, $cipher = "aes-256-gcm") {
        if (!in_array($cipher, openssl_get_cipher_methods())) {
            return false;
        }
        $iv = openssl_random_pseudo_bytes(openssl_cipher_iv_length($cipher));
        $tag = null;
        $ciphertext = openssl_encrypt(
            gzcompress($plaintext),
            $cipher,
            base64_decode($key),
            $options=0,
            $iv,
            $tag,
        );
        return base64_encode(
            json_encode(
                array(
                    "ciphertext" => base64_encode($ciphertext),
                    "cipher" => $cipher,
                    "iv" => base64_encode($iv),
                    "tag" => base64_encode($tag),
                )
            )
        );
    }

    public static function decrypt($ciphertext, $key) {
        try {
            $json = json_decode(base64_decode($ciphertext), true, 2,  JSON_THROW_ON_ERROR);
        } catch (Exception $e) {
            dlog::decho("Oops, could not recognize ciphertext");
            return false;
        }
        return gzuncompress(
            openssl_decrypt(
                base64_decode($json['ciphertext']),
                $json['cipher'],
                base64_decode($key),
                $options=0,
                base64_decode($json['iv']),
                base64_decode($json['tag'])
            )
        );
    }

}
