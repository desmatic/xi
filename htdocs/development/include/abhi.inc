<pre>
<?php

    echo "<h3>SQL Example</h3>\n\n";

    $view = '09123412341';

    $user = new user();
    $user->userid = 4;

    $phone = new phone();

    $phone->phone = 2;

    if ($view) {
        $view = $this->db()->sql()->a()->field("p", "phone")->expression("=")->quote($view);
    }
    $sql = $this->db()->sql()->
        select()->
            //all(p)->
            all()->
            field("p", "phone", "Mobile Phone")->
            field("u", "forenames", "First name")->
            //field("c", "candoid")->
            //field("p", "title")->
            //field("p", "name")->
            //field("p", "description")->
            //field("p", "keywords")->
            //field("a", "articleid")->
            //field("a", "article")->
            //field("a", "datecreated")->
            //field("pp", "position")->
            //field("pa", "method")->
  
        from("user", "u")->

        join("phone", "p")->
        using("userid")->

        where()->field("u", "userid")->expression("=")->expression(4)->
        expression($view)->
        //a()->field("p", "datedeleted")->expression("is null")->
        //a()->field("a", "datedeleted")->expression("is null")->
        //a()->field("a", "datepaused")->expression("is null")->
        //a()->p()->field("a", "datestart")->expression("is null")->o()->sysdate()->expression(">")->field("a", "datestart")->q()->
        //a()->p()->field("a", "dateend")->expression("is null")->o()->field("a", "dateend")->expression("<")->sysdate()->q()->
        //a()->p()->field("pa", "articleid")->expression("is not null")->o()->field("pa", "method")->expression("is not null")->q()->
        //order_by()->field("pp", "position")->field("pa", "priority")->desc()->field("a", "datecreated")->desc();
        order_by()->field("p", "userid")->desc();

    echo $sql;

//     $rows = $sql->load();
//     echo print_r($rows, true);
//     echo print_r($sql, true);
?>
</pre>