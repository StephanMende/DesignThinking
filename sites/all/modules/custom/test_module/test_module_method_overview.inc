<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:55
 */

function test_module_methodenForm($form, &$form_submit) {

    $reminder = check_current_open_protocol();

    if($reminder["exist"] == 1) {
        $test = show_methods_table();

        $form['test'] = array(
            '#title' => "Übersicht bearbeiteter Methoden",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt werden alle im Rahmen dieses Treffens bearbeitete Methoden aufgelistet."
        );

        $form['test']['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $test,
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('back_to_open_tasks')
        );

        $form['new_method'] = array(
            '#type' => 'submit',
            '#value' => 'neue Methode durchführen',
            '#submit' => array('new_method_execution')
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('new_tasks')
        );

    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
<a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
', "error");    }



    return $form;
}

function back_to_open_tasks() {
    drupal_goto('/protokoll_offene_aufgaben');
}

function new_method_execution() {
    drupal_goto('/protokoll_neue_Methodendurchführung/0');
}

function new_tasks() {
    drupal_goto('/protokoll_neue_aufgaben');
}

function show_methods_table() {
    $result = get_relevant_methods5();

    $tempString = "<table><th>Methodenname</th> <th> Bewertung</th><th> Aktion</th></th>";

    foreach ($result as $r) {
        $method_id = $r->method_execution_id;
        $image = "";
        if($r->rating == 20) {
            $image = "one.png";
        } else if($r->rating == 40) {
            $image = "two.png";
        } else if($r->rating == 60) {
            $image = "three.png";
        } else if($r->rating == 80) {
            $image = "four.png";
        } else if($r->rating == 100) {
            $image = "five.png";
        }

        global $base_url;

        $tempString = $tempString . "<tr width=\"10%\"><td><a href='$base_url/methodenanzeige/$method_id/pXs8hYVUqe'>$r->name</a></td>
            <td style=\" text-align:center\"><img src=".$image." height=\"120\" width=\"120\"> </td>
            <td><a href='$base_url/protokoll_methode_aendern/$method_id/pXs8hYVUqe'>bearbeiten</a></td></tr></tr>";
    }

    $tempString .= "</table>";

    return $tempString;
}

function get_relevant_methods5() {
    $participants = getParticipants3();
    $protcol_id = get_open_protocol($participants);

    $sql = "SELECT * FROM {method_execution} as m WHERE m.protocol_id = $protcol_id";
    $result = db_query($sql);

    return $result;
}

function get_open_protocol($participants) {

    $protocol_id = 0;
    $sql = "SELECT * FROM {protocol} as p WHERE p.uid IN ($participants)";
    $result = db_query($sql);

    foreach ($result as $r) {
        $protocol_id = $r->protocol_id;
    }

    return $protocol_id;
}


function getParticipants3() {
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