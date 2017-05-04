<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 30.10.16
 * Time: 09:38
 */

function test_module_beschreibung_form($form, &$form_submit) {

    $reminder = check_current_open_protocol();

    if($reminder["exist"] == 1) {
        $temp = getContent();

        $form['test'] = array(
            '#title' => "Zusammenfassung des Treffens",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt werden abschließende Punkte zum Treffen behandelt."
        );

        $form['test']['problems'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Probleme, die während des Treffens auftraten'),
            '#default_value' => $temp["problems"],
        );

        $form['test']['conclusion'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Fazit des Treffens'),
            '#default_value' => $temp["conclusion"],
        );

        $form['test']['outlook'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Ausblick auf das nächste Treffen'),
            '#default_value' => $temp["outlook"],
        );

        $dateformat = 'Y-m-d H:i';
        $label = t('Ende des Treffens');
        $timestamp = time();
        $dbv = $timestamp;
        if (empty($dbv)) {
            $d = new DateTime('@'.$dbv);
            $value = $d->format($dateformat);
        } else {
            $d = new DateTime();
            $value = $d->format($dateformat);
        }

        $form['test']['deadline'] = array(
            '#type' => 'date_popup',
            '#date_timezone' => date_default_timezone(),
            '#date_format' => $dateformat,
            '#date_year_range' => '-3:+3',
            '#title' => $label,
            '#require' => TRUE,
            '#default_value' => $value,
        );


        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('back_to_new_task')

        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Protokoll abspeichern',
            '#submit' => array('save_protocol')
        );

    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
<a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
', "error");    }

    return $form;
}

function back_to_new_task($form, &$form_state) {
    saveData($form, $form_state);
    drupal_goto('/protokoll_neue_aufgaben');
}

function save_protocol($form, &$form_state) {
    saveData($form, $form_state);
    finish_protocol();
    drupal_goto("/protokoll_uebersicht");
}

function saveData($form, &$form_state) {
    $problems = $form_state['values']['problems'];
    $conclusion = $form_state['values']['conclusion'];
    $outlook = $form_state['values']['outlook'];
    $list = getParticipants2();
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid  IN ($list) AND pro.finished = 0";
    $tempresult = db_query($sql);
    $found = 0;
    $protocol_id = 0;

    foreach ($tempresult as $temp) {
        $found = $found +1;
        $protocol_id = $temp->protocol_id;
    }

    if ($found >0) {
        $raum = getPosition($protocol_id, "room");
        $phase = getPosition($protocol_id, "phase");
        $deadline = $form_state['values']['deadline'];
        $sql = "UPDATE {protocol} SET outlook = '$outlook', end_date = '$deadline', raum_end = '$raum', phase_end = '$phase', problems = '$problems', conclusion = '$conclusion' WHERE uid IN ($list) AND finished = 0";
        db_query($sql);
    }
}

function getPosition($protocol_id, $object) {
    $room = "";
    $phase = "";
    $sql = "SELECT * FROM {method_execution} WHERE protocol_id = ".$protocol_id." ORDER BY method_execution_id DESC LIMIT 1";
    $results = db_query($sql);

    if ($results->rowCount() > 0){
        foreach ($results as $item) {
            $room = $item->raum_end;
            $phase = $item->phase_end;
        }
    } else {
        $parts = getParticipants2();
        $sql = "SELECT * FROM {protocol} WHERE uid in (".$parts.") AND protocol_id != ".$protocol_id. " ORDER BY protocol_id DESC LIMIT 1";
        $results = db_query($sql);
        if ($results->rowCount() > 0){
            foreach ($results as $item) {
                $room = $item->raum_end;
                $phase = $item->phase_end;
            }
        } else {
            $room = "Problemraum";
            $phase = "divergent";
        }
    }

    if(!strcmp($object, "room")) {
        return $room;
    } else {
        return $phase;
    }
}

function getLastPhase($protocol_id) {
    $phase = "";
    $sql = "SELECT * FROM {method_execution} WHERE protocol_id = ".$protocol_id." ORDER BY method_execution_id DESC LIMIT 1";
    $result = db_query($sql);

    if($result->num)

    foreach ($result as $item) {
        drupal_set_message($item->method_execution_id);
        $phase = $item->phase;
    }

    return $phase;
}


function finish_protocol() {
    $parts = getParticipants2();
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($parts) AND pro.finished = 0";
    $result = db_query($sql);
    foreach($result as $item) {
        $p_id = $item->protocol_id;
        $sql = "UPDATE {protocol} as p SET finished = 1 WHERE p.protocol_id = $p_id AND p.finished = 0";
        db_query($sql);
    }
}


function getContent() {

    $results = array();
    global $user;
    $user_id = $user->uid;

    $list = getParticipants2();

    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($list) ORDER BY pro.protocol_id DESC LIMIT 1";

    $result = db_query($sql);
    foreach($result as $item) {
        //drupal_set_message("Es besteht bereits ein Protokoll");
        $results["problems"] = $item->problems;
        $results["conclusion"] = $item->conclusion;
        $results["raum_end"] = $item->raum_end;
        $results["phase_end"] = $item->phase_end;
        $results["outlook"] = $item->outlook;
    }
    return $results;
}

