<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 05.11.16
 * Time: 10:53
 */

function test_module_erstellenForm($form, &$form_submit)
{

    $reminder = check_current_open_protocol();

    if ($reminder["exist"] == 0) {
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
        }

        $form['test2'] = array(
            '#title' => "allgemeine Informationen",
            '#type' => 'fieldset',
            '#description' => "Um ein neues Protokoll anlegen zu können, müssen zunächst folgende allgemeine
            Informationen zum Protokoll eingetragen werden."
        );

        $form['test2']['teilnehmer'] = array(
            '#title' => t('Teilnehmer des Treffens'),
            '#type' => 'select',
            '#multiple' => true,
            '#options' => $new_parts,
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
            '#default_value' => $value,
        );

        $form['test2']['place'] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => t('Ort des Treffens'),
            '#required' => TRUE,

        );

        $form['test2']["type"]["plugin_select"] = array(
            "#type" => "select",
            "#title" => t("Art des Treffens"),
            "#options" => $types,
            '#required' => TRUE

        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('sub')
        );
    } else {
        drupal_set_message('Es existiert noch ein offenes Protokoll.
<a href="protokoll_allgemein">Hier einsehen.</a>
', "error");
    }


    return $form;
}

function sub($form, &$form_state)
{
    $reminder = check_current_open_protocol();

    if ($reminder["exist"] == 0) {
        $users = $form_state['values']['teilnehmer'];
        $users = implode(',', $users);

        global $user;
        $user_id = $user->uid;

        $types = array(
            "1" => t("gruppenintern"),
            "2" => t("gesamter Kurs"),
            "3" => t("Präsentation"),
        );

        $description = $form_state['values']['description'];
        $place = $form_state['values']['place'];
        $type = $form_state['values']['plugin_select'];
        $date = date("Y-m-d H:i:s");
        $start = $form_state['values']['start'];

        $list = getParticipants2();

        $sql = "INSERT INTO {protocol} (place, type, uid, date, finished, preparation, problems, conclusion, participants, start_date, outlook)
VALUES ('$place','$type', $user_id, '$date', 0,'','','','$users','$start', '');";

        //$sql = "UPDATE {protocol} SET participants = '$users', place = '$place', type = '$type' WHERE uid = $user_id";
        $result = db_query($sql);

        drupal_goto('/protokoll_vorbereitung');
    } else {
        drupal_set_message("Es ist ein Fehler aufgetreten. Es existiert bereits ein offenes Protokoll. <a href=\"protokoll_allgemein\">Hier einsehen.</a>");
    }

}