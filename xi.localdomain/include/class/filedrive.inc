<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class filedrive extends model {

    public function tablename() {
        return "filedrive";
    }
    
    public static function abbreviate_mimetype($mimetype) {
        $parts = explode(";", $mimetype);
        return $parts[0] ? trim($parts[0]) : "";
    }
    
    public function extension($mimetype) {
        $mimetypes["application/pdf; charset=binary"] = "pdf";
        $mimetypes["application/x-gzip; charset=binary"] = "tar";
        $mimetypes["application/zip; charset=binary"] = "zip";
        $mimetypes["image/jpeg; charset=binary"] = "jpeg";
        $mimetypes["image/png; charset=binary"] = "png";
        $mimetypes["text/html; charset=utf-8"] = "html";
        $mimetypes["text/plain; charset=us-ascii"] = "txt";
        $mimetypes["text/x-php; charset=us-ascii"] = "txt";
        $mimetypes["text/x-c++; charset=us-ascii"] = "txt";
        
        return isset($mimetypes[$mimetype]) ? $mimetypes[$mimetype] : "bin";
    }
    
    public static function is_image($mimetype) {
        $mimetypes["image/jpeg; charset=binary"] = "jpeg";
        $mimetypes["image/png; charset=binary"] = "png";
        
        return isset($mimetypes[$mimetype]) ? $mimetypes[$mimetype] : false;
    }
    
    public static function file_thumbnail($filename, $x, $crop = null) {
        $i = new Imagick($filename);
        $width = $i->getImageWidth();
        $height = $i->getImageHeight();
        if (!$crop) {
            if ($width > $height) {
                $i->thumbnailImage($x, 0);
            } else {
                $i->thumbnailImage(0, $x);
            }
        } else {
        
        }
        
        $thumb->writeImage($thumbnailFilename);
    }
    
    public static function get_file_mimetype($filename) {
        return trim(exec("file -ib " . $filename));
    }
    
    public static function get_uploaded_file($index = "file") {
        $f = null;
        if (
            (isset($_FILES)) &&
            (isset($_FILES[$index]['tmp_name'])) &&
            (is_uploaded_file($_FILES[$index]['tmp_name'])) &&
            ($fhdl = fopen($_FILES[$index]['tmp_name'], "r")) &&
            ($file = fread($fhdl, $_FILES[$index]['size']))
        ) {
            fclose($fhdl);
            $f = new filedrive();
            $f->mimetype = self::get_file_mimetype($_FILES[$index]['tmp_name']);
            $f->filename = $_FILES[$index]['name'];
            $f->bytes = strlen($file);
            $f->md5sum = md5($file);
            $f->file = base64_encode($file);
        }
        
        return $f;
    }

    public static function slider_thumbnail($file, $width = 896, $height = 504) {
        $thumbfilename = $file . ".thumbnail" . $width . "x" . $height;
        $i = new Imagick($file);
        $i->cropThumbnailImage($width, $height);
        $i->writeImage($thumbfilename);
        $i->destroy();
        
        return file_get_contents($thumbfilename);
    }
    
    public static function thumbnail($file, $max = 504) {
        $thumbfilename = $file . ".thumbnail" . $max;
        $i = new Imagick($file);
        $w = $i->getImageWidth();
        $h = $i->getImageHeight();
        if ($w > $h) {
            $i->thumbnailImage($max, 0);
        } else {
            $i->thumbnailImage(0, $max);
        }
        $i->writeImage($thumbfilename);
        $i->destroy();
        
        return file_get_contents($thumbfilename);
    }
    
    public static function square_thumbnail($file, $max = 32) {
        $thumbfilename = $file . ".thumbnail" . $max;
        $i = new Imagick($file);
        $i->cropThumbnailImage($max, $max);
        $i->writeImage($thumbfilename);
        $i->destroy();
        
        return file_get_contents($thumbfilename);
    }
    
}
