<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.05.17
 * Time: 13:44
 */

/*
 * Diese Funktion erstellt ein Formular, in das der Nutzer einen neuen Termin eintragen kann.
 */
function task_assignment_appointment_form()
{
    $form['appointment'] = array(
        '#title' => "neue Aufgabe erstellen",
        '#type' => 'fieldset',
        '#description' => "In diesem Formular kann ein neuer Termin angelegt werden."
    );

    $form['appointment']['short_description'] = array(
        '#type' => 'textfield',
        '#require' => TRUE,
        '#title' => t('Kurzbezeichnung des Termins'),
    );

    $form['appointment']['task'] = array(
        '#title' => t('Terminbeschreibung'),
        '#resizable' => FALSE,
        '#type' => 'textarea',
    );

    //Das Format für die Eingabe eines Fälligkeitsdatums wird definiert
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

    $form['appointment']['deadline'] = array(
        '#type' => 'date_popup',
        '#date_timezone' => date_default_timezone(),
        '#date_format' => $dateformat,
        '#date_year_range' => '-3:+3',
        '#title' => $label,
        '#default_value' => $date_value,
    );

    $form['back'] = array(
        '#type' => 'submit',
        '#value' => 'Abbrechen',
        '#submit' => array('task_assignment_back_to_home')
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Speichern',
        '#submit' => array('save_new_appointment_assignment')
    );

    return $form;
}

/*
 * Diese Funktion lädt alle vom Benutzer eingegebenen Informationen und speichert sie in die Datenbank. Zeitgleich wird
 * ein neuer Node erstellt, sodass der Termin auch im Kalender angezeigt werden kann.
 */
function save_new_appointment_assignment($form, &$form_state)
{
    //Informationen werden aus den Feldern geladen
    $short_description = $form_state['values']['short_description'];
    $task = $form_state['values']['task'];
    $deadline = $form_state['values']['deadline'];

    //Es wird überprüft, ob Informationen eingegeben wurden
    if (!strlen($short_description) < 1 && !strlen($task) < 1) {

        //Der Termin wird in die dafür vorgesehene Datenbank-Tabelle abgespeichert
        $sql = "INSERT INTO {appointment} (appointment, date, short_description, uid) VALUES('$task', '$deadline','$short_description', $user->uid)";
        db_query($sql);

        //Die ID des eben angelegten Termins wird ermittelt
        $sql2 = "SELECT appointment_id from {appointment} ORDER BY appointment_id DESC LIMIT 1";
        $last_appointment_result = db_query($sql2);

        $last_appointment_id = "";
        foreach ($last_appointment_result as $lar) {
            $last_appointment_id = $lar->appointment_id;
        }

        //Um den Termin im Kalender anzuzeigen, wird ein neuer Node erstellt
        task_assaignment_create_node_for_calendar_view($short_description, $task, $deadline, 1, $last_appointment_id, "TERMIN");

        drupal_set_message("Termin wurde erfolgreich angelegt");
        drupal_goto("gruppentermine");

        //Falls nicht alle Eintragungen gemacht wurden, wird eine Fehlermeldung ausgegeben
    } else {

        if (strlen($short_description) < 1) {
            drupal_set_message("Bitte trage eine Kurzbezeichnung ein!", "error");
        }

        if (strlen($task) < 1) {
            drupal_set_message("Bitte trage eine Terminbeschreibung ein!", "error");
        }
    }
}
