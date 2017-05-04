<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.11.16
 * Time: 20:22
 */


function getParticipants2() {
    $users = array();
    global $user;
    $user_id = $user->uid;

    $sql = "SELECT ur.rid, r.name, ur.uid FROM {role} AS r INNER JOIN {users_roles} AS ur ON r.rid = ur.rid WHERE ur.rid = (SELECT usro.rid FROM {users_roles} as usro WHERE usro.uid = $user_id)";

    $proresult = db_query($sql);

    foreach($proresult as $pro) {
        array_push($users, $pro->uid);
    }

    $list = implode(',', $users);

    return $list;
}

function get_open_protocol2($participants) {

    $protocol_id = 0;
    $sql = "SELECT * FROM {protocol} as p WHERE p.uid IN ($participants) AND p.finished = 0";
    $result = db_query($sql);

    foreach ($result as $r) {
        $protocol_id = $r->protocol_id;
    }

    return $protocol_id;
}

function check_current_open_protocol() {
    $reminder["exist"] = 0;
    $reminder["protocol_id"] = 0;
    $list = getParticipants2();

    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($list) AND pro.finished = 0";


    $result =db_query($sql);

    foreach($result as $r) {
        $reminder["exist"] = 1;
        $reminder["protocol_id"] = $r->protocol_id;
    }
    return $reminder;
}