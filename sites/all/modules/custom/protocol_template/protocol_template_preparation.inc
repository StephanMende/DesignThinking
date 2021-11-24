<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.11.16
 * Time: 07:48
 */

/*
 * Diese Funktion erstellt ein Formular, in das der Nutzer alle nötigen Informationen zur Vorbereitung für das
 * aktuell offene Protokoll einträgt
 */
function protocol_template_set_preparation_form($form, &$form_submit)
{
    //Es wird überprüft, ob schon ein offenes Protokoll existiert
    $reminder = protocol_template_check_current_open_protocol();

    //Sollte ein offenes Protokoll existieren...
    if ($reminder["exist"] == 1) {

        //Die bisherigen eingegebenen Daten der Gruppe werden geladen
        $temp = protocol_template_get_content_for_preparation();

        $form['preparation_set'] = array(
            '#title' => "Vorbereitung",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt wird zunächst nochmals auf das letzte Treffen geblickt, um sich im Anschluss optimal auf das aktuelle Treffen vorbereiten zu können."
        );

        $form['preparation_set']['review'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('kurzer Rückblick auf das letzte Treffen (Was war das Fazit?)'),
            '#default_value' => $temp["review"],
            '#resizable' => FALSE,

        );

        $form['preparation_set']['preparation'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Was ist in diesem Treffen geplant?'),
            '#default_value' => $temp["preparation"],
            '#resizable' => FALSE,
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('protocol_template_preparation_back_to_general')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('protocol_template_preparation_further_to_tasks')
        );
    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
            <a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
            ', "error");
    }

    return $form;
}

/*
 * Diese Funktion liest alle Informationen aus den Formular-Feldern aus und speichert diese in die Datenbank
 */
function protocol_template_preparation_save_data($form, &$form_state)
{
    //Die entsprechenden Felder werden ausgelesen
    $review = $form_state['values']['review'];
    $preparation = $form_state['values']['preparation'];

    //Die Gruppenmitglieder werden ermittelt
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //Das aktuell offene Protokoll der Gruppe wird geladen
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid  IN ($users) AND pro.finished = 0";
    $protocol_result = db_query($sql);

    //diese Variable überprüft, ob das offene Protokoll aus der Datenbank gelesen werden konnte
    $found = 0;

    foreach ($protocol_result as $temp) {
        $found = $found + 1;
    }

    //Sollte ein offenes Protokoll vorliegen...
    if ($found > 0) {

        //Die Informationen werden dem offenen Protokoll hinzugefügt, es wird aktualisiert
        $sql = "UPDATE {protocol} SET preparation = '" . $preparation . "', review = '" . $review . "' WHERE uid IN (" . $users . ") AND finished = 0";
        db_query($sql);
    }
}

/*
 * Diese Funktion schaut in der Datenbank, ob bereits Eintragungen zu diesem Formular vorliegen und lädt diese
 * gegebenenfalls in die entsprechenden Felder
 */
function protocol_template_get_content_for_preparation()
{

    //diese Variable speichert die Eingaben des Nutzers ab.
    $user_input = array();

    //Die Gruppenmitglieder werden ermittelt
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //Das aktuelle Protokoll wird aus der Datenbank geladen
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($users) AND finished = 0";
    $protocol_result = db_query($sql);

    //Das gefundene Protokoll wird ausgelesen
    foreach ($protocol_result as $protocol) {
        $user_input["preparation"] = $protocol->preparation;
        $user_input["review"] = $protocol->review;
    }

    //Die Nutzereingaben werden zurückgegeben
    return $user_input;
}

/*
 * Diese Funktion führt den Nutzer zurück zum vorigen Schritt des Protokoll-Templates
 */
function protocol_template_preparation_back_to_general($form, &$form_state)
{
    protocol_template_preparation_save_data($form, $form_state);
    drupal_goto('/protokoll_allgemein');
}


/*
 * Diese Funktion leitet den Nutzer weiter zur Aufgabenübersicht
 */
function protocol_template_preparation_further_to_tasks($form, &$form_state)
{
    protocol_template_preparation_save_data($form, $form_state);
    drupal_goto('/protokoll_offene_aufgaben');
}

