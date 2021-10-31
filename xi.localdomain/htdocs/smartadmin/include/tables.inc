<h2>Tables</h2>
<?php

    $db = new db();
    $sql = "select * from devdata1 where ukdate is not null limit 80";
    $rows = $db->fetch_all($sql);
    
    echo widget::sorttable(htmlentities("widget::sorttable(\"title\", \$rows);"), $rows);
    
    echo widget::scrolltable(htmlentities("widget::scrolltable(\"title\", \$rows);"), $rows);
    
    $sql = "
        select
            d.*,
            d.name as name1,
            d.name as name2,
            d.name as name3,
            d.name as name4,
            d.name as name5,
            d.name as name6
        from devdata1 as d 
        where ukdate is not null
        order by d.datetime
        limit 80
    ";
    $rows = $db->fetch_all($sql);
    echo widget::fixedtable(htmlentities("widget::fixedtable(\"title\", \$rows);"), $rows);
    
?>