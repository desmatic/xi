<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/
 
class widget {

    private static $_unqiueid;
    
    private static function uniqueid() {
        if (self::$_unqiueid == null) {
            self::$_unqiueid = 0;
        }
        self::$_unqiueid++;
        
        return self::$_unqiueid;
    }
    
    /* fullwidth, nopadding, lightborder, toolbar, widgetid */
    public static function basic($title, $content, $variables = null) {
        if (empty($variables['widgetid'])) {
            $variables['widgetid'] = "widgetid" . self::uniqueid();
        }
        if (!empty($variables['nopadding'])) {
            $variables['nopadding'] = " no-padding";
        } else {
            $variables['nopadding'] = "";
        }
        if (!empty($variables['lightborder'])) {
            $variables['lightborder'] = "";
        } else {
            $variables['lightborder'] = " jarviswidget-color-darken";
        }
        if (!empty($variables['fullwidth'])) {
            $article = "<article class=\"col-sm-12 col-md-12 col-lg-12\">\n";
        } else {
            $article = "<article class=\"col-sm-12 col-md-12 col-lg-6\">\n";
        }
        if ($title == strip_tags($title)) {
            $title = "<h2>{$title}</h2>";
        }
        $comment_title = "widget " . strip_tags($title);
        $output = "<!-- {$comment_title} -->\n";
        $output .=  $article;
        /* widget wrapper */
        $output .= "<div class=\"jarviswidget{$variables['lightborder']}\" id=\"{$variables['widgetid']}\" data-widget-editbutton=\"false\">\n";
        $output .= "\t<header>{$title}</header>\n";
        $output .= "\t<div>\n";
        /* widget edit box */
        $output .= "\t\t<div class=\"jarviswidget-editbox\"></div>\n";
        /* widget content */
        $output .= "\t\t<div class=\"widget-body{$variables['nopadding']}\">\n";
        if (isset($variables['toolbar'])) {
            $output .= "\t\t\t<div class=\"widget-body-toolbar\">{$variables['toolbar']}</div>\n";
        }
        $output .= $content;
        /* end widget content */
        $output .= "\n\t\t</div>\n";
        /* end widget wrapper */
        $output .= "\t</div>\n";
        $output .= "</div>\n";
        $output .= "</article>\n";
        $output .= "<!-- end {$comment_title} -->\n";
        
        return $output;
    }
    
    public static function form($title, $content, $variables) {
        $output = "";
        if ($content) {
            $variables['nopadding'] = true;
            $variables['lightborder'] = true;
            $output = self::basic($title, $content, $variables);
        }
        
        return $output;
    }
    
    public static function sorttable($title, $rows, $variables = null) {
        $output = "";
        $content = template::sorttable($rows, $variables);
        if ($content) {
            $variables['nopadding'] = true;
            $variables['fullwidth'] = true;
            $output = self::basic($title, $content, $variables);
        }
        
        return $output;
    }
    
    public static function scrolltable($title, $rows, $variables = null) {
        $output = "";
        $content = template::scrolltable($rows, $variables);
        if ($content) {
            $variables['nopadding'] = true;
            $variables['fullwidth'] = true;
            $output = self::basic($title, $content, $variables);
        }
        
        return $output;
    }
    
    public static function fixedtable($title, $rows, $variables = null) {
        $output = "";
        $content = template::fixedtable($rows, $variables);
        if ($content) {
            $variables['nopadding'] = true;
            $variables['fullwidth'] = true;
            $output = self::basic($title, $content, $variables);
        }
        
        return $output;
    }

}