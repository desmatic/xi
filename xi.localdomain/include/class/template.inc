<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/
 
class template {

    const TABLE_CLASS = "table table-condensed table-striped table-hover table-bordered";
    
    private static $_unqiueid;
    
    public static function uniqueid() {
        if (self::$_unqiueid == null) {
            self::$_unqiueid = 0;
        }
        self::$_unqiueid++;
        
        return self::$_unqiueid;
    }
    
    public static function fixedtable($rows, $variables = null) {
        $classes = self::TABLE_CLASS . " fixedtable";
        $variables['class'] = empty($variables['class']) ? $classes : $classes . " " . $variables['class'];
        
        return self::table($rows, $variables);
    }
    
    public static function scrolltable($rows, $variables = null) {
        $classes = self::TABLE_CLASS . " scrolltable";
        $variables['class'] = empty($variables['class']) ? $classes : $classes . " " . $variables['class'];
        
        return self::table($rows, $variables);
    }
    
    public static function sorttable($rows, $variables = null) {
        $classes = self::TABLE_CLASS . " sorttable";
        $variables['class'] = empty($variables['class']) ? $classes : $classes . " " . $variables['class'];
        
        return self::table($rows, $variables);
    }
    
    public static function exporttable($rows, $variables = null) {
        $classes = self::TABLE_CLASS . " sorttable";
        $variables['class'] = empty($variables['class']) ? $classes : $classes . " " . $variables['class'];
        
        return self::table($rows, $variables);
    }
    
    /* class, id, divclass, divid, tfoot */
    public static function table($rows, $variables = null) {
        $output = "";
        if (!is_array($rows) || !count($rows)) {
            return $output;
        }
        $keys = array();
        foreach($rows as $row) {
            if (!is_array($row) || !count($row)) {
                return $output;
            }
            $keys = array_keys($row);
            break;
        }
        /* start wrapper div */
        $output = "<div";
        $output .= empty($variables['divid']) ? "" : " id=\"{$variables['divid']}\"";
        $output .= empty($variables['divclass']) ? "" : " class=\"{$variables['divclass']}\"";
        $output .= ">\n";
        /* start table */
        $output .= "<table";
        $output .= empty($variables['id']) ? "" : " id=\"{$variables['id']}\"";
        $output .= empty($variables['class']) ? "" : " class=\"{$variables['class']}\"";
        $output .= empty($variables['width']) ? " width=\"100%\"" : " width=\"{$variables['class']}\"";
        $output .= ">\n";
        /* thead */
        $output .= "\t<thead>\n";
        $output .= "\t\t<tr>\n";
        $i = 0;
        foreach($keys as $key){
            $hide = " data-hide=\"phone,tablet\"";
            if ($i == 1) {
                $hide = " data-class=\"expand\"";
            } else if ($i == 2) {
                $hide = "";
            }
            $output .= "\t\t\t<th{$hide}>{$key}</th>\n";
            $i++;
        }
        $output .= "\t\t</tr>\n";
        $output .= "\t</thead>\n";
        /* tfoot */
        if (!empty($variables['tfoot'])) {
            $output .= "\t<tfoot>\n";
            $output .= "\t\t<tr>\n";
            foreach($keys as $key) {
                $output .= "\t\t\t<th>{$key}</th>\n";
            }
            $output .= "\t\t</tr>\n";
            $output .= "\t</tfoot>\n";
        }
        /* tbody */
        $output .= "\t<tbody>\n";
        foreach($rows as $row) {
            $output .= "\t\t<tr>\n";
            foreach($keys as $key) {
                $output .= "\t\t\t<td>";
                $value = trim($row[$key]);
//                 if ($value == "") { 
//                     $value = "&nbsp;"; 
//                 }
                $output .= $value;
                $output .= "</td>\n";
            }
            $output .= "\t\t</tr>\n";
        }
        $output .= "\t</tbody>\n";
        /* close table and wrapper div */
        $output .= "</table>\n</div>\n";

        return $output;
    }
    
    /* class, id, style, divclass, divid, divstyle, menuid */
    public static function navbar($links, $variables = null) {
        $output = "";
        if (!is_array($links) || !count($links)) {
            return $output;
        }
        if (empty($variables['menuid'])) {
            $variables['menuid'] = "menuid" . self::uniqueid();
        }
        $nav_class = "navbar-default";
        if (!empty($variables['divclass']) && strpos($variables['divclass'], "navbar-inverse")) {
            $variables['menuid'] = "menuid" . self::uniqueid();
        }
        /* start nav */
        $output = "<nav class=\"navbar {$nav_class}";
        $output .= empty($variables['divclass']) ? "\"" : " {$variables['divclass']}\"";
        $output .= empty($variables['divstyle']) ? "" : " style=\"{$variables['divstyle']}\"";
        $output .= empty($variables['divid']) ? "" : " id=\"{$variables['divid']}\"";
        $output .= " role=\"navigation\">\n";
        $output .= "\t<div class=\"container-fluid\">\n";
        /* collapsable menu */
        $output .= "\t\t<div class=\"navbar-header\">\n";
        $output .= "\t\t\t<button type=\"button\" class=\"navbar-toggle pull-left\" data-toggle=\"collapse\" data-target=\"#{$variables['menuid']}\">\n";
        $output .= "\t\t\t\t<span class=\"sr-only\">Toggle navigation</span>\n";
        $output .= "\t\t\t\t<span class=\"icon-bar\"></span>\n";
        $output .= "\t\t\t\t<span class=\"icon-bar\"></span>\n";
        $output .= "\t\t\t\t<span class=\"icon-bar\"></span>\n";
        $output .= "\t\t\t</button>\n";
        $output .= "\t\t</div>\n";
        /* start div */
        $output .= "\t\t<div class=\"collapse navbar-collapse\" id=\"{$variables['menuid']}\">\n";
        /* start ul */
        $output .= "\t\t\t<ul class=\"nav navbar-nav";
        $output .= empty($variables['class']) ? "\"" : " {$variables['class']}\"";
        $output .= empty($variables['style']) ? "" : " style=\"{$variables['style']}\"";
        $output .= empty($variables['id']) ? "" : " id=\"{$variables['id']}\"";
        $output .= ">\n";
        foreach($links as $link) {
            if ($link instanceof html_li) {
                if (strlen($link->string())) {
                    $output .= "\t\t\t\t" . $link->string() . "\n";
                }
            } else if ($link instanceof html_a) {
                $t = "";
                $class = "";
                if ($link->is_active()) {
                    $class = "active ";
                }
                if (!empty($link->submenu)) {
                    $class .= "dropdown ";
                    $droplink = clone $link;
                    $droplink->href = "#";
                    $droplink->class = "dropdown-toggle";
                    $droplink->data_toggle = "dropdown";
                    $droplink->innerHTML .= " <b class=\"caret\"></b>";
                    $alink = $droplink->string();
                    $alink = strlen($alink) ? "\n{$alink}\n" . self::unordered_list($droplink->submenu, array("class" => "dropdown-menu")) : "";
                    $t = "\t\t\t\t";
                } else {
                    $alink = $link->string();
                }
                $class = $class ? " class=\"" . trim($class) . "\"" : "";
                if (strlen($alink)) {
                    $output .= "\t\t\t\t<li{$class}>{$alink}{$t}</li>\n";
                }
            } else if (strlen($link)) {
                $output .= "\t\t\t\t<li>" . $link . "</li>\n";
            }
        }
        /* close ul, div, nav */
        $output .= "\t\t\t</ul>\n";
        $output .= "\t\t</div><!-- /.navbar-collapse -->\n";
        $output .= "\t</div><!-- /.container-fluid -->\n";
        $output .= "</nav>\n";
        
        return $output;
    }
    
    /* class, id, style, divclass, divid, divstyle tfoot */
    public static function unordered_list($list_items, $variables = null) {
        $output = "";
        if (!is_array($list_items) || !count($list_items)) {
            return $output;
        }
        /* start wrapper div */
        $t = "";
        $divwrapper = "";
        if (!empty($variables['divid']) || !empty($variables['divclass']) || !empty($variables['divstyle'])) {
            $t = "\t";
            $divwrapper = "<div";
            $divwrapper .= empty($variables['divid']) ? "" : " id=\"{$variables['divid']}\"";
            $divwrapper .= empty($variables['divclass']) ? "" : " class=\"{$variables['divclass']}\"";
            $divwrapper .= empty($variables['divstyle']) ? "" : " style=\"{$variables['divstyle']}\"";
            $divwrapper .= ">\n";
        }
        /* start ul */
        $output .= "{$t}<ul";
        $output .= empty($variables['id']) ? "" : " id=\"{$variables['id']}\"";
        $output .= empty($variables['class']) ? "" : " class=\"{$variables['class']}\"";
        $output .= empty($variables['style']) ? "" : " style=\"{$variables['style']}\"";
        $output .= ">\n";
        foreach($list_items as $list_item) {
            if ($list_item instanceof html_li) {
                if (strlen($list_item->string())) {
                    $output .= "{$t}\t" . $list_item->string() . "\n";
                }
            } else if ($list_item instanceof html_a) {
                $class = "";
                if ($list_item->is_active()) {
                    $class = " class=\"active\"";
                }
                $submenu = "";
                if (!empty($list_item->submenu)) {
                    $s = clone $list_item;
                    $submenu = "\n" . self::unordered_list($s->submenu);
                }
                $string = $list_item->string();
                if (strlen($string)) {
                    $output .= "{$t}\t<li{$class}>{$string}{$submenu}</li>\n";
                }
            } else if (strlen($list_item)) {
                $output .= "{$t}\t<li>" . $list_item . "</li>\n";
            }
        }
        /* close ul and wrapper div */
        $output .= "{$t}</ul>\n";
        /* add wrapper div */
        $output = $divwrapper ? $divwrapper . $output . "</div>\n" : $output;
        
        return $output;
    }
    
    /* class, id, style, divclass, divid, divstyle tfoot */
    public static function ordered_list($list_items, $variables = null) {
        $output = "";
        if (!is_array($list_items) || !count($list_items)) {
            return $output;
        }
        /* start wrapper div */
        $t = "";
        $divwrapper = "";
        if (!empty($variables['divid']) || !empty($variables['divclass']) || !empty($variables['divstyle'])) {
            $t = "\t";
            $divwrapper = "<div";
            $divwrapper .= empty($variables['divid']) ? "" : " id=\"{$variables['divid']}\"";
            $divwrapper .= empty($variables['divclass']) ? "" : " class=\"{$variables['divclass']}\"";
            $divwrapper .= empty($variables['divstyle']) ? "" : " style=\"{$variables['divstyle']}\"";
            $divwrapper .= ">\n";
        }
        /* start ul */
        $output .= "{$t}<ol";
        $output .= empty($variables['id']) ? "" : " id=\"{$variables['id']}\"";
        $output .= empty($variables['class']) ? "" : " class=\"{$variables['class']}\"";
        $output .= empty($variables['style']) ? "" : " style=\"{$variables['style']}\"";
        $output .= ">\n";
        foreach($list_items as $list_item) {
            if ($list_item instanceof html_li) {
                if (strlen($list_item->string())) {
                    $output .= "{$t}\t" . $list_item->string() . "\n";
                }
            } else if ($list_item instanceof html_a && $list_item->is_active()) {
                if (strlen($list_item->string())) {
                    $output .= "{$t}\t<li class=\"active\">" . $list_item->string() . "</li>\n";
                }
            } else if (strlen($list_item)) {
                $output .= "{$t}\t<li>" . $list_item . "</li>\n";
            }
        }
        /* close ul and wrapper div */
        $output .= "{$t}</ol>\n";
        /* add wrapper div */
        $output = $divwrapper ? $divwrapper . $output . "</div>\n" : $output;
        
        return $output;
    }
    
    /* class, id, divclass, divid */
    public static function list_group($links, $variables = null) {
        $output = "";
        if (!is_array($links)) {
            return $output;
        }
        /* start wrapper div */
        $output = "<div";
        $output .= empty($variables['divid']) ? "" : " id=\"{$variables['divid']}\"";
        $output .= " class=\"list-group";
        $output .= empty($variables['divclass']) ? "" :  "  " . $variables['divclass'];
        $output .= "\">\n";
        /* links */
        foreach($links as $link) {
            if ($link instanceof html_abstract && strlen($link->string())) {
                $link->class = empty($link->class) ? "" : $link->class . " ";
                $link->class .= empty($variables['class']) ? "list-group-item" : "list-group-item " . $variables['class'];
                $output .= "\t" . $link->string() . "\n";
            }
        }
        /* close wrapper div */
        $output .= "</div>\n";
        
        return $output;
    }
    
    public static function menu($views, $views_user_can_view, $current_view, $render_menu_function) {
        return call_user_func($render_menu_function, $views, $current_view);
    }
    
    public static function error_message($message) {
        if (!$message) {
            return "";
        }
        
        return "<span class=\"help-block\"><i class=\"fa fa-warning\"></i> {$message}</span>";
    }
    
    public function modal($id, $title, $body, $variables = null) {
        if (!empty($variables['footer'])) {
            $footer = "<div class=\"modal-footer\">{$variables['footer']}</div>\n";
        }
        $size = "col-xs-12 col-sm-8";
        if (!empty($variables['size'])) {
            $size = "col-xs-{$variables['size']} col-sm-{$variables['size']}";
        }
        $output = <<<END
            <div class="modal fade" id="{$id}">
                <div class="modal-dialog">
                    <div class="modal-content">
                        <div class="modal-header">
                            <button type="button" class="close" data-dismiss="modal">×</button>
                            <h4 class="modal-title">{$title}</h4>
                        </div>
                        <div class="modal-body">{$body}</div>
                        {$footer}
                    </div>
                </div>
            </div>
END;

        return $output;
    }
    
    public function success($heading, $message) {
        $output = <<<END
            <div class="alert alert-success">
                <a class="close" data-dismiss="alert" href="#">×</a>
                <h4 class="alert-heading">{$heading}</h4>
                {$message}
            </div> 
END;
    
        return $output;
    }
    
    public function danger($heading, $message) {
        $output = <<<END
            <div class="alert alert-danger">
                <a class="close" data-dismiss="alert" href="#">×</a>
                <h4 class="alert-heading">{$heading}</h4>
                {$message}
            </div> 
END;
    
        return $output;
    }
    
    public function info($heading, $message) {
        $output = <<<END
            <div class="alert alert-info">
                <a class="close" data-dismiss="alert" href="#">×</a>
                <h4 class="alert-heading">{$heading}</h4>
                {$message}
            </div> 
END;
    
        return $output;
    }

}