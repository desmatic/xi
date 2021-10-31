<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/
 
abstract class controller_abstract {

    public static $nook;     /* a tidy place for keeping controller, action, and view information */

    private static $_controller;
    private static $_view_method;
    private static $_dynamic_view_query;
    private static $_bootstrap;
    private static $_init_bootstrap;
    private static $_actions;
    private static $_is_ajax_request;
    private static $_ajax_output;
    
    abstract public function db();
    abstract public function module_include_dir();
    abstract public function module_path();
    abstract public function view_unauthorized();
    abstract public function view_not_found();
    abstract public function view_login();
    abstract public function view_index();
    abstract public function template_dir();
    abstract public function template_default();

    public function model() {
        return null;
    }
    
    public function authenticated() {
        return false;
    }
    
    public function login() {
        return false;
    }
    
    public function can_view_all() {
        return false;
    }

    public function name() {
        return clean::moduledir($this->module_path());
    }
    
    protected function bootstrap() {
        return true;
    }
    
    public function module_index() {
        return $this->module_path() . "index.php";
    }
    
    public function method_prefix_widget() {
        return "widget_";
    }

    public function method_prefix_view() {
        return "view_";
    }
    
    public function default_view_method() {
        return $this->method_prefix_view() . "index";
    }
    
    public function href($page) {
        if ($page == "#") {
            return "#";
        }
        
        return $this->module_path() . $page . define::PAGE_SUFFIX;
    }

    public static function get_page_request() {
        $url = parse_url($_SERVER['REQUEST_URI']);
        if (!empty($_REQUEST['view']) && $url['path'] == $_SERVER['SCRIPT_NAME']) {
            return $_REQUEST['view'];
        }
        $view = clean::file_no_extension(clean::basename($url['path']));
        if ($view) {
            return $view;
        }
            
        return null;
    }
    
    public static function get_page_request_filename() {
        $url = parse_url($_SERVER['REQUEST_URI']);
        
        return clean::basename($url['path']);
    }
    
    public static function get_page_request_path() {
        $url = parse_url($_SERVER['REQUEST_URI']);
        
        return clean::dir($url['path']);
    }
    
    public static function get_page_request_variables() {
        $url = parse_url($_SERVER['REQUEST_URI']);
        if (!empty($url['query'])) {
            return $url['query'];
        }
            
        return null;
    }


    public function method_prefix_can_do_view() {
        return "can_";
    }
    
    public function method_can_do_view($method) {
        return $this->method_prefix_can_do_view() . $method;
    }

    public function can_do_view_all() {
        return $this->method_can_do_view($this->method_prefix_view() . "all");
    }
    
    public function method_prefix_action() {
        return "action_";
    }
    
    public function request_action() {
        return "action";
    }
    
    public function method_prefix_can_do_action() {
        return "can_";
    }
    
    public function method_can_do_action($method) {
        return $this->method_prefix_can_do_action() . $method;
    }
    
    public function can_do_action_all() {
        return $this->method_can_do_action($this->method_prefix_action() . "all");
    }

    public function can_view_unauthorized() {
        return true;
    }
    
    public function can_view_not_found() {
        return true;
    }
    
    public function can_view_login() {
        return true;
    }
    
    public function set_ajax_output($content, $append = true) {
        self::$_ajax_output = $append ? self::$_ajax_output . $content : $content;
    }
    
    public function is_ajax_request() {
        return self::$_is_ajax_request;
    }
    
    protected function get_dynamic_view($controller_class_name, $view, &$authorized) {
        /* this code needs a rewrite */
        return array();
        $authorized = true;
        if (!$this->db()) {
            return array();
        }
        
        if ($view) {
            $view = $this->db()->sql()->a()->field("p", "name")->expression("=")->quote($view);
        }
        $sql = $this->db()->sql()->
            select()->
                field("p", "pageid", "pageid")->
                field("c", "candoid")->
                field("p", "title")->
                field("p", "name")->
                field("p", "description")->
                field("p", "keywords")->
                field("a", "articleid")->
                field("a", "article")->
                field("a", "datecreated")->
                field("pp", "position")->
                field("pa", "method")->
      
            from("page", "p")->
            
            join("page_pageposition_article", "pa")->
            using("pageid")->
            
            join("pageposition", "pp")->
            using("pagepositionid")->
            
            left_join("article", "a")->
            using("articleid")->
            
            where()->field("p", "controller")->expression("=")->quote($controller_class_name)->
            expression($view)->
            a()->field("p", "datedeleted")->expression("is null")->
            a()->field("a", "datedeleted")->expression("is null")->
            a()->field("a", "datepaused")->expression("is null")->
            a()->p()->field("a", "datestart")->expression("is null")->o()->sysdate()->expression(">")->field("a", "datestart")->q()->
            a()->p()->field("a", "dateend")->expression("is null")->o()->field("a", "dateend")->expression("<")->sysdate()->q()->
            a()->p()->field("pa", "articleid")->expression("is not null")->o()->field("pa", "method")->expression("is not null")->q()->
            order_by()->field("pp", "position")->field("pa", "priority")->desc()->field("a", "datecreated")->desc();

        if ($rows = $sql->fetch_all_cached()) {
            foreach($rows as $row) {
                if (!session::can_do($row['candoid'])) {
                    $authorized = false;
                }
                break;
            }
        }
        
        return $rows;
    }
    
    public function get_dynamic_views() {
        $authorized = "";
        $rows = $this->get_dynamic_view(get_class($this), null, $authorized);
        $views = array();
        foreach($rows as $row) {
            $views[$row['name']] = null;
        }
        
        return array_keys($views);
    }
    
    public function get_dynamic_views_user_can_view() {
        $authorized = "";
        $rows = $this->get_dynamic_view(get_class($this), null, $authorized);
        $views = array();
        foreach($rows as $row) {
            if (session::can_do($row['candoid'])) {
                $views[$row['name']] = null;
            }
        }
        
        return array_keys($views);
    }

    public function replace($view) {
        return $view;
    }
    
    public function dynamic_view() {
        if (!is_array(self::$_dynamic_view_query)) {
            return false;
        }
        $c = self::$_controller;
        $classname = get_class($c);
        $row = array();
        $page = array();
        foreach(self::$_dynamic_view_query as $row) {
            if ($row['method']) {
                $widget_method = $c->method_prefix_widget() . $row['method'];
                if (method_exists($c, $widget_method)) {
                    $page[$row['position']] .= $c->$widget_method();
                    dlog::decho("calling: {$classname}->{$widget_method}();");
                } else {
                    dlog::decho("not found: {$classname}->{$widget_method}();");
                }
            } else if ($row['articleid']) {
                $page[$row['position']] .= $c->replace($row['article']);
                dlog::decho("selected: viewid=" . $row['viewid'] . ", name=" . $row['name']);
            }
        }
        $page['title'] = $row['title'];
        $page['description'] = $row['description'];
        $page['keywords'] = $row['keywords'];
        echo $this->template($page);
    }
    
    protected function get_controller($controller) {
        if ($class = clean::method($controller)) {
            try {
                if (!class_exists($class)) {
                    dlog::decho("action class {$class} not found\n");
                }
            } catch (Exception $e) {
                dlog::decho("action class {$class} not found\n");
            }
            if (is_subclass_of($class, __CLASS__)) {
                return new $controller();
            }
        }
        
        return null;
    }
    
    protected function set_view($view = null, $check_only = false) {
        return $this->set_controller(self::$_controller, $view, $check_only);
    }

    protected function set_controller($controller, $view = null, $check_only = false) {
        $view = strtolower(clean::alpha_numeric_underscore_hyphen($view));
        if ($view) {
            $view_method = $controller->method_prefix_view() . str_replace("-", "_", $view);
        } else {
            $view_method = $controller->default_view_method();
        }
        $debug = "";
        $check_only_return = false;
        $has_can_do = true;
        $controller_class_name = get_class($controller);
        /* controller method view */
        if (
            ($controller instanceof controller) &&
            (method_exists($controller, $view_method))
        ) {
            $can_view_method = $controller->method_can_do_view($view_method);
            $can_view_exists = method_exists($controller, $can_view_method);
            $can_view_all_method = $controller->can_do_view_all();
            $can_view_all_exists = method_exists($controller, $can_view_all_method);
            if (
                (!$can_view_exists && !$can_view_all_exists) ||
                ($can_view_exists && $controller->$can_view_method()) ||
                (!$can_view_exists && $can_view_all_exists && $controller->$can_view_all_method())
            ) {
                $check_only_return = true;
                $debug = "  setting: ";
            } else if (!$this->authenticated()) {
                $check_only_return = false;
                $debug = " no login: ";
                $controller = $this;
                $view_method = "view_login";
            } else {
                $check_only_return = false;
                $debug = "unauthorized: ";
                $controller = $this;
                $view_method = "view_unauthorized";
            }
        /* database driven view */
        } else if (($rows = $controller->get_dynamic_view($controller_class_name, $view, $has_can_do)) && $has_can_do) {
            $check_only_return = true;
            $debug = "setting: ";
            self::$_dynamic_view_query = $rows;
            $view_method = "dynamic_view";
        } else if (!$has_can_do && !$this->authenticated()) {
            $check_only_return = false;
            $debug = "login: ";
            $controller = $this;
            $view_method = "view_login";
        } else if (!$has_can_do) {
            $check_only_return = false;
            $debug = "unauthorized: pageid=" . $rows[0]['pageid'] . ", name=" . $rows[0]['name']. ", cando=" . $rows[0]['candoid'];
            $controller = $this;
            $view_method = "view_unauthorized";
        } else {
            /* no view found */
            $check_only_return = false;
            $debug = "not found: ";
            $controller = $this;
            $view_method = "view_not_found";
        }
        if ($check_only) {
            return $check_only_return;
        }
        self::$_controller = $controller;
        self::$_view_method = $view_method;
        dlog::decho($debug .  "{$controller_class_name}->{$view_method}();");

        return true;
    }

    public function can_view($controller, $view = null) {
        return $this->set_controller($controller, $view, true);
    }

    public function get_action_request() {
        $actions = array();
        $x = $this->request_action();
        if (!empty($_REQUEST[$x])) {
            foreach (preg_split("/\,/", $_REQUEST[$x]) as $action) {
                if (strlen($action)) {
                    $action = preg_split("/\./", $action);
                    if (
                        (count($action) == 2) && 
                        ($method = trim($action[1])) && 
                        ($class = trim($action[0]))
                   ) {
                        $actions[][$class] = $method;
                   }
                }
            }
        }

        return $actions;
    }
    
    public function get_action_output() {
        $output = "";
        if (is_array(self::$_actions)) {
            foreach (self::$_actions as $class => $method) {
                foreach ($method as $widget) {
                    if (isset($widget['widget'])) {
                        $output .= $widget['widget'];
                    }
                }
            }
        }
        
        return $output;
    }
    
    private function _call_action($class, $classname, $methodname) {
        if (empty(self::$_actions[$classname][$methodname])) {
            dlog::decho("   action: {$classname}->{$methodname}();");
            ob_start();
            ob_implicit_flush(false);
            $start_time = microtime(true);
            $return = $class->$methodname();
            $end_time = microtime(true);
            $action_time = $end_time - $start_time;
            $actionsummary = htmlentities("action {$classname}->{$methodname}() returned: {$return} {$action_time}");
            if ($action_output = ob_get_clean()) {
                $action_output = "<pre>" . htmlentities($action_output) . "</pre>\n";
                self::$_actions[$classname][$methodname]['widget'] = widget::basic($actionsummary, $action_output, array("widgetid" => "actionid" . template::uniqueid()));
            }
            self::$_actions[$classname][$methodname]['return'] = $return;
        }
    }

    public function do_actions($actions) {
        $classes = array();
        foreach ($actions as $action) {
            foreach ($action as $class => $method) {
                dlog::decho(" locating: {$class}->{$method}();");
                try {
                    if (!class_exists($class)) {
                        dlog::decho("action class {$class} not found\n");
                        continue;
                    }
                } catch (Exception $e) {
                    dlog::decho("action class {$class} not found\n");
                    continue;
                }
                if (empty($classes[$class])) {
                    if ($class == get_class($this)) {
                        $classes[$class] = $this;
                    } else if (is_subclass_of($class, __CLASS__)) {
                        $classes[$class] = new $class();
                    } else {
                        dlog::decho("action class {$class} is not a subclass of " . __CLASS__ . "\n");
                        continue;
                    }
                }
                $method = $classes[$class]->method_prefix_action() . clean::method($method);
                if (!method_exists($classes[$class], $method)) {
                    dlog::decho("action method {$class}->{$method}() does not exist\n");
                    continue;
                }
                $can_action_method = $classes[$class]->method_can_do_action($method);
                $can_action_exists = method_exists($classes[$class], $can_action_method);
                $can_action_all_method = $classes[$class]->can_do_action_all();
                $can_action_all_exists = method_exists($classes[$class], $can_action_all_method);
                if (
                    ($can_action_exists && $classes[$class]->$can_action_method()) ||
                    (!$can_action_exists && $can_action_all_exists && $classes[$class]->$can_action_all_method())
                ) {
                    /* bootstrap controller, then call action */
                    $this->_call_action($classes[$class], $class, "bootstrap");
                    $this->_call_action($classes[$class], $class, $method);
                } else {
                    dlog::decho("action: {$class}->{$method}() can_do denied\n");
                }
            }
        }
    }
    
    /* run html page */
    public function run() {
        if (empty($_SERVER['REQUEST_URI'])) {
            return false;
        }
        /* init bootstrap */
        if (empty(self::$_init_bootstrap)) {
            self::$_init_bootstrap = init::bootstrap();
        }
        /* set controller->method() */
        dlog::decho("requested controller " . get_class($this));
        $requested_page = $this->get_page_request();
        dlog::decho("requested page " . $requested_page);
        $this->set_controller($this, $requested_page);
        /* do any requested actions */
        if (
            (empty($_REQUEST['REQUESTID_REQUEST_KEY'])) ||
            (!empty($_REQUEST['REQUESTID_REQUEST_KEY']) && !session::done_requestid($_REQUEST['REQUESTID_REQUEST_KEY']))
        ) {
            if (!empty($_REQUEST['REQUESTID_REQUEST_KEY'])) {
                session::doing_requestid($_REQUEST['REQUESTID_REQUEST_KEY']);
            }
            $this->do_actions($this->get_action_request());
        } else {
            dlog::decho("skipping actions: actions already done!");
        }
        /* invoke controller->method() */
        $c = self::$_controller;
        $m = self::$_view_method;
        $classname = get_class($c);
        /* bootstrap controller */
        $this->_call_action($c, $classname, "bootstrap");
        dlog::decho("  calling: {$classname}->{$m}();");
               
        return $c->$m();
    }
    
    /* run ajax page */
    public function run_ajax() {
        self::$_is_ajax_request = true;
        if (empty($_SERVER['REQUEST_URI'])) {
            return false;
        }
        /* init bootstrap */
        if (empty(self::$_init_bootstrap)) {
            self::$_init_bootstrap = init::bootstrap();
        }
        if (
            (empty($_REQUEST['REQUESTID_REQUEST_KEY'])) ||
            (!empty($_REQUEST['REQUESTID_REQUEST_KEY']) && !session::done_requestid($_REQUEST['REQUESTID_REQUEST_KEY']))
        ) {
            if (!empty($_REQUEST['REQUESTID_REQUEST_KEY'])) {
                session::doing_requestid($_REQUEST['REQUESTID_REQUEST_KEY']);
            }
            $this->do_actions($this->get_action_request());
            if (self::$_ajax_output !== null) {
                echo self::$_ajax_output;
            }
            
            return true;
        }

        return false;
    }

    /* run cli script */
    public static function run_script($method) {
        /* init bootstrap */
        if (empty(self::$_init_bootstrap)) {
            self::$_init_bootstrap = init::bootstrap();
        }
        if (method_exists($this, $method)) {
            return $this->$method();
        } else {
            echo "error: could not find method {$method}\n";
        }
        
        return false;
    }

    public static function render($____view____, $___variables___ = null) {
        if (is_file($____view____)) {
            if (is_array($___variables___)) {
                extract($___variables___);
            }
            ob_start();
            ob_implicit_flush(false);
            $___start_time___ = microtime(true);
            include($____view____);
            $end_time = microtime(true);
            $render_time = $end_time - $___start_time___;
            dlog::decho("rendering: {$____view____} {$render_time}");
        
            return ob_get_clean();
        } else {
            dlog::decho("view not found: {$____view____}");
        }
        
        return $return;
    }
    
    /* 
     * a duplicate of the static function above that allows $this
     * to be used
     */
    private function _render($____view____, $___variables___ = null) {
        $return = null;
        if (is_file($____view____)) {
            if (is_array($___variables___)) {
                extract($___variables___);
            }
            ob_start();
            ob_implicit_flush(false);
            $___start_time___ = microtime(true);
            include($____view____);
            $end_time = microtime(true);
            $render_time = $end_time - $___start_time___;
            dlog::decho("rendering: {$____view____} {$render_time}");
        
            $return = ob_get_clean();
        } else {
            dlog::decho("view not found: {$____view____}");
        }
        
        return $return;
    }
    
    public function view_include($view, $variables = null) {
        if (!is_array($variables)) {
            $variables = array();
        }
        if ($this->model() instanceof element) {
            $variables = array_merge(array("model" => $this->model()->hash()), $variables);
        }
        $variables = array_merge($variables, array("module_index" => $this->module_index()));
        
        return $this->_render($view, $variables);
    }
    
    /* head, title, content */
    public function template($variables, $view = null) {
        if ($view === null) {
            $view = $this->template_default();
        }
        
        return $this->view_include($this->template_dir() . $view, $variables);
    }
    
    public function view_shared($view, $variables = null) {
        return $this->view_include(define::SHARED_VIEWS . $view, $variables);
    }

    public function view($view, $variables = null) {
        return $this->view_include($this->module_include_dir() . $view, $variables);
    }
    
    public function view_widget_shared($title, $view, $variables = null) {
        $content = $this->view_shared($view, $variables);
        if ($content) {
            return widget::basic($title, $content, $variables);
        }
        
        return "";
    }
    
    public function view_widget($title, $view, $variables = null) {
        $content = $this->view($view, $variables);
        if ($content) {
            return widget::basic($title, $content, $variables);
        }
        
        return "";
    }
    
    public function view_widget_form($title, $view, $variables = null) {
        $content = $this->view($view, $variables);
        if ($content) {
            return widget::form($title, $content, $variables);
        }
        
        return "";
    }

    public function get_methods($prefix) {
        $methods = array();
        $class = new ReflectionClass(get_class($this));
        foreach($class->getMethods(ReflectionMethod::IS_PUBLIC) as $method) {
            $name = $method->name;
            if (substr($name, 0, strlen($prefix)) == $prefix) {
                $methods[] = substr($name, strlen($prefix));
            }
        }
        
        return $methods;
    }
    
    public function get_actions() {
        return $this->get_methods($this->method_prefix_action());
    }
    
    public function get_can_do_actions() {
        return $this->get_methods($this->method_prefix_can_do_action());
    }
    
    public function get_can_do_views() {
        return $this->get_methods($this->method_prefix_can_do_view());
    }
    
    public function get_widgets() {
        return $this->get_methods($this->method_prefix_widget());
    }
    
    public function get_views() {
        return array_merge($this->get_methods($this->method_prefix_view()), $this->get_dynamic_views());
    }

    public function get_views_user_can_view($views) {
        $dynamic_views = $this->get_dynamic_views_user_can_view();
        $controller_views = $this->get_views();
        $can_view_all_method = $this->can_do_view_all();
        $can_view_all_exists = method_exists($this, $can_view_all_method);
        $allowed_views = array();
        foreach($views as $view) {
            $view_method = str_replace("-", "_", $view);
            $view_name = str_replace("_", "-", $view);
            if (in_array($view_method, $controller_views)) {
                $can_view_method = $this->method_can_do_view($view_method);
                $can_view_exists = method_exists($this, $can_view_method);
                if (
                    (!$can_view_exists && !$can_view_all_exists) ||
                    ($can_view_exists && $this->$can_view_method()) ||
                    (!$can_view_exists && $can_view_all_exists && $this->$can_view_all_method())
                ) {
                    $allowed_views[] = $view_name;
                }
            } else if (in_array($view_name, $dynamic_views)) {
                $allowed_views[] = $view_name;
            }
        }
        
        return $allowed_views;
    }

    public function get_menu($links, $recurse = 0) {
        $i = 0;
        $menu = array();
        $controllers = array();
        if (is_array($links) && count($links)) {
            foreach ($links as $link) {
                if (!empty($link->controller)) {
                    if ($link->controller instanceof controller) {
                        $classname = get_class($link->controller);
                        if (empty($controllers[$classname])) {
                            $controllers[$classname] = $link->controller;
                        }
                    } else if (is_string($link->controller)) {
                        $classname = $link->controller;
                        if (empty($controllers[$classname])) {
                            if ($c = $this->get_controller($classname)) {
                                $controllers[$classname] = $c;
                            } else {
                                continue;
                            }
                        }
                    } else {
                        continue;
                    }
                    if ($this->can_view($controllers[$classname], $link->view)) {
                        if (!$recurse) {
                            $link->href = "#";
                        } else {
                            $link->href = $controllers[$classname]->href($link->view);
                        }
                        if ($recurse || $link->submenu) {
                            $menu[$i] = $link;
                        }
                    }
                } else if ($link->can_do()) {
                    $menu[$i] = $link;
                }
                if (!empty($link->submenu) && is_array($link->submenu)) {
                    if ($submenu = $this->get_menu($link->submenu, $recurse + 1)) {
                        $menu[$i]->submenu = $submenu;
                    } else {
                        /* if no submenu links, skip link and continue */
                        unset($menu[$i]);
                        continue;
                    }
                }
                
                $i++;
            }
        }

        return $menu;
    }

}
