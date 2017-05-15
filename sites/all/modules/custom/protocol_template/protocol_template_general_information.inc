<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:40
 */

/*
 * Diese Funktion erstellt ein Formular, in dem allgemeine Informationen des Protokolls
 * verändert werden können
 */
function protocol_template_set_general_information_form($form, &$form_submit)
{

    //Es wird geschaut, ob bereits ein offenes Protokoll existiert
    $reminder = protocol_template_check_current_open_protocol();

    //Sollte bereits ein offenes Protokoll existieren, kann das Formular erstellt werden
    if ($reminder["exist"] == 1) {

        //Die Protokoll-Arten werden definiert
        $types = array(
            "gruppenintern" => t("gruppenintern"),
            "gesamter Kurs" => t("gesamter Kurs"),
            "Präsentation" => t("Präsentation"),
        );


        //Die Gruppenmitglieder werden ermittelt
        $users = protocol_template_get_participants_from_logged_in_user_group();
        $user_array = explode(',', $users);
        $user_name_array = array();

        //für jeden Benutzer werden Vor- und Nachname ermittelt und in ein Array gespeichert
        foreach ($user_array as $user) {
            $user_element = user_load($user);
            $username = $user_element->field_first_name['und'][0]['value'] . " " . $user_element->field_last_name['und'][0]['value'];
            $user_name_array[$user] = $username;
        }

        //Falls schon allgemeine Informationen für dieses Protokoll vorliegen, werden diese
        //aus der Datenbank geladen und in die Formular-Felder eingefügt
        $content = protocol_template_get_content_for_general_information();

        //Die teilnehmenden Gruppenmitglieder werden für die Anzeige ermittelt
        $joined_participants = explode(",", $content["participants"]);

        //Das Format für die Eingabe eines Startdatum wird definiert
        date_default_timezone_set('Europe/Berlin');
        $dateformat = 'Y-m-d H:i';
        $label = t('Startzeit des Treffens');

        $form['new_protocol'] = array(
            '#title' => "allgemeine Informationen",
            '#type' => 'fieldset',
            '#description' => "Um ein neues Protokoll anlegen zu können, müssen zunächst folgende allgemeine
            Informationen zum Protokoll eingetragen werden. Als Autor wird automatisch das Gruppenmitglied eingetragen,
            das das Protokoll angelegt hat."
        );

        $form['new_protocol']['teilnehmer'] = array(
            '#type' => 'checkboxes',
            '#options' => $user_name_array,
            '#required' => TRUE,
            '#default_value' => $joined_participants,
            '#title' => t('Teilnehmer des Treffens'),
        );

        $form['new_protocol']['start'] = array(
            '#type' => 'date_popup',
            '#date_timezone' => date_default_timezone(),
            '#date_format' => $dateformat,
            '#date_year_range' => '-3:+3',
            '#title' => $label,
            '#required' => TRUE,
            '#default_value' => $content["start_date"],
        );

        $form['new_protocol']['place'] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => t('Ort des Treffens'),
            '#default_value' => $content["place"],
            '#required' => TRUE,
        );

        $form['new_protocol']["type"]["plugin_select"] = array(
            "#type" => "select",
            "#title" => t("Art des Treffens"),
            "#default_value" => $content["type"],
            "#options" => $types,
            '#required' => TRUE,
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('protocol_template_update_general_information')
        );

    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
        <a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
        ', "error");
    }

    return $form;
}

/*
 * Diese Funktion lädt die Eingaben des Nutzers aus den Formular-Feldern und speichert sie
 * in die Datenbank ab
 */
function protocol_template_update_general_information($form, &$form_state)
{

    //Die ID des aktuellen Nutzers wird geladen
    global $user;
    $user_id = $user->uid;

    //Die Teilnehmer am Meeting werden ermittelt
    $new_array = array();

    $users = $form_state['values']['teilnehmer'];

    foreach ($users as $item) {
        if ($item != 0) {
            array_push($new_array, $item);
        }
    }

    $users = implode(',', $new_array);

    //Die restlichen Informationen werden aus den Formular-Feldern geladen
    $place = $form_state['values']['place'];
    $type = $form_state['values']['plugin_select'];
    $date = date("Y-m-d H:i:s");
    $start = $form_state['values']['start'];

    //Die Gruppenmitglieder werden ermittelt
    $user_list = protocol_template_get_participants_from_logged_in_user_group();

    //Das offene Protokoll der Gruppe wird geladen
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid  IN ($user_list) AND pro.finished = 0";
    $protocol_result = db_query($sql);
    $found = 0;
    foreach ($protocol_result as $temp) {
        $found = $found + 1;
    }

    //Sollte das offene Protokoll gefunden worden sein, werden dessen Attribute aktualisiert
    if ($found > 0) {
        $sql = "UPDATE {protocol} SET start_date = '" . $start . "', participants = '$users', place = '$place', type = '$type' WHERE uid = $user_id";
        db_query($sql);

        //Andernfalls wird ein neues Protokoll erstellt
    } else {
        $sql = "INSERT INTO {protocol} (place,type,uid,date, participants, start_date) VALUES('$place', '$type', $user_id, '$date', '$users', '$start')";
        db_query($sql);
    }
    drupal_goto('/protokoll_vorbereitung');
}

/*
 * Diese Funktion überprüft, ob für dieses Protokoll bereits allgemeine Informationen eingetragen wurden und lädt
 * diese gegebenenfalls aus der Datenbank
 */
function protocol_template_get_content_for_general_information()
{
    //In diese Variable werden die bereits existierenden allgemeinen Informationen gespeichert
    $results = array();

    //Die Gruppenmitglieder werden ermittelt
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //Das Protokoll wird aus der Datenbank geladen
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($users) AND pro.finished = 0";
    $protocol_result = db_query($sql);

    //Die Variablen für die allgemeinen Informationen werden angelegt
    $results["place"] = "";
    $results["type"] = "";
    $results["participants"] = "";

    //Das gefundene Protokoll-Element wird ausgelesen und die Variable gefüllt
    foreach ($protocol_result as $protocol) {
        $results["place"] = $protocol->place;
        $results["type"] = $protocol->type;
        $results["participants"] = $protocol->participants;
        $results["start_date"] = $protocol->start_date;

    }
    return $results;
}



