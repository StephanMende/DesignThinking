<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:40
 */

function test_module_allgemeineForm($form, &$form_submit) {

    $reminder = check_current_open_protocol();

    if($reminder["exist"] == 1) {
        $types = array(
            "gruppenintern" => t("gruppenintern"),
            "gesamter Kurs" => t("gesamter Kurs"),
            "Präsentation" => t("Präsentation"),
        );

        $list = getParticipants2();
        $list = explode(',', $list);
        $new_parts = array();

        foreach ($list as $p) {
            $user_details = user_load($p);
            $username = $user_details->field_first_name['und'][0]['value'] . " " . $user_details->field_last_name['und'][0]['value'];
            $new_parts[$p] = $username;
            $new_parts[$p] = $username;
        }
        $temp = get_temp_content_for_general_information();

        $parts = explode(",",$temp["participants"]);

        $form['test2'] = array(
            '#title' => "allgemeine Informationen",
            '#type' => 'fieldset',
            '#description' => "Im ersten Schritt des Protokolls müssen zunächst allgemeine Informationen zum Treffen eingegeben werden."
        );


        $form['test2']['teilnehmer'] = array(
            '#title' => t('Teilnehmer des Treffens'),
            '#type' => 'select',
            '#multiple' => true,
            '#options' => $new_parts,
            '#default_value' => $parts,
            '#required' => TRUE,
        );

        $dateformat = 'Y-m-d H:i';
        $label = t('Beginn des Treffens');
        $timestamp = time();
        $dbv = $timestamp;
        if (empty($dbv)) {
            $d = new DateTime('@'.$dbv);
            $value = $d->format($dateformat);
        } else {
            $d = new DateTime();
            $value = $d->format($dateformat);
        }

        $form['test2']['start'] = array(
            '#type' => 'date_popup',
            '#date_timezone' => date_default_timezone(),
            '#date_format' => $dateformat,
            '#date_year_range' => '-3:+3',
            '#title' => $label,
            '#required' => TRUE,
            '#default_value' => $temp["start_date"],
        );

        $form['test2']['place'] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => t('Ort des Treffens'),
            '#default_value' => $temp["place"],
            '#required' => TRUE,
        );

        $form['test2']["type"]["plugin_select"] = array(
            "#type" => "select",
            "#title" => t("Art des Treffens"),
            "#default_value" => $temp["type"],
            "#options" => $types,
            '#required' => TRUE,
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('submit_general_information')
        );

    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
<a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
', "error");
    }


        return $form;
}

function submit_general_information($form, &$form_state) {

    $users = $form_state['values']['teilnehmer'];
    $users = implode(',', $users);

    global $user;
    $user_id = $user->uid;

    $types = array(
        "1" => t("gruppenintern"),
        "2" => t("gesamter Kurs"),
        "3" => t("Präsentation"),
    );

    $place = $form_state['values']['place'];
    $type = $form_state['values']['plugin_select'];
    $date = date("Y-m-d H:i:s");
    $start = $form_state['values']['start'];

    $list = getParticipants2();

    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid  IN ($list) AND pro.finished = 0";

    $tempresult = db_query($sql);
    $found = 0;
    foreach ($tempresult as $temp) {
        $found = $found +1;
    }

    if ($found >0) {
        $sql = "UPDATE {protocol} SET start_date = '".$start."', participants = '$users', place = '$place', type = '$type' WHERE uid = $user_id";
        $result = db_query($sql);

    } else {
        $sql = "INSERT INTO {protocol} (place,type,uid,date, participants, start_date) VALUES('$place', '$type', $user_id, '$date', '$users', '$start')";
        $result = db_query($sql);
    }
    drupal_goto('/protokoll_vorbereitung');
}

function get_temp_content_for_general_information() {
    $results = array();
    global $user;
    $user_id = $user->uid;
    $list = getParticipants2();
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($list) AND pro.finished = 0";
    $result = db_query($sql);

    $results["description"] = "";
    $results["place"] = "";
    $results["type"] = "";
    $results["participants"] = "";

    foreach($result as $item) {
        $results["description"] = "Wird nicht eingespeichert!";
        $results["place"] = $item->place;
        $results["type"] = $item->type;
        $results["participants"] = $item->participants;
        $results["start_date"] = $item->start_date;

    }
    return $results;
}



