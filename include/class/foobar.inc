<?php

/********************
 *
 * Author:      Des Jones
 * Project:     xi
 * Date:        Mon 25 Oct 19:10:13 BST 2021
 * Version:     1.0
 *
 *****************************************/

class foobar extends model {

    public function tablename() {
        return "foobar";
    }

//     public function sql_load($id) {
//         $sql_where = $this->sql_where($id);
//         if (!$sql_where) {
//             return $return;
//         }
// 
//         $phone = new phone();
//         $manager = new user();
//         $manager->alias("manager");
//         $manager_phone = new phone();
//         $manager_phone->alias("managerphone");
//         $sql = $this->sql()->
//             select()->
//                 field($this, new expression("now()"), "today")-> 
//                 all($this)->
//                 all($phone)->
//                 all($manager)->
//                 all($manager_phone);
//         $expressions = $this->load_expressions();
//         if (is_array($expressions)) {
//             foreach ($expressions as $key => $value) {
//                 if ($value instanceof expression) {
//                     $sql->field($this, $value, $key);
//                 }
//             }
//         }
//         $sql->from($this)->
//         
//         left_join($phone)->
//         on($this, "userid", $phone, "userid")->
//         
//         join($manager)->
//         on($this, "managerid", $manager, "userid")->
//         
//         left_join($manager_phone)->
//         on($manager, "userid", $manager_phone, "userid")->
//     
//         expression($sql_where)->
//         order_by()->field($phone, "phoneid")->desc();
// 
//         return $sql;
//     }

}
