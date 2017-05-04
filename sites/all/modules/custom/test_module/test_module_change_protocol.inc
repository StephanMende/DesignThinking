<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.12.16
 * Time: 14:55
 */

function test_module_change_protocol_general_information_form($form, &$form_submit) {

    $protocol_id = arg(1);

    if(test_module_check_protocol_change_permission($protocol_id)) {
        $protocol = get_protocol_by_id($protocol_id);

        $list = getParticipants2();
        $list = explode(',', $list);
        $new_parts = array();

        foreach ($list as $p) {
            $user=user_load($p);
            $username=$user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'];;
            $new_parts[$p] = $username;
        }

        $parts = explode(",",$protocol->participants);

        $types = array(
            "gruppenintern" => t("gruppenintern"),
            "gesamter Kurs" => t("gesamter Kurs"),
            "Präsentation" => t("Präsentation"),
        );

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
            '#default_value' => $parts,
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
            '#default_value' => $protocol->start_date,
        );

        $form['test2']['end'] = array(
            '#type' => 'date_popup',
            '#date_timezone' => date_default_timezone(),
            '#date_format' => $dateformat,
            '#date_year_range' => '-3:+3',
            '#title' => $label,
            '#default_value' => $protocol->end_date,
        );

        $form['test2']['place'] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => t('Ort des Treffens'),
            '#default_value' => $protocol->place,
        );

        $form['test2']["type"]["plugin_select"] = array(
            "#type" => "select",
            "#title" => t("Art des Treffens"),
            "#default_value" => $protocol->type,
            "#options" => $types,
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('back_to_protocol_overview')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Speichern',
            '#submit' => array('save_changed_general_protocol')
        );

        return $form;
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um das Protokoll ändern zu können! <a href='".$base_url."'>Zurück zum Hauptmenü</a>", "error");
    }


}

function test_module_check_protocol_change_permission($protocol_id) {
    $proof = false;
    $protocol = $protocol_id;

    $list = getParticipants2();
    $list = explode(',', $list);

    $sql = "SELECT * FROM {protocol} WHERE protocol_id = ".$protocol;
    $result = db_query($sql);

    foreach ($result as $item) {
        $proof = in_array($item->uid, $list);
    }

    global $user;
    if (in_array('administrator', $user->roles)) {
        $proof = true;
    }

    return $proof;
}

function save_changed_general_protocol($form, &$form_state) {
    $protocol_id = arg(1);
    $place = $form_state['values']['place'];
    $type = $form_state['values']['plugin_select'];
    $users = $form_state['values']['teilnehmer'];
    $start = $form_state['values']['start'];
    $end = $form_state['values']['end'];
    $users = implode(',', $users);

    if(!strlen($place) <1 && !strlen($type) <1 && !strlen($users) <1 && !strlen($start) <1) {
        $sql = "UPDATE {protocol} SET end_date = '".$end."', start_date = '".$start."', participants = '$users', place = '$place', type = '$type' WHERE protocol_id = " . $protocol_id;
        db_query($sql);
        drupal_goto('/show_protocol/'.$protocol_id);

    } else {
        if(strlen($place) <1) {
            drupal_set_message ("Bitte trage einen Ort ein!", "error");
        }
        if(strlen($type) <1) {
            drupal_set_message ("Bitte trage eine Art ein!", "error");
        }
        if(strlen($users) <1) {
            drupal_set_message ("Bitte wähle die Teilnehmer aus!", "error");
        }
        if(strlen($start) <1) {
            drupal_set_message ("Bitte wähle die Startzeit aus!", "error");
        }
    }
}

function test_module_change_protocol_content_information_form($form, &$form_submit) {

    $protocol_id = arg(1);

    if(test_module_check_protocol_change_permission($protocol_id)) {
        $protocol = get_protocol_by_id($protocol_id);

        $form['test2'] = array(
            '#title' => "allgemeine Informationen",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt wird zunächst nochmals auf das letzte Treffen geblickt, um sich im Anschluss optimal auf das aktuelle Treffen vorbereiten zu können."
        );


        $form['test2']['preparation'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Inhalt: Was ist in diesem Treffen geplant?'),
            '#default_value' => $protocol->preparation,
        );

        $form['test2']['problems'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Probleme, die während des Treffens auftraten'),
            '#default_value' => $protocol->problems,
        );

        $form['test2']['conclusion'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Fazit des Treffens'),
            '#default_value' => $protocol->conclusion,
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Abbrechen',
            '#submit' => array('back_to_protocol_overview')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Speichern',
            '#submit' => array('save_changed_content_protocol')
        );

        return $form;
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um das Protokoll ändern zu können! <a href='".$base_url."'>Zurück zum Hauptmenü</a>", "error");
    }

}

function save_changed_content_protocol($form, &$form_state) {
    $protocol_id = arg(1);
    $preparation = $form_state['values']['preparation'];
    $problems = $form_state['values']['problems'];
    $conclusion = $form_state['values']['conclusion'];

        $sql = "UPDATE {protocol} SET preparation = '$preparation', problems = '$problems', conclusion = '$conclusion' WHERE protocol_id = " . $protocol_id;
        $result = db_query($sql);
        drupal_goto('/show_protocol/'.$protocol_id);
}

function test_module_change_protocol_method_information_form($form, &$form_submi) {

    $method_id = arg(1);

    if(test_module_check_method_permission($method_id)) {
        $current_method = get_current_method($method_id);

        print_r($current_method);


        $methods = get_available_methods();

        $form["method"]["plugin_select1"] = array(
            "#type" => "select",
            "#title" => t("Bitte passende Methode aussuchen"),
            "#options" => $methods,
            "#default_value" => $current_method->name,
            "#description" => t("Wähle Methode."),
        );

        $form["phase"]["plugin_select2"] = array(
            "#type" => "select",
            "#title" => t("Bitte passende Phase aussuchen"),
            "#default_value" => $current_method->phase,
            "#options" => array(
                "konvergent" => t("konvergent"),
                "divergent" => t("divergent"),
            ),
            "#description" => t("Wähle Phase."),
        );

        $form["raum"]["plugin_select3"] = array(
            "#type" => "select",
            "#title" => t("Bitte passenden Raum aussuchen"),
            "#default_value" => $current_method->room,
            "#options" => array(
                "Problemraum" => t("Problemraum"),
                "Lösungsraum" => t("Lösungsraum"),
                "Implementierungsraum" => t("Implementierungsraum"),
            ),
            "#description" => t("Wähle Raum."),
        );

        $form['grund'] = array(
            '#title' => t('Grund der Methodenwahl'),
            '#resizable' => false,
            "#default_value" => $current_method->reason,
            '#type' => 'textarea'
        );

        $form['ziel'] = array(
            '#title' => t('Ziel der Methode'),
            '#resizable' => FALSE,
            "#default_value" => $current_method->goal,
            '#type' => 'textarea'
        );

        $form['ausfuehrung'] = array(
            '#title' => t('Ausführung der Methode'),
            '#resizable' => FALSE,
            "#default_value" => $current_method->execution,
            '#type' => 'textarea'
        );

        $form['rating'] = array(
            '#type' => 'fivestar',
            '#stars' => 5,
            '#default_value' => $current_method->rating,
            '#title' => t('Bewertung der Methode'),
        );

        $form['bewertung'] = array(
            '#title' => t('Bewertung der Ergebnisse'),
            '#resizable' => FALSE,
            "#default_value" => $current_method->evaluation,
            '#type' => 'textarea'
        );

        $form['file'] = array(
            '#type' => 'file',
            '#name' => 'files[]',
            '#title' => t('Upload some photos'),
            '#attributes' => array('multiple' => 'multiple'),
        );

        $test = test_module_get_all_images($current_method->method_execution_id);


        $form['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $test,
        );


        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('back_to_protocol_overview')
        );

        $form['delete'] = array(
            '#type' => 'submit',
            '#value' => 'Löschen',
            '#submit' => array('delete_method_execution')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Speichern',
            '#submit' => array('save_protocol_method_information')
        );

        return $form;
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Methode ändern zu können! <a href='".$base_url."'>Zurück zum Hauptmenü</a>", "error");
    }


}

function save_protocol_method_information($form, &$form_state) {
    $method_id = arg(1);


    $ausfuehrung = "";
    $fazit = "";
    $ziel = "";
    $grund = "";
    $rating = "";

    $method = $form_state['values']['plugin_select1'];
    $phase = $form_state['values']['plugin_select2'];
    $raum = $form_state['values']['plugin_select3'];
    $ziel .= $form_state['values']['ziel'];
    $ausfuehrung .= $form_state['values']['ausfuehrung'];
    $fazit .= $form_state['values']['bewertung'];
    $grund .=  $form_state['values']['grund'];
    $rating .= $form_state['values']['rating'];



    $sql = "UPDATE {method_execution} SET rating = $rating, name='$method',room='$raum', phase='$phase',evaluation='$fazit',goal='$ziel', execution='$ausfuehrung', reason='$grund' WHERE method_execution_id = ".$method_id;


    $result = db_query($sql);

    $num_files = count($_FILES['files']['name']);
    $main_path = "";

    for ($i = 0; $i < $num_files; $i++) {
        $file = file_save_upload($i, array(
                'file_validate_is_audio' => array(),
                'file_validate_extensions' => array('png pdf jpg jpeg mp3 mp4'),
                'file_validate_size' => array(5 * 1024 * 1024),
            ));
            if ($file) {
                $file->status = FILE_STATUS_PERMANENT;
                if ($file = file_move($file, 'public://methods')) {
                    $file->status = FILE_STATUS_PERMANENT;
                    $form_state['values']['file'][$i] = $file;
                    $path = substr($file->uri, 9);
                    $sql = "INSERT INTO {method_execution_data} (method_execution_id, path) VALUES($method_id, '$path')";
                    db_query($sql);
                }
            } else {
                drupal_set_message("NOO");

            }
        }
        drupal_goto('protokoll_uebersicht');
}

function get_protocol_by_id($protocol_id) {
    $temp = "";
    $sql = "SELECT * FROM {protocol} WHERE protocol_id = ".$protocol_id;
    $result = db_query($sql);
    foreach ($result as $r) {
        $temp = $r;
    }
    return $temp;
}

function back_to_protocol_overview() {

    $protocol_id = arg(1);
    drupal_goto('/show_protocol/'.$protocol_id);
}

function get_current_method($id) {
    $method = "";
    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = $id";
    $result = db_query($sql);
    foreach($result as $r) {
        $method = $r;
    }
    return $method;
}

function get_available_methods() {
    $methods = array();
    $sql = "SELECT title, nid FROM {node} WHERE type = 'methode' ORDER BY title";
    $result = db_query($sql);
    foreach ($result as $r) {
        $methods[$r->title] = $r->title;
    }
    return $methods;
}

function delete_method_execution() {
    $method_id = arg(1);
    $sql = "DELETE FROM {method_execution} WHERE method_execution_id = $method_id";
    $result = db_query($sql);
    $sql2 = "DELETE FROM {method_execution_data} WHERE method_execution_id = $method_id";
    db_query($sql2);
    drupal_set_message("Objekt wurde gelöscht");
    drupal_goto('/protokoll_methoden_uebersicht');

}

function test_module_get_all_images($id) {
    $sql = "SELECT * FROM {method_execution_data} WHERE method_execution_id = ".$id;

    $result = db_query($sql);

    $temp = "";

    foreach($result as $r) {
        $allowed =  array('gif','png' ,'jpg');
        $filename = $r->path;
        $basename = preg_replace('/^.+[\\\\\\/]/', '', $filename);
        $ext = pathinfo($filename, PATHINFO_EXTENSION);
        if(in_array($ext,$allowed) ) {
            $path = "/design_thinking_tool/sites/default/files/".$r->path;

            $temp .= "<img style=\"max-width:540px;\" src='".$path."'alt=Selfhtml><br><b>$basename</b><a href=/design_thinking_tool/delete_file/$id/$basename/pXs8hYVUqe>&nbsp;&nbsp;Löschen</a><br><br>";
        } else {
            $temp .= "<a href='/design_thinking_tool/sites/default/files/$r->path' target='blank'>$basename</a><a href='/design_thinking_tool/delete_file/$id/$basename/pXs8hYVUqe'>&nbsp;&nbsp;Löschen</a><br><br>";
        }
    }

    return $temp;

}