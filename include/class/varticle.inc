<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class varticle extends model {

    public function tablename() {
        return "varticle";
    }
    
    public function article_search($companyid, $offset = 0, $count = 500) {    
        $a = new varticle();
        $sql = $this->sql()->
            select()->
                field($a, "articleid", "ID")->
                field($a, "nickname", "Author")->
                field($a, "title", "Title")->
                field($a, "datetimestart", "Starts")->
                field($a, "datetimeend", "Ends")->
                field($a, "datepaused", "Paused")->
            from($a)->
            order_by()->field($a, "datepinned")->desc()->
            field($a, "datetimestart")->desc()->
            limit($offset, $count);
        $rows = $sql->fetch_all();
        $return = array();
        if ($rows) {
            foreach($rows as $row) {
                $row['Title'] = "<a href=\"/article/article.html?article[articleid]={$row['ID']}\" title=\"Edit Article\">{$row['Title']}<a>";
                $return[] = $row;
            }
        }

        return $return;
    }
    
    public function article_list($companyid, $offset = 0, $count = 10) {
        $v = new varticle();
        $ua = new article_user_answer();
        $rows = $this->sql()->
            select()->
                all($v)->
                field($ua, "answer")->
            from($v)->
            left_join($ua)->
            on($v, "articleid", $ua, "articleid")->
            a()->field($ua, "userid")->eq()->e(session::userid())->
            where()->field($v, "datepaused")->is_null()->
            a()->field($v, "datetimestart")->e("<")->sysdate()->
            a()->field($v, "datetimeend")->e(">")->sysdate()->
            order_by()->field($v, "datepinned")->desc()->
            field($v, "datetimestart")->desc()->
            limit($offset, $count)->
            fetch_all_cached(0);
            
        /* if no articles are found, display the first article */
        if (!$rows) {
            $rows = $this->sql()->
                select()->
                    all($v)->
                    field($ua, "answer")->
                from($v)->
                left_join($ua)->
                on($v, "articleid", $ua, "articleid")->
                a()->field($ua, "userid")->eq()->e(session::userid())->
                where()->field($v, "articleid")->eq()->e(1)->
                fetch_all_cached(0);
        }


        return $rows;
    }
    
}
