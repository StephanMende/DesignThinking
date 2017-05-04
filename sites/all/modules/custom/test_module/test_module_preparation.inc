<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.11.16
 * Time: 07:48
 */

function test_module_vorbereitungForm($form, &$form_submit)
{

    $reminder = check_current_open_protocol();

    if ($reminder["exist"] == 1) {
        $temp = get_content_for_preparation();

        $form['test'] = array(
            '#title' => "Vorbereitung",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt wird zunächst nochmals auf das letzte Treffen geblickt, um sich im Anschluss optimal auf das aktuelle Treffen vorbereiten zu können."
        );

        $form['test']['review'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('kurzer Rückblick auf das letzte Treffen (Was war das Fazit?)'),
            '#default_value' => $temp["review"],
            '#resizable' => FALSE,

        );

        $form['test']['preparation'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Was ist in diesem Treffen geplant?'),
            '#default_value' => $temp["preparation"],
            '#resizable' => FALSE,
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('back_to_general')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('further_to_tasks')
        );
    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
<a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
', "error");
    }

    return $form;
}

function back_to_general($form, &$form_state)
{
    saveData($form, $form_state);
    drupal_goto('/protokoll_allgemein');
}

function further_to_tasks($form, &$form_state)
{
    saveData($form, $form_state);
    drupal_goto('/protokoll_offene_aufgaben');
}

function saveData($form, &$form_state)
{
    $review = $form_state['values']['review'];
    $preparation = $form_state['values']['preparation'];
    $list = getParticipants2();
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid  IN ($list) AND pro.finished = 0";
    $tempresult = db_query($sql);
    $found = 0;
    foreach ($tempresult as $temp) {
        $found = $found + 1;
    }

    if ($found > 0) {
        $sql = "UPDATE {protocol} SET preparation = '".$preparation."', review = '".$review."' WHERE uid IN (".$list.") AND finished = 0";
        $result = db_query($sql);
    }


}

function get_content_for_preparation()
{

    $results = array();
    global $user;
    $user_id = $user->uid;

    $list = getParticipants2();

    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($list) AND finished = 0";

    $result = db_query($sql);
    foreach ($result as $item) {
        $results["preparation"] = $item->preparation;
        $results["review"] = $item->review;
    }
    return $results;
}

