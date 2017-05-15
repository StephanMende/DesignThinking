<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.12.16
 * Time: 14:55
 */

/*
 * Diese Funktion erstellt ein Formular, in das allgemeine Informationen eines Protokolls eingetragen werden. Anschließend
 * können diese Informationen verändert werden.
 */
function protocol_template_change_protocol_general_information_form($form, &$form_submit)
{

    //Das entsprechende Protokoll wird au der URL ausgelesen
    $protocol_id = arg(1);

    //es wird überprüft, ob der aktuell angemeldete Benutzer das Recht zur Veränderung des Protokolls besitzt
    if (protocol_template_check_protocol_change_permission($protocol_id)) {

        //Die Daten des Protokolls werden geladen
        $protocol = protocol_template_get_protocol_element_by_id($protocol_id);

        //Die Gruppenmitglieder werden ermittelt, Vor- und Nachname geladen und in ein Array gespeichert
        $users = protocol_template_get_participants_from_logged_in_user_group();
        $users = explode(',', $users);
        $user_names = array();

        foreach ($users as $p) {
            $user = user_load($p);
            $username = $user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'];;
            $user_names[$p] = $username;
        }

        //Es wird ermittelt, welche Nutzer beim Meeting anwesend waren
        $default_participants = explode(",", $protocol->participants);

        //Die Protokoll-Arten werden festgelegt
        $types = array(
            "gruppenintern" => t("gruppenintern"),
            "gesamter Kurs" => t("gesamter Kurs"),
            "Präsentation" => t("Präsentation"),
        );

        //Das Format für die Eingabe eines Enddatums wird definiert
        date_default_timezone_set('Europe/Berlin');
        $dateformat = 'Y-m-d H:i';
        $label = t('Endzeit des Treffens');

        $form['change_protocol'] = array(
            '#title' => "allgemeine Informationen",
            '#type' => 'fieldset',
            '#description' => "Um ein neues Protokoll anlegen zu können, müssen zunächst folgende allgemeine
            Informationen zum Protokoll eingetragen werden."
        );

        $form['change_protocol']['teilnehmer'] = array(
            '#title' => t('Teilnehmer des Treffens'),
            '#type' => 'select',
            '#multiple' => true,
            '#options' => $user_names,
            '#default_value' => $default_participants,
        );

        $form['change_protocol']['start'] = array(
            '#type' => 'date_popup',
            '#date_timezone' => date_default_timezone(),
            '#date_format' => $dateformat,
            '#date_year_range' => '-3:+3',
            '#title' => $label,
            '#default_value' => $protocol->start_date,
        );

        $form['change_protocol']['end'] = array(
            '#type' => 'date_popup',
            '#date_timezone' => date_default_timezone(),
            '#date_format' => $dateformat,
            '#date_year_range' => '-3:+3',
            '#title' => $label,
            '#default_value' => $protocol->end_date,
        );

        $form['change_protocol']['place'] = array(
            '#type' => 'textfield',
            '#require' => TRUE,
            '#title' => t('Ort des Treffens'),
            '#default_value' => $protocol->place,
        );

        $form['change_protocol']["type"]["plugin_select"] = array(
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
            '#submit' => array('protocol_template_save_changed_general_protocol')
        );

        return $form;
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um das Protokoll ändern zu können! <a href='" . $base_url . "'>Zurück zum Hauptmenü</a>", "error");
    }
}

/*
 * Diese Funktion überprüft, ob der aktuell angemeldete Benutzer das Recht besitzt, um das Protokoll zu
 * verändern
 */
function protocol_template_check_protocol_change_permission($protocol_id)
{

    //Die Boolean-Variable zeigt, ob der Nutzer das Recht besitzt oder nicht
    $proof = false;

    //Die ID des Protokolls wird übergeben
    $protocol = $protocol_id;

    //Die Gruppenmitglieder werden ermittelt, Vor- und Nachname geladen und in ein Array gespeichert
    $users = protocol_template_get_participants_from_logged_in_user_group();
    $user_list = explode(',', $users);

    //Das entsprechende Protokoll wird geladen
    $sql = "SELECT * FROM {protocol} WHERE protocol_id = " . $protocol;
    $protocol_result = db_query($sql);

    //Sollte der Autor des Protokolls in der Gruppenteilnehmerliste vorkommen, werden die Rechte erteilt
    foreach ($protocol_result as $item) {
        $proof = in_array($item->uid, $user_list);
    }

    //Sollte der Nutzer Administrator sein, werden die Rechte ebenfalls erteilt
    global $user;
    if (in_array('administrator', $user->roles)) {
        $proof = true;
    }

    return $proof;
}

/*
 * Diese Funktion liest die Eingaben aus den Formular-Feldern und speichert sie in die Datenbank
 */
function protocol_template_save_changed_general_protocol($form, &$form_state)
{
    //Die ID des entsprechenden Protokolls wird aus der URL gelesen
    $protocol_id = arg(1);

    //Die entsprechenden Felder werden ausgelesen
    $place = $form_state['values']['place'];
    $type = $form_state['values']['plugin_select'];
    $users = $form_state['values']['teilnehmer'];
    $start = $form_state['values']['start'];
    $end = $form_state['values']['end'];

    $users = implode(',', $users);

    //Sofern Eingaben getätigt wurden...
    if (!strlen($place) < 1 && !strlen($type) < 1 && !strlen($users) < 1 && !strlen($start) < 1) {

        //... wird das Protokoll aktualisiert
        $sql = "UPDATE {protocol} SET end_date = '" . $end . "', start_date = '" . $start . "', participants = '$users', place = '$place', type = '$type' WHERE protocol_id = " . $protocol_id;
        db_query($sql);
        drupal_goto('/show_protocol/' . $protocol_id);

        //Sollten Eingaben fehlen, wird eine entsprechende Fehlermeldung ausgegeben
    } else {
        if (strlen($place) < 1) {
            drupal_set_message("Bitte trage einen Ort ein!", "error");
            $form_state['redirect'] = FALSE;
        }
        if (strlen($type) < 1) {
            drupal_set_message("Bitte trage eine Art ein!", "error");
            $form_state['redirect'] = FALSE;
        }
        if (strlen($users) < 1) {
            drupal_set_message("Bitte wähle die Teilnehmer aus!", "error");
            $form_state['redirect'] = FALSE;
        }
        if (strlen($start) < 1) {
            drupal_set_message("Bitte wähle die Startzeit aus!", "error");
            $form_state['redirect'] = FALSE;
        }
    }
}

/*
 * Diese Funktion erstellt ein Formular, in das Informationen zum Inhalt eines Protokolls eingetragen werden. Anschließend
 * können diese Informationen verändert werden.
 */
function protocol_template_change_protocol_content_information_form($form, &$form_submit)
{
    //Die ID des entsprechenden Protokolls wird aus der URL ausgelesen
    $protocol_id = arg(1);

    //es wird überprüft, ob der aktuell angemeldete Benutzer das Recht zur Veränderung des Protokolls besitzt
    if (protocol_template_check_protocol_change_permission($protocol_id)) {

        //Falls der Benutzer Rechte besitzt, wird das entsprechende Protokoll aus der Datenbank geladen
        $protocol = protocol_template_get_protocol_element_by_id($protocol_id);

        $form['content'] = array(
            '#title' => "allgemeine Informationen",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt wird zunächst nochmals auf das letzte Treffen geblickt, um sich im Anschluss optimal auf das aktuelle Treffen vorbereiten zu können."
        );

        $form['content']['preparation'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Inhalt: Was ist in diesem Treffen geplant?'),
            '#default_value' => $protocol->preparation,
        );

        $form['content']['problems'] = array(
            '#type' => 'textarea',
            '#require' => TRUE,
            '#title' => t('Probleme, die während des Treffens auftraten'),
            '#default_value' => $protocol->problems,
        );

        $form['content']['conclusion'] = array(
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
            '#submit' => array('protocol_template_save_changed_content_protocol')
        );

        return $form;
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um das Protokoll ändern zu können! <a href='" . $base_url . "'>Zurück zum Hauptmenü</a>", "error");
    }

}

/*
 * Diese Funktion liest die Eingaben aus den Formular-Feldern und speichert sie in die Datenbank
 */
function protocol_template_save_changed_content_protocol($form, &$form_state)
{
    //Die ID des entsprechenden Protokolls wird aus der URL gelesen
    $protocol_id = arg(1);

    //Die entsprechenden Felder werden ausgelesen
    $preparation = $form_state['values']['preparation'];
    $problems = $form_state['values']['problems'];
    $conclusion = $form_state['values']['conclusion'];

    //Das Protokoll wird aktualisiert
    $sql = "UPDATE {protocol} SET preparation = '$preparation', problems = '$problems', conclusion = '$conclusion' WHERE protocol_id = " . $protocol_id;
    db_query($sql);
    drupal_goto('/show_protocol/' . $protocol_id);
}

/*
 * Diese Funktion erstellt ein Formular, in das Informationen zu einer Methodendurchführung eines Protokolls
 * eingetragen werden. Anschließend können diese Informationen verändert werden.
 */
function protocol_template_change_protocol_method_information_form($form, &$form_submi)
{
    //Die ID der entsprechenden Methode wird aus der URL gelesen
    $method_id = arg(1);

    //es wird überprüft, ob der aktuell angemeldete Benutzer das Recht zur Veränderung der Methodendurchführung besitzt
    if (protocol_template_check_method_permission($method_id)) {

        //Sollte der Nutzer das Recht besitzen, wird die Methodendurchführung aus der Datenbank geladen
        $current_method = protocol_template_get_method_element_by_id($method_id);

        //Für das Dropdown-Menü werden alle verfügbaren Methoden aus der Datenbank geladen
        $methods = protocol_template_get_available_methods();

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

        $images = protocol_template_get_all_images($current_method->method_execution_id);


        $form['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $images,
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('protocol_template_change_protocol_back_to_protocol_overview')
        );

        $form['delete'] = array(
            '#type' => 'submit',
            '#value' => 'Löschen',
            '#submit' => array('protocol_template_delete_method_execution')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Speichern',
            '#submit' => array('protocol_template_save_changed_protocol_method_information')
        );

        return $form;
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Methode ändern zu können! <a href='" . $base_url . "'>Zurück zum Hauptmenü</a>", "error");
    }


}

/*
 * Diese Funktion liest die Eingaben aus den Formular-Feldern und speichert sie in die Datenbank
 */
function protocol_template_save_changed_protocol_method_information($form, &$form_state)
{
    //Die ID der entsprechenden Methode wird aus der URL ausgelesen
    $method_id = arg(1);

    //Die Variablen für jedes einzelne Feld werden angelegt
    $execution = "";
    $conclusion = "";
    $goal = "";
    $reason = "";
    $rating = "";

    //Die Felder werden ausgelesen
    $method = $form_state['values']['plugin_select1'];
    $phase = $form_state['values']['plugin_select2'];
    $room = $form_state['values']['plugin_select3'];
    $goal .= $form_state['values']['ziel'];
    $execution .= $form_state['values']['ausfuehrung'];
    $conclusion .= $form_state['values']['bewertung'];
    $reason .= $form_state['values']['grund'];
    $rating .= $form_state['values']['rating'];

    //Die Methode wird in der Datenbank aktualisiert
    $sql = "UPDATE {method_execution} SET rating = $rating, name='$method',room='$room', phase='$phase',evaluation='$conclusion',goal='$goal', execution='$execution', reason='$reason' WHERE method_execution_id = " . $method_id;
    db_query($sql);

    //Es wird überprüft, ob Dateien hochgeladen wurden
    $num_files = count($_FILES['files']['name']);

    //es wird über alle Dateien iteriert
    for ($i = 0; $i < $num_files; $i++) {
        $file = file_save_upload($i, array(
            'file_validate_is_audio' => array(),
            //Es wird überprüft, ob es sich um eine zugelassene Datei handelt
            'file_validate_extensions' => array('png pdf jpg jpeg mp3 mp4'),
            'file_validate_size' => array(5 * 1024 * 1024),
        ));

        //Sollte die Überprüfung erfolgreich sein, wird die Datei abgespeichert
        if ($file) {
            $file->status = FILE_STATUS_PERMANENT;
            if ($file = file_move($file, 'public://methods')) {
                $file->status = FILE_STATUS_PERMANENT;
                $form_state['values']['file'][$i] = $file;
                $path = substr($file->uri, 9);
                $sql = "INSERT INTO {method_execution_data} (method_execution_id, path) VALUES($method_id, '$path')";
                db_query($sql);
            }
        }
    }
    drupal_goto('protokoll_uebersicht');
}

/*
 * Diese Funktion lädt ein Protokoll-Element anhand der Protokoll-ID aus der Datenbank. Die ID wird der Funktion
 * als Parameter übergeben
 */
function protocol_template_get_protocol_element_by_id($protocol_id)
{
    //In diese Variable wird das Protokoll-Element gespeichert
    $protocol_string = "";

    //Das betreffende Protokoll-Element wird aus der Datenbank geladen
    $sql = "SELECT * FROM {protocol} WHERE protocol_id = " . $protocol_id;
    $protocol_result = db_query($sql);

    foreach ($protocol_result as $protocol) {
        $protocol_string = $protocol;
    }
    return $protocol_string;
}

/*
 * Diese Funktion lädt ein Methodenelement anhand der Methoden-ID aus der Datenbank. Die ID wird er Funktion
 * als Parameter übergeben
 */
function protocol_template_get_method_element_by_id($id)
{
    //In diese Variable wird später das Methodenelement geladen
    $method_element = "";

    //Das Methodenelement wird aus der Datenbank gelesen
    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = $id";
    $method_result = db_query($sql);

    foreach ($method_result as $method) {
        $method_element = $method;
    }
    return $method_element;
}

/*
 * Diese Funktion lädt alle im Informationssystem verfügbaren Methoden aus der Datenbank
 */
function protocol_template_get_available_methods()
{
    //In dieses Array werden später alle Methoden gespeichert
    $methods = array();

    //Die Methoden werden aus der Datenbank geladen
    $sql = "SELECT title, nid FROM {node} WHERE type = 'methode' ORDER BY title";
    $node_result = db_query($sql);

    //Es wird über jede Methode iteriert
    foreach ($node_result as $node) {

        //Der Name jeder Methode wird in die Array-Variable gespeichert
        $methods[$node->title] = $node->title;
    }
    return $methods;
}

/*
 * Diese Funktion löscht eine bestehende Methodendurchführung aus der Datenbank. Die ID der
 * betreffenden Methode wird über die URL übergeben
 */
function protocol_template_delete_method_execution()
{
    //Die ID der Methode wird aus der URL gelesen
    $method_id = arg(1);

    //Die Methodendurchführung wird aus der Datenbank gelöscht
    $sql = "DELETE FROM {method_execution} WHERE method_execution_id = $method_id";
    db_query($sql);

    //Alle zur Methodendurchführung hochgeladenen Dateien werden gelöscht
    $sql2 = "DELETE FROM {method_execution_data} WHERE method_execution_id = $method_id";
    db_query($sql2);

    drupal_set_message("Objekt wurde gelöscht");
    drupal_goto('/protokoll_methoden_uebersicht');

}

//Diese Funktion lädt alle hochgeladenen Dateien zu einer Methodendurchführung aus der Datenbank.
//Die ID der Methodendurchführung wird der Methode als Parameter übergeben
function protocol_template_get_all_images($id)
{

    global $base_url;

    //Die hochgeladenen Dateien zur Methodendurchführung werden geladen
    $sql = "SELECT * FROM {method_execution_data} WHERE method_execution_id = " . $id;
    $method_data_result = db_query($sql);

    //In diese Variable werden die einzelnen Dateien via HTML gespeichert
    $data_string = "";

    //Es wird über alle Dateien iteriert
    foreach ($method_data_result as $method) {
        $allowed = array('gif', 'png', 'jpg');
        $filename = $method->path;
        $basename = preg_replace('/^.+[\\\\\\/]/', '', $filename);
        $ext = pathinfo($filename, PATHINFO_EXTENSION);

        //Sollte es sich um eine Bilddatei handeln, wird diese angezeigt. Andernfalls wird ein Link
        //zur Datei angezeigt
        if (in_array($ext, $allowed)) {
            $path = "/" . $base_url . "/sites/default/files/" . $method->path;

            $data_string .= "<img style=\"max-width:540px;\" src='" . $path . "'alt=Selfhtml><br><b>$basename</b><a href=/" . $base_url . "/delete_file/$id/$basename/pXs8hYVUqe>&nbsp;&nbsp;Löschen</a><br><br>";
        } else {
            $data_string .= "<a href='/" . $base_url . "/sites/default/files/$method->path' target='blank'>$basename</a><a href='/" . $base_url . "/delete_file/$id/$basename/pXs8hYVUqe'>&nbsp;&nbsp;Löschen</a><br><br>";
        }
    }

    return $data_string;

}

/*
 * Diese Funktion leitet den Nutzer zurück zur Protokollanzeige
 */
function protocol_template_change_protocol_back_to_protocol_overview()
{

    $protocol_id = arg(1);
    drupal_goto('/show_protocol/' . $protocol_id);
}