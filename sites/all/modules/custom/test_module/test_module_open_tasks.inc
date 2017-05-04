<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:43
 */

function test_module_offeneAufgabenForm($form, &$form_submit) {

    $reminder = check_current_open_protocol();

    if($reminder["exist"] == 1) {
        $test = get_task_table();
        $test2 = get_second_table();


        $form['test'] = array(
            '#title' => "Übersicht über Aufgaben aus dem letzten Protokoll",
            '#type' => 'fieldset',
            '#description' => "In dieser Tabelle werden alle Aufgaben angezeigt, die innerhalb des letzten Gruppentreffens angelegt wurden."
        );

        $form['test']['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $test
        );


        $form['test2'] = array(
            '#title' => "Übersicht über alle noch offenen Aufgaben",
            '#type' => 'fieldset',
            '#description' => "In dieser Tabelle werden alle Aufgaben angezeigt, die bis zu diesem Zeitpunkt noch nicht erledigt wurden."
        );

        $form['test2']['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $test2
        );



        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('back_to_general_information')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('submit_method_overview')
        );

    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
<a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
', "error");    }


    return $form;

}

function get_task_table() {

    global $user;
    $users = getParticipants5();
    $protocol_id = get_open_protocol4($users);

    $sql = "SELECT * FROM {task} AS t WHERE t.uid IN ($users) AND t.done = 0 ORDER BY t.deadline";

    $proresult = db_query($sql);

    $sql2 = "SELECT * FROM {protocol} WHERE uid IN ($users)";

    $protocol_result = db_query($sql2);
    $counter = 0;
    $id = 0;
    $last_id = 0;
    foreach ($protocol_result as $prores) {
        if ($protocol_id == $prores->protocol_id) {
            $last_id = $id;
        }
        $id = $prores->protocol_id;
        $counter++;
    }

    $tempString = "<table><th>Kurzbezeichnung</th><th>zugehöriger Benutzer (Wer?)</th><th>Aufgabe (Was?</th><th>Deadline (Wann?)</th><th>Status</th><th>Aktionen</th>";

    $sql = "SELECT * FROM {task} AS t WHERE t.uid IN ($users) AND t.protocol_id = ".$last_id." ORDER BY t.deadline";

    $protocol_task_result = db_query($sql);
    foreach($protocol_task_result as $protare) {
        $date = new DateTime($protare->deadline);
        $deadline2 = $date->format('d.m.Y');
        $deadline3 = $date->format('d.m.Y H:i:s');
        $user = user_load($protare->uid);
        $task_id = $protare->task_id;

        if ($protare->done) {
            $done = "<font color=\"green\">erledigt</font><td></td>";
        } else {
            $done = "<font color=\"red\">offen</font></td><td><a href='/design_thinking_tool/delete_task/$task_id/pXs8hYVUqe'/>erledigt</a></td></tr></tr>";
        }

        $tempString = $tempString . "<tr><td>$protare->short_description</td><td>".$user->field_first_name['und'][0]['value']." ". $user->field_last_name['und'][0]['value']."</td>
            <td>$protare->task</td><td>$deadline3</td>
            <td>".$done;
    }

    $tempString .= "</table><br>";


    return $tempString;
}

function get_second_table() {
    global $user;
    $user_id = $user->uid;
    $users = getParticipants5();
    $protocol_id = get_open_protocol4($users);

    $sql = "SELECT * FROM {task} AS t WHERE t.uid IN ($users) AND t.done = 0 ORDER BY t.deadline";

    $proresult = db_query($sql);

    $tempString = "<table><th>Kurzbezeichnung</th><th>zugehöriger Benutzer (Wer?)</th></th><th>Aufgabe (Was?</th> <th>Deadline (Wann?)</th><th>Status</th><th>Aktionen</th>";
    foreach($proresult as $pro) {
        $date = new DateTime($pro->deadline);
        $deadline2 = $date->format('d.m.Y');
        $deadline3 = $date->format('d.m.Y H:i:s');
        $task_id = $pro->task_id;


        if ($pro->done == 0 && $pro->protocol_id != $protocol_id) {

            $user = user_load($pro->uid);
            if ($pro->done) {
                $done = "<font color=\"green\">erledigt</font>";
            } else {
                $done = "<font color=\"red\">offen</font></td><td><a href='/design_thinking_tool/delete_task/$task_id/pXs8hYVUqe'>erledigt</a></td></tr></tr>";
            }

            /*
            if(strlen($pro->task) >170) {
                $pro->task = substr($pro->task,0,170);
                $filename = strrpos($pro->task, " ");
                $pro->task = substr($pro->task,0,$filename);

                $pro->task .= " ...";
            }*/

            $tempString = $tempString . "<tr><td>$pro->short_description</td><td>".$user->field_first_name['und'][0]['value']." ". $user->field_last_name['und'][0]['value']."</td>
            <td>$pro->task</td><td>$deadline3</td>
            <td>".$done;
        }
    }
    $tempString .= "</table>";

    return $tempString;
}

function get_open_protocol4($participants) {

    $protocol_id = 0;
    $sql = "SELECT * FROM {protocol} as p WHERE p.uid IN ($participants) AND p.finished = 0";
    $result = db_query($sql);

    foreach ($result as $r) {
        $protocol_id = $r->protocol_id;
    }

    return $protocol_id;
}

function submit_method_overview($form, &$form_state) {
    drupal_goto('/protokoll_methoden_uebersicht');
}

function back_to_general_information($form, &$form_state) {
    drupal_goto('/protokoll_vorbereitung');
}

function getParticipants5() {
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

