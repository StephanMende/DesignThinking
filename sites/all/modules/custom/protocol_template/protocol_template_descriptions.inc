<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 30.10.16
 * Time: 09:38
 */

/*
 * Diese Funktion erstellt ein Formular, in das zusätzliche Beschreibungen für das Protokoll eingetragen werden können
 */
function protocol_template_set_protocol_descriptions_form($form, &$form_submit)
{

    //Es wird geschaut, ob bereits ein offenes Protokoll existiert
    $reminder = protocol_template_check_current_open_protocol();

    //Sollte bereits ein offenes Protokoll existieren, können die Tabellen erstellt werden
    if ($reminder["exist"] == 1) {

        //Falls bereits Eintragungen von der Gruppe gemacht wurden, werden sie in die Felder geladen
        $content = protocol_template_description_get_content();

        $form['description'] = array(
            '#title' => "Zusammenfassung des Treffens",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt werden abschließende Punkte zum Treffen behandelt."
        );

        $form['description']['problems'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Probleme, die während des Treffens auftraten'),
            '#default_value' => $content["problems"],
        );

        $form['description']['conclusion'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Fazit des Treffens'),
            '#default_value' => $content["conclusion"],
        );

        $form['description']['outlook'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Ausblick auf das nächste Treffen'),
            '#default_value' => $content["outlook"],
        );

        //Das Format für die Eingabe eines Enddatums wird definiert
        $dateformat = 'Y-m-d H:i';
        $label = t('Aktuelles Datum');
        $timestamp = time();

        if (empty($timestamp)) {
            $d = new DateTime('@' . $timestamp);
            $date_value = $d->format($dateformat);
        } else {
            $d = new DateTime();
            $date_value = $d->format($dateformat);
        }

        date_default_timezone_set('Europe/Berlin');
        drupal_set_message("heutiges Datum: " . date("d.m.Y H:i"));

        drupal_set_message($date_value);

        $form['description']['deadline'] = array(
            '#type' => 'date_popup',
            '#date_timezone' => 'Europe/Berlin',
            '#date_format' => $dateformat,
            '#date_year_range' => '-3:+3',
            '#title' => $label,
            '#require' => TRUE,
            '#default_value' => $date_value,
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('protocol_template_description_back_to_new_task')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Protokoll abspeichern',
            '#submit' => array('protocol_template_save_protocol')
        );

    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
        <a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
        ', "error");
    }

    return $form;
}

/*
 * Diese Funktion leitet die Fertigstellung und Speicherung des Protokolls in die Wege
 */
function protocol_template_save_protocol($form, &$form_state)
{
    //die eingegebenen Daten werden gespeichert
    protocol_template_descriptions_save_data($form, $form_state);

    //Das Protokoll wird fertiggestellt
    protocol_template_finish_protocol();
    drupal_goto("/protokoll_uebersicht");
}

/*
 * Diese Funktion liest alle Informationen aus den Formular-Feldern aus und speichert diese in die Datenbank
 */
function protocol_template_descriptions_save_data($form, &$form_state)
{
    //Die entsprechenden Felder werden ausgelesen
    $problems = $form_state['values']['problems'];
    $conclusion = $form_state['values']['conclusion'];
    $outlook = $form_state['values']['outlook'];

    //Die Gruppenmitglieder werden ermittelt
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //das aktuell offene Protokoll wird ermittelt.
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid  IN ($users) AND pro.finished = 0";
    $tempresult = db_query($sql);
    $found = 0;
    $protocol_id = 0;
    foreach ($tempresult as $temp) {
        $found = $found + 1;
        $protocol_id = $temp->protocol_id;
    }

    //Sofern ein offenes Protokoll vorliegt, werden die Daten in die Datenbank gespeichert
    if ($found > 0) {
        $raum = protocol_template_get_group_position_after_finish($protocol_id, "room");
        $phase = protocol_template_get_group_position_after_finish($protocol_id, "phase");
        $deadline = $form_state['values']['deadline'];
        $sql = "UPDATE {protocol} SET outlook = '$outlook', end_date = '$deadline', raum_end = '$raum', phase_end = '$phase', problems = '$problems', conclusion = '$conclusion' WHERE uid IN ($users) AND finished = 0";
        db_query($sql);
    }
}

/*
 * Diese Funktion ermittelt, in welchen Raum und welche Phase sich die Gruppe nach Fertigstellung des Protokolls befindet
 */
function protocol_template_get_group_position_after_finish($protocol_id, $object)
{
    //In diese Variablen werden später Raum- beziehungsweise Phase gespeichert
    $room = "";
    $phase = "";

    //Die letzte durchgeführte Methode wird ermittelt
    $sql = "SELECT * FROM {method_execution} WHERE protocol_id = " . $protocol_id . " ORDER BY method_execution_id DESC LIMIT 1";
    $method_result = db_query($sql);

    //Sofern eine Methodendurchführung gefunden wurde...
    if ($method_result->rowCount() > 0) {

        //Werden Raum und Phase, in denen sich die Gruppe nach der Durchführung befanden, abgespeichert
        foreach ($method_result as $item) {
            $room = $item->raum_end;
            $phase = $item->phase_end;
        }

        //Sollte keine Methode durchgeführt worden sein
    } else {

        //Die Gruppenmitglieder werden ermittelt
        $users = protocol_template_get_participants_from_logged_in_user_group();

        //das vorige Protokoll wird ermittelt
        $sql = "SELECT * FROM {protocol} WHERE uid in (" . $users . ") AND protocol_id != " . $protocol_id . " ORDER BY protocol_id DESC LIMIT 1";
        $protocol_result = db_query($sql);

        //Sollte ein Protokoll gefunden worden sein...
        if ($protocol_result->rowCount() > 0) {

            //Werden Raum und Phase davon übernommen
            foreach ($method_result as $item) {
                $room = $item->raum_end;
                $phase = $item->phase_end;
            }

            //Sollte noch kein Protokoll vorliegen, werden automatisch Raum und Phase festgelegt
        } else {
            $room = "Problemraum";
            $phase = "divergent";
        }
    }

    //Je nachdem, welcher Übergabeparameter übergeben wurde, werden entweder Werte für Raum oder Phase zurückgegeben
    if (!strcmp($object, "room")) {
        return $room;
    } else {
        return $phase;
    }
}

/*
 * Diese Methode stellt das aktuell offene Protokoll fertig und schließt es
 */
function protocol_template_finish_protocol()
{
    //Die Gruppenmitglieder werden ermittelt
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //Das offene Protokoll der Gruppe wird ermittelt
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($users) AND pro.finished = 0";
    $protocol_result = db_query($sql);

    foreach ($protocol_result as $protocol) {
        $protocol_id = $protocol->protocol_id;

        //das Protokoll wird geschlossen
        $sql = "UPDATE {protocol} as p SET finished = 1 WHERE p.protocol_id = $protocol_id AND p.finished = 0";
        db_query($sql);
    }
}

/*
 * Diese Funktion schaut in der Datenbank, ob bereits Eintragungen zu diesem Formular vorliegen und lädt diese
 * gegebenenfalls in die entsprechenden Felder
 */
function protocol_template_description_get_content()
{

    //diese Variable speichert die Eingaben des Nutzers ab.
    $user_input = array();

    //Die Gruppenmitglieder werden ermittelt
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //Das aktuelle Protokoll wird aus der Datenbank geladen
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($users) ORDER BY pro.protocol_id DESC LIMIT 1";
    $protocol_result = db_query($sql);

    //die eingebenen Daten aus der Datenbank werden in ein Array abgespeichert
    foreach ($protocol_result as $protocol) {
        $user_input["problems"] = $protocol->problems;
        $user_input["conclusion"] = $protocol->conclusion;
        $user_input["raum_end"] = $protocol->raum_end;
        $user_input["phase_end"] = $protocol->phase_end;
        $user_input["outlook"] = $protocol->outlook;
    }
    return $user_input;
}

/*
 * Diese Funktion leitet den Nutzer zurück zur Erstellung von neuen Aufgaben
 */
function protocol_template_description_back_to_new_task($form, &$form_state)
{
    protocol_template_descriptions_save_data($form, $form_state);
    drupal_goto('/protokoll_neue_aufgaben');
}



