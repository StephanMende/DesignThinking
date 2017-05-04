<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 30.10.16
 * Time: 11:30
 */

function test_module_new_task_execution_form($form, &$form_submit)
{

    $reminder = check_current_open_protocol();

    $dateformat = 'Y-m-d H:i';
    $label = t('Deadline');
    $timestamp = time();
    $dbv = $timestamp;

    $parts = getParticipants2();
    $list = explode(',', $parts);
    $new_parts = array();

    foreach ($list as $p) {
        $user = user_load($p);
        $username = $user->name;
        $new_parts[$p] = $username;
    }

    $value = NULL;
    if (empty($dbv)) {
        $d = new DateTime('@' . $dbv);
        $value = $d->format($dateformat);
    } else {
        $d = new DateTime();
        $value = $d->format($dateformat);
    }

    $form['test'] = array(
        '#title' => "neue Aufgabe erstellen",
        '#type' => 'fieldset',
        '#description' => "Es kann eine Mail an einen beliebigen Empfänger versendet werden. Die Adresse des Absenders ist standardmäßig die registrierte Mail-Adresse des Benutzers."
    );

    $form['test']['short_description'] = array(
        '#type' => 'textfield',
        '#require' => TRUE,
        '#title' => t('Kurzbezeichnung der Aufgabe'),
    );

    $form['test']["raum"]["responsible"] = array(
        "#type" => "select",
        "#title" => t("Verantwortliche Person"),
        "#options" => $new_parts,
    );

    $form['test']['task'] = array(
        '#title' => t('Aufgabenbeschreibung'),
        '#resizable' => FALSE,
        '#type' => 'textarea',
    );

    $form['test']['deadline'] = array(
        '#type' => 'date_popup',
        '#date_timezone' => date_default_timezone(),
        '#date_format' => $dateformat,
        '#date_year_range' => '-3:+3',
        '#title' => $label,
        '#default_value' => $value,
    );

    $types = array("niedrig" => "niedrig", "mittel" => "mittel", "hoch" => "hoch");


    $form['test']["raum2"]["prio"] = array(
        "#type" => "select",
        "#title" => t("Priorität der Aufgabe"),
        "#options" => $types,
    );

    $form['back'] = array(
        '#type' => 'submit',
        '#value' => 'Zurück',
        '#submit' => array('back_to_overview')
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Speichern',
        '#submit' => array('save_new_task')
    );

    return $form;
}

function back_to_overview($form, &$form_state)
{
    drupal_goto("/protokoll_neue_aufgaben");

}

function save_new_task($form, &$form_state)
{
    $user_id = $form_state['values']['responsible'];
    $user_id2 = $form_state['values']['teilnehmer'];

    $parts = getParticipants2();
    $p_id = get_open_protocol2($parts);

    $protocol_id = $p_id;
    $task = $form_state['values']['task'];
    $deadline = $form_state['values']['deadline'];
    $short_description = $form_state['values']['short_description'];
    $priority = $form_state['values']['prio'];
    $priority = "niedrig";

    if (!strlen($short_description) < 1 && !strlen($task) < 1 && !strlen($user_id) < 1) {
        $node = new stdClass();

        // Set content type
        $node->type = 'event';

        // Prepare defaults
        node_object_prepare($node);

        // Define language (currently language neutral)
        $node->language = LANGUAGE_NONE;

        // Basic content
        $node->title = $short_description;
        $node->body[$node->language][0]['value'] = $task;

        $date = new DateTime($deadline);

        $date->sub(new DateInterval('PT1H'));
        $deadline2 = $date->format('Y-m-d H:i:s');

        $deadline2 = str_replace(" ", "T", $deadline2);
        //$deadline2 = $deadline2 . ":00";
        //drupal_set_message($deadline2);

        $test = '2016-12-20T22:30:00';

        $username = user_load($user_id);

        $referenz = "";


        $sql = "INSERT INTO {task} (uid, task, deadline, protocol_id, done, short_description, priority) VALUES($user_id, '$task','$deadline', $protocol_id, 0, '$short_description', '$priority')";
        $result = db_query($sql);


        $sql2 = "SELECT task_id from {task} ORDER BY task_id DESC LIMIT 1";
        $result2 = db_query($sql2);

        foreach ($result2 as $ko) {
            $referenz = $ko->task_id;
        }

        $node->field_event_date[$node->language][0]['value'] = $deadline2;
        $node->field_event_date[$node->language][0]['timezone'] = "Europe/Berlin";
        $node->field_event_date[$node->language][0]['timezone_db'] = "Europe/Berlin";
        $node->field_art[$node->language][0]['tid'] = 2;
        $node->field_referenznummer[$node->language][0]['value'] = $referenz;
        $node->field_referenzmitglied[$node->language][0]['value'] = $username->name;
        // Save node
        node_save($node);


        drupal_set_message("Die Aufgabe wurde angelegt!");
        drupal_goto("/protokoll_neue_aufgaben");

    } else {
        if (strlen($short_description) < 1) {
            drupal_set_message("Bitte trage eine Kurzbezeichnung ein!", "error");
        }

        if (strlen($task) < 1) {
            drupal_set_message("Bitte trage eine Aufgabenbeschreibung ein!", "error");
        }
    }
}

