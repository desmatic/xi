<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class html_workflow extends html_abstract {

    protected function _string() {
        $output = "<a";
        if ($this->href) {
            $request = "";
            if ($this->request) {
                $request = is_array($this->request) ? self::array2url($this->request) : $this->request;
                $request = strpos($this->href, "?") ? "&amp;" . $request :  "?" . $request;
            }
            $output .= " href=\"{$this->href}{$request}\"";
        }
        $output .= $this->target ? " target=\"{$this->target}\"" : "";
        $output .= $this->id ? " id=\"{$this->id}\"" : "";
        $output .= $class ? " class=\"{$class}\"" : "";
        $output .= $this->title ? " title=\"{$this->title}\"" : "";
        $output .= $this->onclick ? " onclick=\"{$this->onclick}\"" : "";
        $output .= $this->data_toggle ? " data-toggle=\"{$this->data_toggle}\"" : "";
        $output .= ">\n";
        $output .= "\t<div class=\"workflow workflowgreen\">\n";
        $output .= "\t\t<h2>";
        $output .= $this->count ? $this->count : "*";
        $output .= "</h2>\n";
        $output .= "\t\t<div class=\"workflowcontent\">\n\t\t\t";
        $output .= $this->innerHTML ? $this->innerHTML : $this->href;
        $output .= "\n\t\t</div>\n";
        $output .= "\t</div>\n";
        $output .= "</a>\n";
        
        return $output;
    }
    
}