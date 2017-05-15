<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 30.10.16
 * Time: 11:30
 */

require_once ("protocol_template_new_tasks.inc");


/*
 * Diese Funktion erstellt ein Formular, in das eine neue Aufgabe eingetragen werden kann
 */
function protocol_template_new_task_execution_form($form, &$form_submit)
{

    //Die Gruppenteilnehmer des aktuell angemeldeten Nutzers werden aus der Datenbank geladen, die ID ermittelt und
    //im Anschluss der Vor- und Nachname geladen, welche für die Anzeige im Protokoll benötigt werden
    $users = protocol_template_get_participants_from_logged_in_user_group();
    $user_array = explode(',', $users);
    $user_name_array = array();

    foreach ($user_array as $u) {
        $user = user_load($u);
        $username = $user->name;
        $user_name_array[$u] = $username;
    }

    //Das Format für die Eingabe eines Fälligkeitsdatums wird definiert
    date_default_timezone_set('Europe/Berlin');
    $dateformat = 'Y-m-d H:i';
    $label = t('Fälligkeitsdatum der Aufgabe');
    $timestamp = date("Y-m-d H:i");

    if (empty($timestamp)) {
        $d = new DateTime('@' . $timestamp);
        $date_value = $d->format($dateformat);
    } else {
        $d = new DateTime();
        $date_value = $d->format($dateformat);
    }

    //In diesem Array werden die einzelnen Auswahlmöglichkeiten bei der Priorisierung angegeben
    $priority_types = array("niedrig" => "niedrig", "mittel" => "mittel", "hoch" => "hoch");

    $form['new_task'] = array(
        '#title' => "neue Aufgabe erstellen",
        '#type' => 'fieldset',
        '#description' => "In diesem Formular kann eine neue Aufgabe angelegt werden. Dazu muss eine Aufgabe einem
        Gruppenmitglied zugeordnet werden, ein Fälligkeitsdatum besitzen und über Kurzbeschreibung als auch ausführliche
        Beschreibung verfügen."
    );

    $form['new_task']['short_description'] = array(
        '#type' => 'textfield',
        '#require' => TRUE,
        '#title' => t('Kurzbezeichnung der Aufgabe'),
    );

    $form['new_task']["raum"]["responsible"] = array(
        "#type" => "select",
        "#title" => t("Verantwortliche Person"),
        "#options" => $user_name_array,
    );

    $form['new_task']['task'] = array(
        '#title' => t('Aufgabenbeschreibung'),
        '#resizable' => FALSE,
        '#type' => 'textarea',
    );

    $form['new_task']['deadline'] = array(
        '#type' => 'date_popup',
        '#date_timezone' => date_default_timezone(),
        '#date_format' => $dateformat,
        '#date_year_range' => '-3:+3',
        '#title' => $label,
        '#default_value' => $date_value,
    );

    $form['new_task']["raum2"]["prio"] = array(
        "#type" => "select",
        "#title" => t("Priorität der Aufgabe"),
        "#options" => $priority_types,
    );

    $form['back'] = array(
        '#type' => 'submit',
        '#value' => 'Zurück',
        '#submit' => array('protocol_template_task_execution_back_to_overview')
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Speichern',
        '#submit' => array('protocol_template_save_new_task')
    );

    return $form;
}

/*
 * Diese Funktion liest alle nötigen Informationen zu einer Aufgabe aus den Feldern und speichert diese in die
 * Datenbank ab
 */
function protocol_template_save_new_task($form, &$form_state)
{

    $responsible_user_id = $form_state['values']['responsible'];

    //Um das aktuell offene Protokoll zu ermitteln, müssen zunächst die Gruppenteilnehmer erneut geladen werden
    $parts = protocol_template_get_participants_from_logged_in_user_group();

    //Im Anschluss wird in der Datenbank geschaut, welches Protokoll von der Gruppe noch nicht geschlossen wurde. Sollte
    //keines vorhanden sein, wird die ID 0 übergeben, was bedeutet, dass die Aufgabe keinem Protokoll zugeordnet wird
    $p_id = protocol_template_get_current_open_group_protocol($parts);

    //Die nötigen Informationen werden aus den Feldern geladen
    $task = $form_state['values']['task'];
    $deadline = $form_state['values']['deadline'];
    $short_description = $form_state['values']['short_description'];
    $priority = $form_state['values']['prio'];

    //Sofern alle Eintragungen gemacht wurden, wird die Aufgabe eingespeichert
    if (!strlen($short_description) < 1 && !strlen($task) < 1 && !strlen($responsible_user_id) < 1) {

        //Die Aufgabe wird in die passende Datenbanktabelle eingespeichert
        $sql = "INSERT INTO {task} (uid, task, deadline, protocol_id, done, short_description, priority) VALUES($responsible_user_id, '$task','$deadline', $p_id, 0, '$short_description', '$priority')";
        db_query($sql);

        //Für die Aufgaben-ID wird die zuvor gespeicherte Aufgabe direkt wieder aus der Datenbank gelesen
        $sql2 = "SELECT task_id from {task} ORDER BY task_id DESC LIMIT 1";
        $task_result = db_query($sql2);

        $task_id = "";

        foreach ($task_result as $t) {
            $task_id = $t->task_id;
        }

        //Um die Aufgabe im Kalender anzeigen zu können, wird sie zusätzlich als Node eingespeichert
        $node = new stdClass();
        $node->type = 'event';
        node_object_prepare($node);
        $node->language = LANGUAGE_NONE;

        //Titel und Kurzbeschreibung werden dem Node hinzugefügt
        $node->title = $short_description;
        $node->body[$node->language][0]['value'] = $task;

        //Die Deadline der Aufgabe wird für den Node konvertiert
        $deadline_date = new DateTime($deadline);
        $deadline_date->sub(new DateInterval('PT1H'));
        $converted_deadline = $deadline_date->format('Y-m-d H:i:s');
        $converted_deadline = str_replace(" ", "T", $converted_deadline);

        //Das Nutzer-Objekt des zugewiesenen Nutzers wird geladen
        $username = user_load($responsible_user_id);

        //Weitere Attribute werden dem Node hinzugefügt
        $node->field_event_date[$node->language][0]['value'] = $converted_deadline;
        $node->field_event_date[$node->language][0]['timezone'] = "Europe/Berlin";
        $node->field_event_date[$node->language][0]['timezone_db'] = "Europe/Berlin";
        $node->field_art[$node->language][0]['tid'] = 2;
        $node->field_referenznummer[$node->language][0]['value'] = $task_id;
        $node->field_referenzmitglied[$node->language][0]['value'] = $username->name;

        //Der Node wird abgespeichert
        node_save($node);

        drupal_set_message("Die Aufgabe wurde angelegt!");
        drupal_goto("/protokoll_neue_aufgaben");

        //Sollten nicht alle Eintragungen gemacht worden sein, wird eine Fehlermeldung ausgegeben
    } else {
        if (strlen($short_description) < 1) {
            drupal_set_message("Bitte trage eine Kurzbezeichnung ein!", "error");
            $form_state['redirect'] = FALSE;
        }

        if (strlen($task) < 1) {
            drupal_set_message("Bitte trage eine Aufgabenbeschreibung ein!", "error");
            $form_state['redirect'] = FALSE;
        }
    }
}

/*
 * Diese Funktion leitet den Benutzer zurück zur Aufgabenübersicht des Protokolls
 */
function protocol_template_task_execution_back_to_overview($form, &$form_state)
{
    drupal_goto("/protokoll_neue_aufgaben");

}

