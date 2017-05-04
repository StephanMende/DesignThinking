<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.11.16
 * Time: 20:22
 */


//Diese Funktion lädt alle Gruppenmitglieder des angemeldeten Nutzers aus der Datenbank
function protocol_template_get_participants_from_logged_in_user_group()
{
    $users = array();
    global $user;
    $user_id = $user->uid;

    $sql = "SELECT ur.rid, r.name, ur.uid FROM {role} AS r INNER JOIN {users_roles} AS ur ON r.rid = ur.rid WHERE ur.rid = (SELECT usro.rid FROM {users_roles} as usro WHERE usro.uid = $user_id)";
    $role_result = db_query($sql);

    foreach ($role_result as $role) {
        array_push($users, $role->uid);
    }

    $user_list = implode(',', $users);

    return $user_list;
}

//Diese Funktion lädt alle Gruppenmitglieder des angemeldeten Nutzers aus der Datenbank
function protocol_template_get_participants_by_protocol($protocol_id)
{
    $protocol = protocol_template_get_protocol_from_id($protocol_id);

    $users = array();

    $author = $protocol->uid;
    $user = user_load($author);
    $user_id = $user->uid;

    $sql = "SELECT ur.rid, r.name, ur.uid FROM {role} AS r INNER JOIN {users_roles} AS ur ON r.rid = ur.rid WHERE ur.rid = (SELECT usro.rid FROM {users_roles} as usro WHERE usro.uid = $user_id)";
    $role_result = db_query($sql);

    foreach ($role_result as $role) {
        array_push($users, $role->uid);
    }

    $user_list = implode(',', $users);

    return $user_list;
}


/*
 * Diese FUnktion lädt das aktuell offene Protokoll einer Gruppe aus der Datenbank. Die Mitglieder der Gruppe
 * werden als Übergabeparameter übergeben
 */
function protocol_template_get_current_open_group_protocol($participants)
{

    $protocol_id = 0;
    $sql = "SELECT * FROM {protocol} as p WHERE p.uid IN ($participants) AND p.finished = 0";
    $protocol_result = db_query($sql);

    foreach ($protocol_result as $protocol) {
        $protocol_id = $protocol->protocol_id;
    }

    return $protocol_id;
}

/*
 * Diese Funktion überprüft, ob für die Gruppe des aktuell angemeldeten Nutzers bereits ein offenes Protokoll
 * existiert oder nicht
 */
function protocol_template_check_current_open_protocol()
{
    $open_protocol["exist"] = 0;
    $open_protocol["protocol_id"] = 0;
    $user_list = protocol_template_get_participants_from_logged_in_user_group();

    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($user_list) AND pro.finished = 0";
    $protocol_result = db_query($sql);

    foreach ($protocol_result as $protocol) {
        $open_protocol["exist"] = 1;
        $open_protocol["protocol_id"] = $protocol->protocol_id;
    }
    return $open_protocol;
}