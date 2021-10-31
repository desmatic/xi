<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/
 
class controller extends controller_abstract {

    private static $_user;
    private static $_action;
    
    /* accessor function for actions to store whatever */
    public static function action() {
        if (self::$_action === null) {
            self::$_action = new stdClass();
        }
        
        return self::$_action;
    }

    public function module_include_dir() {
        return define::DOCUMENT_ROOT . "include/";
    }

    public function module_path() {
        return "/";
    }
    
    public function db() {
        return new db();
    }

    public function authenticated() {
        return session::authenticated();
    }
    
    public function login($email = null, $password = null, $companyid = null) {
        $email = $email ? $email : $_REQUEST['user']['email'];
        $password = $password ? $password : $_REQUEST['user']['password'];
        $companyid = $companyid ? $companyid : $_REQUEST['user']['companyid'];
        if (
            empty($email) ||
            empty($password) ||
            empty($companyid)
        ) {
            return false;
        }

        return session::authenticate($email, $password, $companyid, $_SERVER['SERVER_ADDR']);
    }
    
    public function can_action_login() {
        return session::user_can_do(cando::GUEST);
    }
    
    public function action_login() {
        if ($this->login()) {
            if ($controller = $this->get_controller($_REQUEST['controller'])) {
                if (!empty($_REQUEST['get']) && ($query = parse_url($_REQUEST['get'], PHP_URL_QUERY))) {
                    parse_str($query, $get);
                    $_REQUEST = array_merge($_REQUEST, $get);
                }
            } else {
                $controller = new controller_guest();
            }
            
            return $this->set_controller($controller, $_REQUEST['page']);
        }

        return false;
    }
    
    public function can_action_logout() {
        return session::user_can_do(cando::GUEST);
    }
    
    public function action_logout() {
        session::logout();
        $this->set_view("login");
    }
    
    public function can_action_signup() {
        return session::user_can_do(cando::GUEST);
    }
    
    public function action_signup() {
        self::$_user = new user();
        if (
            self::$_user->attributes($_REQUEST) && 
            !empty($_REQUEST['signup']) && 
            !self::$_user->invalid_signup() &&
            self::$_user->save()
        ) {
            if ($this->login($_REQUEST['user']['login'])) {
                $controller = new controller_guest();
                return $this->set_controller($controller);
            } else {
            
            }
        }

        return false;
    }
    
    public function action_addimage() {
        $this->set_ajax_output($this->view_shared("addimage.inc"));
    }

    public function view_unauthorized() {
        $page['title'] = "Not Authorized";
        $page['content'] = $this->view_shared("not_authorized.inc");
        echo $this->template($page);
    }

    public function view_not_found() {
        $page['title'] = "Page Not Found";
        $page['content'] = $this->view_shared("not_found.inc");
        echo $this->template($page);
    }

    public function view_login() {
        $login = new user();
        if ($login->attributes($_REQUEST) && !empty($_REQUEST['login'])) {
            $login->checkpassword();
        }
        if (empty(self::$_user)) {
            self::$_user = new user();
        }
        $page['title'] = "Login";
        $page['content'] = $this->view_shared("login.inc", array("model" => self::$_user->hash(), "login" => $login->hash()));
        echo $this->template($page);
    }
    
    public function view_index() {
        echo "under construction";
    }
    
    public function template_dir() {
        return dirname(__FILE__) . define::TEMPLATE_DIR_RELATIVE_TO_CLASS;
    }
    
    public function template_default() {
        return define::TEMPLATE_DEFAULT;
    }
    
    public function menu_main() {
        $i = 0;
        $links = array();
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = "controller_guest";
        $links[$i]->view = "index";
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-home\"></i> <span class=\"menu-item-parent\">Dashboard</span>";
        $links[$i]->title = "Welcome";
        $links[$i] = new html_a();
        $links[$i]->controller = new controller_dashboard();
        $links[$i]->view = "index";
        $links[$i]->submenu = $links[$i]->controller->menu();
        $links[$i]->set_active_function(function () { return get_class($this) == "controller_dashboard"; });
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-home\"></i> <span class=\"menu-item-parent\">Dashboard</span>";
        $links[$i]->title = "Users";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = new controller_userfile();
        $links[$i]->view = "index";
        $links[$i]->submenu = $links[$i]->controller->menu();
        $links[$i]->set_active_function(function () { return get_class($this) == "controller_userfile"; });
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-floppy-o\"></i> <span class=\"menu-item-parent\">My Files</span>";
        $links[$i]->title = "My Files";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = new controller_user();
        $links[$i]->view = "index";
        $links[$i]->submenu = $links[$i]->controller->menu();
        $links[$i]->set_active_function(function () { return get_class($this) == "controller_user"; });
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-user\"></i> <span class=\"menu-item-parent\">Users</span>";
        $links[$i]->title = "Users";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = new controller_agent();
        $links[$i]->view = "index";
        $links[$i]->submenu = $links[$i]->controller->menu();
        $links[$i]->set_active_function(function () { return get_class($this) == "controller_agent"; });
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-user-plus\"></i> <span class=\"menu-item-parent\">Agents</span>";
        $links[$i]->title = "Agent Management";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = new controller_article();
        $links[$i]->view = "index";
        $links[$i]->submenu = $links[$i]->controller->menu();
        $links[$i]->set_active_function(function () { return get_class($this) == "controller_article"; });
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-file-text-o\"></i> <span class=\"menu-item-parent\">Articles</span>";
        $links[$i]->title = "Article Management";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = new controller_development();
        $links[$i]->view = "index";
        $links[$i]->submenu = $links[$i]->controller->menu();
        $links[$i]->set_active_function(function () { return get_class($this) == "controller_development"; });
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-cogs\"></i> <span class=\"menu-item-parent\">Development</span>";
        $links[$i]->title = "Development";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = new controller_smartadmin();
        $links[$i]->view = "index";
        $links[$i]->submenu = $links[$i]->controller->menu();
        $links[$i]->set_active_function(function () { return get_class($this) == "controller_smartadmin"; });
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-life-buoy\"></i> <span class=\"menu-item-parent\">SmartAdmin</span>";
        $links[$i]->title = "SmartAdmin";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = new controller_skeleton();
        $links[$i]->view = "index";
        $links[$i]->submenu = $links[$i]->controller->menu();
        $links[$i]->set_active_function(function () { return get_class($this) == "controller_skeleton"; });
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-paw\"></i> <span class=\"menu-item-parent\">Skeleton</span>";
        $links[$i]->title = "Developer Skeleton Module";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = "controller_email";
        $links[$i]->view = "index";
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-envelope-o\"></i> <span class=\"menu-item-parent\">Email</span>";
        $links[$i]->title = "Email Inbox";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = "controller_report";
        $links[$i]->view = "index";
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-bar-chart-o\"></i> <span class=\"menu-item-parent\">Reports</span>";
        $links[$i]->title = "Reports";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = "controller_print";
        $links[$i]->view = "index";
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-print\"></i> <span class=\"menu-item-parent\">Printing</span>";
        $links[$i]->title = "Print Management";
        $i++;
        $links[$i] = new html_a();
        $links[$i]->controller = "controller_sms";
        $links[$i]->view = "index";
        $links[$i]->innerHTML = "<i class=\"fa fa-lg fa-fw fa-mobile\"></i> <span class=\"menu-item-parent\">SMS Messaging</span>";
        $links[$i]->title = "SMS Messaging Management";

        return $links;
    }

    public function page() {
        $page = array();
        $page['header'] = $this->template(null, "smartadmin_header.inc");
        $page['ribbon'] = $this->template(null, "smartadmin_ribbon.inc");
        $page['sitenav'] = template::unordered_list($this->get_menu($this->menu_main()));
        $page['logininfo'] = $this->template(null, "smartadmin_logininfo.inc");
        
        return $page;
    }

}
