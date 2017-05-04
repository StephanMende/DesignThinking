<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:13
 */

function test_module_new_task_form($form, &$form_submit) {

    $reminder = check_current_open_protocol();

    if($reminder["exist"] == 1) {

        $test = get_new_table();

        $form['test'] = array(
            '#title' => "Übersicht über neue Aufgaben",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt werden alle im Rahmen dieses Treffens erstellten Aufgaben aufgelistet."
        );

        $form['test']['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $test,
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('back_to_method_overview')
        );

        $form['new_task'] = array(
            '#type' => 'submit',
            '#value' => 'neue Aufgabe anlegen',
            '#submit' => array('new_task_execution')
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

function get_new_table() {

    global $user;
    $list = getParticipants2();
    $protocol_id = 0;

    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($list) AND pro.finished = 0";
    $result = db_query($sql);

    foreach($result as $item) {
        $protocol_id = $item->protocol_id;
    }

    $sql = "SELECT * FROM {task} AS t WHERE t.protocol_id = :pro_id ORDER BY t.deadline";
    $proresult = db_query($sql, array(
        ':pro_id' => $protocol_id,
    ));

    $tempString = "<table><th>Kurzbezeichnung</th><th>zugehöriger Benutzer (Wer?)</th><th> Aufgabe (Was?)</th> <th>Deadline (Wann?)</th><th>Aktionen</th>";
    foreach($proresult as $pro) {

        if ($pro->done == 0) {
            $user = user_load($pro->uid);
            $date = new DateTime($pro->deadline);
            $deadline3 = $date->format('d.m.Y H:i:s');
            $tempString = $tempString . "<tr><td>$pro->short_description</td><td>".$user->field_first_name['und'][0]['value']." ". $user->field_last_name['und'][0]['value']."</td><td>$pro->task</td><td>$deadline3</td><td><a href='/design_thinking_tool/delete_task/$pro->task_id/t8HVhPRlPZ'>löschen</a></td></tr></tr>";
        }

    }
    $tempString .= "</table>";


    return $tempString;
}

function new_task_execution($form, &$form_state) {
    drupal_goto('/protokoll_neue_Aufgabendurchführung');
}

function submit_method_overview($form, &$form_state) {
    drupal_goto('/protokoll_beschreibungen');
}

function back_to_method_overview($form, &$form_state) {
    drupal_goto('protokoll_methoden_uebersicht');
}