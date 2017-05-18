<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:13
 */

/*
 * Diese Funktion erstellt ein Formular, in das vom Benutzer eine neue Methodendurchführung
 * eingetragen werden kann.
 */
function protocol_template_new_method_execution_form($form, &$form_state)
{
    //Die ID des Protokolls wird aus der URL ausgelesen
    $protocol_id = arg(1);

    //Es wird geschaut, ob bereits ein offenes Protokoll existiert
    $reminder = protocol_template_check_current_open_protocol();

    //Zusätzlich wird geschaut, ob die übergebene Protokoll-ID valide ist
    if ($protocol_id != 0) {
        $reminder["exist"] = 1;
    }

    //Sollte bereits ein offenes Protokoll existieren, kann das Formular erstellt werden
    if ($reminder["exist"] == 1) {

        $form['room_type'] = array(
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('Vorbereitung'), t('Problemraum'), t('Lösungsraum'),
                t('Implementierungsraum'), t('Projektmanagement'),)),
            '#title' => t('Bitte wählen Sie einen passenden Raum'),
            '#default_value' => t('Problemraum'),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['vorbereitung'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Vorbereitungsmethode auswählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Vorbereitung')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['vorbereitung']['methodenx'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(51, 142),
            '#title' => t('Methoden für die Vorbereitung'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Vorbereitung')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['problemraum'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Phase im Problemraum auswählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Problemraum')),
                ),
            ),
        );

        $form['problemraum']['choose'] = array(
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('konvergent'), t('divergent'))),
            '#title' => t('Bitte Phase im Problemraum wählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Problemraum')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['problemraum']['konvergent_problemraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(29, 28),
            '#title' => t('Methoden Problemraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose"]' => array('value' => t('konvergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['problemraum']['divergent_problemraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(29, 27),
            '#title' => t('Methoden Problemraum divergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose"]' => array('value' => t('divergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['loesungsraum'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Phase im Lösungsraum auswählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Lösungsraum')),
                ),
            ),
        );

        $form['loesungsraum']['choose2'] = array(
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('konvergent'), t('divergent'))),
            '#title' => t('Bitte Phase im Lösungsraum wählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Lösungsraum')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['loesungsraum']['konvergent_loesungsraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(30, 28),
            '#title' => t('Methoden Lösungsraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose2"]' => array('value' => t('konvergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['loesungsraum']['divergent_loesungsraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(30, 27),
            '#title' => t('Methoden Lösungsraum divergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose2"]' => array('value' => t('divergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['implementierungsraum'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Phase im Implementierungsraum auswählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Implementierungsraum')),
                ),
            ),

        );

        $form['implementierungsraum']['choose3'] = array(
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('konvergent'), t('divergent'))),
            '#title' => t('Bitte Phase im Implementierungsraum wählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Implementierungsraum')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['implementierungsraum']['konvergent_implementierungsraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(31, 28),
            '#title' => t('Methoden Implementierungsraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose3"]' => array('value' => t('konvergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['implementierungsraum']['divergent_implementierungsraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(31, 27),
            '#title' => t('Methoden Implementierungsraum divergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose3"]' => array('value' => t('divergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'protocol_template_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['projektmanagement'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Projektmanagement-Methode auswählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Projektmanagement')),
                ),
            ),
        );

        $form['projektmanagement']['methoden'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(52, 143),
            '#title' => t('Methoden für das Projektmanagement'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Projektmanagement')),
                ),
            ),
        );

        $form['checkboxes_fieldset'] = array(
            '#title' => t("Link zur Methodenbeschreibung"),
            '#prefix' => '<div id="checkboxes-div">',
            '#suffix' => '</div>',
            '#type' => 'fieldset',
        );

        $raum = $form_state['values']['room_type'];
        $phase = "";
        $number_room = 0;
        $number_phase = 0;

        if (!strcmp($raum, "Problemraum")) {
            $phase = $form_state['values']['choose'];
        } else if (!strcmp($raum, "Lösungsraum")) {
            $phase = $form_state['values']['choose2'];
        } else {
            $phase = $form_state['values']['choose3'];
        }

        switch ($raum) {
            case "Vorbereitung":
                $method = $form_state['values']['methodenx'];
                $number_room = 51;
                $number_phase = 142;
                break;
            case "Problemraum":
                if (!strcmp($phase, "konvergent")) {
                    $method = $form_state['values']['konvergent_problemraum'];
                    $number_room = 29;
                    $number_phase = 28;

                } else {
                    $method = $form_state['values']['divergent_problemraum'];
                    $number_room = 29;
                    $number_phase = 27;
                }
                break;
            case "Lösungsraum":
                if (!strcmp($phase, "konvergent")) {
                    $method = $form_state['values']['konvergent_loesungsraum'];
                    $number_room = 30;
                    $number_phase = 28;
                } else {
                    $method = $form_state['values']['divergent_loesungsraum'];
                    $number_room = 30;
                    $number_phase = 27;
                }
                break;
            case "Implementierungsraum":
                if (!strcmp($phase, "konvergent")) {
                    $method = $form_state['values']['konvergent_implementierungsraum'];
                    $number_room = 31;
                    $number_phase = 28;
                } else {
                    $method = $form_state['values']['divergent_implementierungsraum'];
                    $number_room = 31;
                    $number_phase = 27;
                }
                break;
            case "Projektmanagement":
                $method = $form_state['values']['methoden'];
                break;
        }

        global $base_url;

        $node = 0;
        $sql = "SELECT * FROM {node} WHERE title = '" . $method . "'";
        $result = db_query($sql);
        foreach ($result as $item) {
            $node = $item->nid;
        }
        $test = "Bitte eine Methode auswählen";

        if ($node != 0) {
            $test = "<a target=\"_blank\" href=\"" . $base_url . "/node/" . $node . "/Methodenauswahl/" . $number_room . "/" . $number_phase . "\">Link zur Methodenbeschreibung (" . $method . ")</a>";
        }


        $form['checkboxes_fieldset']["link"] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $test
        );

        $form['method_fieldset'] = array(

            '#type' => 'fieldset',
            '#title' => t('Bitte geben Sie alle nötigen Informatione für die Dokumentation einer Methodendurchführung ein'),
        );

        $form['method_fieldset']['grund'] = array(
            '#title' => t('Grund der Methodenwahl'),
            '#resizable' => FALSE,
            '#type' => 'textarea'
        );

        $form['method_fieldset']['ziel'] = array(
            '#title' => t('Ziel der Methode'),
            '#resizable' => FALSE,
            '#type' => 'textarea'
        );

        $form['method_fieldset']['ausfuehrung'] = array(
            '#title' => t('Ausführung der Methode'),
            '#resizable' => FALSE,
            '#type' => 'textarea'
        );

        $form['method_fieldset']['rating'] = array(
            '#type' => 'fivestar',
            '#stars' => 5,
            '#default_value' => 20,
            '#title' => t('Bewertung der Methode'),
        );

        $form['method_fieldset']['bewertung'] = array(
            '#title' => t('Grund der Bewertung'),
            '#resizable' => FALSE,
            '#type' => 'textarea'
        );

        $form['method_fieldset']['file'] = array(
            '#type' => 'file',
            '#name' => 'files[]',
            '#title' => t('Datei-Upload'),
            '#attributes' => array('multiple' => 'multiple'),
        );


        $types = array("Problemraum" => "Problemraum", "Lösungsraum" => "Lösungsraum", "Implementierungsraum" => "Implementierungsraum");

        $form['path_fieldset'] = array(

            '#type' => 'fieldset',
            '#title' => t('Bitte wählen Sie den neuen Raum und die neue Phase der Gruppe'),
        );

        $form['path_fieldset']["raum2"]["plugin_select_raum_end"] = array(
            "#type" => "select",
            "#title" => t("Raum am Ende der Methodendurchführung (In welchem Raum befindet ihr euch nach dem Durchführen der Methode?)"),
            "#options" => $types,
            "#default_value" => $types["Problemraum"],
        );

        $types2 = array("konvergent" => "konvergent", "divergent" => "divergent");


        $form['path_fieldset']["phase3"]["plugin_select_phase_end"] = array(
            "#type" => "select",
            "#title" => t("Phase am Ende der Methodendurchführung (In welcher Phase befindet ihr euch nach dem Durchführen der Methode?)"),
            "#options" => $types2,
            "#default_value" => $types2["konvergent"],
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück zur Methodenübersicht',
            '#submit' => array('protocol_template_back_to_method_execution')
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Speichern',
            '#submit' => array('protocol_template_save_method_execution')
        );


    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
<a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
', "error");
    }


    return $form;
}

/*
 * Diese Funktion initiiert das Speichern der Methodendurchführung
 */
function protocol_template_save_method_execution($form, &$form_state)
{
    save_data($form, $form_state);
}

/*
 * Diese Funktion wird aufgerufen, sobald sich bei der Wahl einer passenden Methode ein Wert
 * in einem der Dropdown-Menüs ändert. Die Funktion gibt ein Fieldset zurück, welches bis dahin
 * verborgen war
 */
function protocol_template_autocheckboxes_callback($form, $form_state)
{
    return $form['checkboxes_fieldset'];
}


/*
 * Diese Funktion führt den Benutzer zurück zur jeweiligen Webseite, von der aus er das
 * Methodendurchführungsformular aufgerufen hat
 */
function protocol_template_back_to_method_execution()
{
    //Die ID der Methodendurchführung wird aus der URL ausgelesen
    $method_id = arg(1);

    //Die ID des Protokolls wird anhand der Methoden-ID ermittelt
    $protocol_id = 0;
    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = " . $method_id;
    $result = db_query($sql);
    foreach ($result as $item) {
        $protocol_id = $item->protocol_id;
    }

    //Anhand dieser Variable kann der Weg zu diesem Formular nachvollzogen werden. So wird eine
    //optimale Weiterleitung zurück zur vorigen Seite gewährleistet
    $pass_code = arg(2);

    //Sollte die letzte Seite die Methodenübersicht innerhalb des Protokoll-Templates gewesen sein...
    if (!strcmp($pass_code, "pXs8hYVUqe")) {
        drupal_goto('/protokoll_methoden_uebersicht');

        //Sollte die letzte Seite die Protokollübersicht gewesen sein...
    } else if (!strcmp($pass_code, "t8HVhPRlPZ")) {
        drupal_goto('/protokoll_uebersicht');

        //Sollte die letzte Seite die Methodenübersicht gewesen sein...
    } else if (!strcmp($pass_code, "zqjvEz44Qx")) {
        drupal_goto('/methoden_uebersicht');

        //Sollte die letzte Seite die Protokollanzeige gewesen sein...
    } else if (!strcmp($pass_code, "r21dPaUZRy")) {
        drupal_goto('/protokollanzeige/' . $protocol_id);
    } else {
        drupal_goto('/protokoll_methoden_uebersicht');
    }
}

/*
 * Diese Funktion liest alle Formular-Felder aus und speichert diese in die Datenbank ab
 */
function save_data($form, &$form_state)
{

    //Der Raum der Methode wird ausgelesen
    $method_room = $form_state['values']['room_type'];
    $method_phase = "";

    //Die Phase der Methode wird bestimmt
    if (!strcmp($method_room, "Problemraum")) {
        $method_phase = $form_state['values']['choose'];
    } else if (!strcmp($method_room, "Lösungsraum")) {
        $method_phase = $form_state['values']['choose2'];
    } else {
        $method_phase = $form_state['values']['choose3'];
    }

    //Je nachdem, welche Werte innerhalb der Dropdown-Menüs ausgewählt wurden,
    //muss das richtige Feld ausgelesen werden
    switch ($method_room) {
        case "Vorbereitung":
            $method = $form_state['values']['methodenx'];
            break;
        case "Problemraum":
            if (!strcmp($method_phase, "konvergent")) {
                $method = $form_state['values']['konvergent_problemraum'];

            } else {
                $method = $form_state['values']['divergent_problemraum'];
            }
            break;
        case "Lösungsraum":
            if (!strcmp($method_phase, "konvergent")) {
                $method = $form_state['values']['konvergent_loesungsraum'];
            } else {
                $method = $form_state['values']['divergent_loesungsraum'];
            }
            break;
        case "Implementierungsraum":
            if (!strcmp($method_phase, "konvergent")) {
                $method = $form_state['values']['konvergent_implementierungsraum'];

            } else {
                $method = $form_state['values']['divergent_implementierungsraum'];
            }
            break;
        case "Projektmanagement":
            $method = $form_state['values']['methoden'];
            break;
    }

    //Die Gruppenmitglieder werden ermittelt
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //Die ID des aktuell offenen Protokolls der Gruppe wird ermittelt
    $protocol_id = arg(1);

    //Sollte ein Fehler bei der Protokoll-ID vorliegen, wird das aktuell offene Protokoll
    //erneut aus der Datenbank geladen
    if ($protocol_id == 0) {
        $protocol_id = protocol_template_get_current_open_group_protocol($users);
    }

    //Die User-ID des aktuell angemeldeten Nutzers wird ermittelt
    global $user;
    $user_id = $user->uid;

    //Die Variablen, in die später die Werte aus den Formular-Feldern eingespeichert werden,
    //werden angelegt
    $execution = "";
    $conclusion = "";
    $goal = "";
    $reason = "";

    //Die Formular-Felder werden ausgelesen
    $goal .= $form_state['values']['ziel'];
    $execution .= $form_state['values']['ausfuehrung'];
    $conclusion .= $form_state['values']['bewertung'];
    $reason .= $form_state['values']['grund'];
    $rating = $form_state['values']['rating'];
    $today = date("Y-m-d H:i:s");

    //Raum und Phase der Gruppe nach Beendigung der Methode werden ermittelt
    $end_room = $form_state['values']['plugin_select_raum_end'];
    $end_phase = $form_state['values']['plugin_select_phase_end'];

    //Sollte es sich bei der Methode um eine Vorbereitungs- oder Projektmanagement-Methode handeln,
    //kann keine spezifische Phase zugeordnet werden. Sie wird als neutral bezeichnet
    if (!strcmp($method_room, "Vorbereitung") || !strcmp($method_room, "Projektmanagement")) {
        $method_phase = "neutral";
    }

    //Die Methodendurchführung mit allen Attributen wird in die Datenbank geschrieben
    $sql = "INSERT INTO {method_execution} (uid, protocol_id, name, evaluation, room, phase, goal, execution, rating, raum_end, phase_end, reason, creation_date)
      VALUES
    (
    $user_id,
    $protocol_id,
    '$method',
    '$conclusion',
    '$method_room',
    '$method_phase',
    '$goal',
    '$execution',
    '$rating',
    '$end_room',
    '$end_phase',
    '$reason',
    '$today');";

    //Nach der Speicherung wird gleich die ID der Methode zurückgegeben
    $method_id = db_query($sql, array(), array('return' => Database::RETURN_INSERT_ID));

    //Die Anzahl der hochgeladenenn Dateien wird ermittelt
    $num_files = count($_FILES['files']['name']);

    //Es wird über alle Dateien iteriert
    for ($i = 0; $i < $num_files; $i++) {

        //Es wird überprüft, ob es sich um eine zugelassene Dateiendung handelt
        $file = file_save_upload($i, array(
            'file_validate_is_audio' => array(),
            'file_validate_extensions' => array('png pdf jpg jpeg mp3 mp4'),
            'file_validate_size' => array(20 * 1024 * 1024),
        ));

        //War die Überprüfung erfolgreich, wird die Datei abgespeichert
        if ($file) {
            if ($file = file_move($file, 'public://methods')) {
                $file->status = FILE_STATUS_PERMANENT;
                $form_state['values']['file'][$i] = $file;
                $path = substr($file->uri, 9);
                $sql = "INSERT INTO {method_execution_data} (method_execution_id, path) VALUES($method_id, '$path')";
                db_query($sql);
            }
        }
    }

    drupal_set_message("Dokumentation zur Methode wurde angelegt");

    $protocol_id = arg(1);

    $participants = protocol_template_get_participants_from_logged_in_user_group();
    $current_open_id = protocol_template_get_current_open_group_protocol($participants);

    //Sollte die ID des aktuell offenen Protokolls vorliegen, wird dessen Raum und Phase nach Beendigung des Protokolls
    //aktualisiert
    if ($protocol_id != $current_open_id) {
        $sql = "UPDATE {protocol} SET raum_end = '" . $end_room . "', phase_end = '" . $end_phase . "' WHERE protocol_id = " . $protocol_id;
        db_query($sql);
        drupal_goto('/protokollanzeige/' . $protocol_id);

    } else {
        drupal_goto('/protokoll_methoden_uebersicht');
    }
}

/*
 * Diese Funktion überprüft, ob der aktuell angemeldete Nutzer die Berechtigung besitzt, die Methodendurchführung
 * einzusehen beziehungsweise zu bearbeiten. Die ID der Methodendurchführung wird per Parameter übergeben
 */
function protocol_template_check_method_permission($method_id)
{

    $proof = false;
    $method = $method_id;

    //Die Gruppenmitglieder des aktuell angemeldeten Nutzers werden geladen
    $user_list = protocol_template_get_participants_from_logged_in_user_group();
    $user_array = explode(',', $user_list);

    //Die entsprechende Methodendurchführung wird aus der Datenbank geladen
    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = " . $method;
    $result = db_query($sql);

    //Sollte der Autor der Methodendurchführung mit der ID eines der Gruppenmitglieder übereinstimmen,
    //wird die Berechtigung erteilt
    foreach ($result as $item) {
        $proof = in_array($item->uid, $user_array);
    }

    //Sollte es sich bei dem aktuell angemeldetem Nutzer um einen Administrator handeln, wird die
    //Berechtigung erteilt
    global $user;
    if (in_array('administrator', $user->roles)) {
        $proof = true;
    }

    return $proof;
}

/*
 * Diese Funktion erzeugt ein Formular, in das eine bestehende Methodendurchführung verändert werden kann.
 * Dafür wird im Vorfeld die Methodendurchführung aus der Datenbank geladen und in die Formular-Felder
 * gefüllt.
 */
function protocol_template_change_method_execution_form($form, &$form_submit)
{
    //Die ID der Methodendurchführung wird aus der Datenbank geladen
    $method_id = arg(1);

    //Es wird überprüft, ob der aktuell angemeldete Nutzer die Berechtigung besitzt
    if (protocol_template_check_method_permission($method_id)) {

        //Das Methodendurchführungselement wird aus der Datenbank geladen
        $current_method = protocol_template_get_method_by_id($method_id);

        $form['room_type'] = array(
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('Problemraum'), t('Lösungsraum'),
                t('Implementierungsraum'))),
            '#title' => t('Bitte wählen Sie einen passenden Raum'),
            "#default_value" => $current_method->room,
        );

        $form['problemraum'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Phase im Problemraum auswählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Problemraum')),
                ),
            ),
        );

        $form['problemraum']['choose'] = array(
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('konvergent'), t('divergent'))),
            '#title' => t('Bitte Phase im Problemraum wählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Problemraum')),
                ),
            ),
            "#default_value" => $current_method->phase
        );

        $form['problemraum']['konvergent_problemraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(29, 28),
            '#title' => t('Methoden Problemraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose"]' => array('value' => t('konvergent')),
                ),
            ),
            "#default_value" => $current_method->name,
        );

        $form['problemraum']['divergent_problemraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(29, 27),
            '#title' => t('Methoden Problemraum divergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose"]' => array('value' => t('divergent')),
                ),
            ),
            "#default_value" => $current_method->name,
        );

        $form['loesungsraum'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Phase im Lösungsraum auswählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Lösungsraum')),
                ),
            ),
        );

        $form['loesungsraum']['choose2'] = array(
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('konvergent'), t('divergent'))),
            '#title' => t('Bitte Phase im Lösungsraum wählen:'),

            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Lösungsraum')),
                ),
            ),
            "#default_value" => $current_method->phase

        );

        $form['loesungsraum']['konvergent_loesungsraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(30, 28),
            '#title' => t('Methoden Lösungsraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose2"]' => array('value' => t('konvergent')),
                ),
            ),
            "#default_value" => $current_method->name,
        );

        $form['loesungsraum']['divergent_loesungsraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(30, 27),
            '#title' => t('Methoden Lösungsraum divergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose2"]' => array('value' => t('divergent')),
                ),
            ),
            "#default_value" => $current_method->name,
        );

        $form['implementierungsraum'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Phase im Implementierungsraum auswählen:'),
            "#default_value" => $current_method->phase,
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Implementierungsraum')),
                ),
            ),
        );

        $form['implementierungsraum']['choose3'] = array(

            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('konvergent'), t('divergent'))),
            '#title' => t('Bitte Phase im Implementierungsraum wählen:'),

            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Implementierungsraum')),
                ),
            ),
            "#default_value" => $current_method->phase
        );

        $form['implementierungsraum']['konvergent_implementierungsraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(31, 28),
            '#title' => t('Methoden Implementierungsraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose3"]' => array('value' => t('konvergent')),
                ),
            ),
            "#default_value" => $current_method->name,
        );

        $form['implementierungsraum']['divergent_implementierungsraum'] = array(
            '#type' => 'select',
            '#options' => protocol_template_get_methods_from_room_and_phase(31, 27),
            '#title' => t('Methoden Implementierungsraum divergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose3"]' => array('value' => t('divergent')),
                ),
            ),
            "#default_value" => $current_method->name,
        );

        $form['main'] = array(

            '#type' => 'fieldset',
            '#title' => t('Angabe aller nötigen Informationen'),
        );

        $form['main']['grund'] = array(
            '#title' => t('Grund der Methodenwahl'),
            '#resizable' => FALSE,
            "#default_value" => $current_method->reason,
            '#type' => 'textarea'
        );

        $form['main']['ziel'] = array(
            '#title' => t('Ziel der Methode'),
            '#resizable' => FALSE,
            "#default_value" => $current_method->goal,
            '#type' => 'textarea'
        );

        $form['main']['ausfuehrung'] = array(
            '#title' => t('Ausführung der Methode'),
            '#resizable' => FALSE,
            "#default_value" => $current_method->execution,
            '#type' => 'textarea'
        );

        $form['main']['rating'] = array(
            '#type' => 'fivestar',
            '#stars' => 5,
            '#default_value' => $current_method->rating,
            '#title' => t('Bewertung der Methode'),
        );

        $form['main']['bewertung'] = array(
            '#title' => t('Grund für die Bewertung'),
            '#resizable' => FALSE,
            "#default_value" => $current_method->evaluation,
            '#type' => 'textarea'
        );

        $form['main']['file'] = array(
            '#type' => 'file',
            '#name' => 'files[]',
            '#title' => t('Datei-Upload'),
            '#attributes' => array('multiple' => 'multiple'),
        );

        $types = array("Problemraum" => "Problemraum", "Lösungsraum" => "Lösungsraum", "Implementierungsraum" => "Implementierungsraum");

        $form['end'] = array(

            '#type' => 'fieldset',
            '#title' => t('Angabe der Position nach Bearbeitung der Methode'),
        );

        $form['end']["raum2"]["plugin_select_raum_end"] = array(
            "#type" => "select",
            "#title" => t("Raum am Ende des Treffens (In welchem Raum befindet ihr euch nach der Methodendurchführung?)"),
            "#options" => $types,
            '#default_value' => $current_method->raum_end,
        );

        $types2 = array("konvergent" => "konvergent", "divergent" => "divergent");


        $form['end']["phase3"]["plugin_select_phase_end"] = array(
            "#type" => "select",
            "#title" => t("Phase am Ende des Treffens (In welcher Phase befindet ihr euch nach der Methodendurchführung?)"),
            "#options" => $types2,
            '#default_value' => $current_method->phase_end,
        );


        $data_elements = protocol_template_get_images_from_method($current_method->method_execution_id, true);

        $form['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $data_elements,
        );


        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Speichern',
            '#submit' => array('protocol_template_update_method_execution')
        );

        $form['delete'] = array(
            '#type' => 'submit',
            '#value' => 'Löschen',
            '#submit' => array('protocol_template_delete_method_execution_element')
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück zur Methodenübersicht',
            '#submit' => array('back_to_method_execution')
        );


        return $form;

    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Methode ändern zu können! <a href='" . $base_url . "'>Zurück zum Hauptmenü</a>", "error");
    }
}

/*
 * Diese Funktion liest alle Informationen aus den Formularfeldern aus und aktualisiert die Methodendurchführung
 * in der Datenbank
 */
function protocol_template_update_method_execution($form, &$form_state)
{
    //Die ID der Methodendurchführung wird aus der URL ausgelesen
    $method_id = arg(1);

    //Die Formular-Felder werden ausgelesen
    $rating = $form_state['values']['rating'];
    $goal = $form_state['values']['ziel'];
    $execution = $form_state['values']['ausfuehrung'];
    $conclusion = $form_state['values']['bewertung'];
    $reason = $form_state['values']['grund'];
    $end_room = $form_state['values']['plugin_select_raum_end'];
    $end_phase = $form_state['values']['plugin_select_phase_end'];
    $method_room = $form_state['values']['room_type'];
    $method_phase = "";

    //Je nachdem, welche Werte in den Dropdown-Menüs ausgewählt wurden,
    //wird das entsprechende Feld der Phase der Methode ausgelesen
    if (!strcmp($method_room, "Problemraum")) {
        $method_phase = $form_state['values']['choose'];
    } else if (!strcmp($method_room, "Lösungsraum")) {
        $method_phase = $form_state['values']['choose2'];
    } else {
        $method_phase = $form_state['values']['choose3'];
    }

    //Je nachdem, welche Werte in den Dropdown-Menüs ausgewählt wurden,
    //wird das entsprechende Feld der Methode ausgelesen
    switch ($method_room) {
        case "Problemraum":
            if (!strcmp($method_phase, "konvergent")) {
                $method = $form_state['values']['konvergent_problemraum'];
            } else {
                $method = $form_state['values']['divergent_problemraum'];
            }
            break;
        case "Lösungsraum":
            if (!strcmp($method_phase, "konvergent")) {
                $method = $form_state['values']['konvergent_loesungsraum'];
            } else {
                $method = $form_state['values']['divergent_loesungsraum'];
            }
            break;
        case "Implementierungsraum":
            if (!strcmp($method_phase, "konvergent")) {
                $method = $form_state['values']['konvergent_implementierungsraum'];

            } else {
                $method = $form_state['values']['divergent_implementierungsraum'];
            }
            break;
    }

    //Die Methodendurchführung wird in der Datenbank aktualisiert
    $sql = "UPDATE {method_execution} SET
      name = '$method',
      evaluation = '$conclusion',
      room = '$method_room',
      phase = '$method_phase',
      goal = '$goal',
      execution = '$execution',
      path = '',
      rating = '$rating',
      raum_end = '$end_room',
      phase_end ='$end_phase',
      reason = '$reason'
      WHERE method_execution_id = '$method_id'";

    db_query($sql);

    //Die hochgeladenen Dateien zur Methodendurchführung werden ermittelt
    $num_files = count($_FILES['files']['name']);

    //Es wird über alle Dateien iteriert
    for ($i = 0; $i < $num_files; $i++) {

        //Es wird überprüft, ob es sich um eine gültige Dateiendung handelt
        $file = file_save_upload($i, array(
            'file_validate_is_audio' => array(),
            'file_validate_extensions' => array('png pdf jpg jpeg mp3 mp4'),
            'file_validate_size' => array(20 * 1024 * 1024),
        ));

        //Wenn die Überprüfung erfolgreich war, wird die Datei abgespeichert
        if ($file) {
            $file->status = FILE_STATUS_PERMANENT;
            if ($file = file_move($file, 'public://methods')) {
                $file->status = FILE_STATUS_PERMANENT;
                $file->status = 1;
                $form_state['values']['file'][$i] = $file;
                $path = substr($file->uri, 9);
                $sql = "INSERT INTO {method_execution_data} (method_execution_id, path) VALUES($method_id, '$path')";
                db_query($sql);
            }
        }
    }

    //Die Protokoll-ID des aktuell offenen Protokolls wird ermittelt
    $protocol_id = 0;
    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = " . $method_id;
    $result = db_query($sql);


    foreach ($result as $item) {
        $protocol_id = $item->protocol_id;
    }

    //Anhand dieser Variable kann der Weg zu diesem Formular nachvollzogen werden. So wird eine
    //optimale Weiterleitung zurück zur vorigen Seite gewährleistet
    $pass_code = arg(2);

    drupal_set_message("Dokumentation zur Methode wurde geändert");

    //Sollte die letzte Seite die Methodenübersicht innerhalb des Protokoll-Templates gewesen sein...
    if (!strcmp($pass_code, "pXs8hYVUqe")) {
        drupal_goto('/protokoll_methoden_uebersicht');

        //Sollte die letzte Seite die Protokollübersicht gewesen sein...
    } else if (!strcmp($pass_code, "t8HVhPRlPZ")) {
        drupal_goto('/protokoll_uebersicht');

        //Sollte die letzte Seite die Methodenübersicht gewesen sein...
    } else if (!strcmp($pass_code, "zqjvEz44Qx")) {
        drupal_goto('/methoden_uebersicht');

        //Sollte die letzte Seite die Protokollanzeige gewesen sein...
    } else if (!strcmp($pass_code, "r21dPaUZRy")) {
        drupal_goto('/protokollanzeige/' . $protocol_id);
    } else {
        drupal_set_message("Ein Weiterleitungsfehler ist aufgetreten. Sie wurden zur Startseite weitergeleitet", "error");
        drupal_goto('');
    }
}

/*
 * Diese Funktion lädt eine Methodendurchführung anhand einer per Parameter übergebenen Methodendurchführungs-ID
 * aus der Datenbank
 */
function protocol_template_get_method_by_id($method_id)
{
    $method = "";

    //Die Methodendurchführung wird aus der Datenbank geladen
    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = $method_id";
    $result = db_query($sql);

    //Das gefundene Methodendurchführungselement wird in die vorher angelegte Variable abgespeichert
    foreach ($result as $r) {
        $method = $r;
    }
    return $method;
}

/*
 * Diese Funktion löscht eine Methodendurchführung aus der Datenbank
 */
function protocol_template_delete_method_execution_element()
{
    //Die ID der Methodendurchführung wird aus der URL ausgelesen
    $method_id = arg(1);

    //Die Methodendurchführung wird aus der Datenbank gelöscht
    $sql = "DELETE FROM {method_execution} WHERE method_execution_id = $method_id";
    db_query($sql);

    //Alle zur Methodendurchführung gehörenden Dateien werden aus der Datenbank gelöscht
    $sql2 = "DELETE FROM {method_execution_data} WHERE method_execution_id = $method_id";
    db_query($sql2);

    drupal_set_message("Objekt wurde gelöscht");

    //Anhand dieser Variable kann der Weg zu diesem Formular nachvollzogen werden. So wird eine
    //optimale Weiterleitung zurück zur vorigen Seite gewährleistet
    $pass_code = arg(2);

    //Sollte die letzte Seite die Methodenübersicht innerhalb des Protokoll-Templates gewesen sein...
    if (!strcmp($pass_code, "pXs8hYVUqe")) {
        drupal_goto('/protokoll_methoden_uebersicht');

        //Sollte die letzte Seite die Protokollübersicht gewesen sein...
    } else if (!strcmp($pass_code, "t8HVhPRlPZ")) {
        drupal_goto('/protokoll_uebersicht');

        //Sollte die letzte Seite die Methodenübersicht gewesen sein...
    } else if (!strcmp($pass_code, "zqjvEz44Qx")) {
        drupal_goto('/methoden_uebersicht');
    } else {
        drupal_set_message("Ein Weiterleitungsfehler ist aufgetreten. Sie wurden zur Startseite weitergeleitet", "error");
        drupal_goto('');
    }

}

/*
 * Diese Funktion erstellt ein Formular, in das eine Methodendurchführung geladen wird. Dieses Formular dient
 * nur zur Anzeige und kann nicht verändert werden
 */
function protocol_template_show_method_execution_form($form, &$form_submit)
{
    //Die ID der Methodendurchführung wird aus der URL ausgelesen
    $method_id = arg(1);

    //Es wird überprüft, ob der aktuell angemeldete Benutzer die Berechtigung besitzt, die Methodendurchführung
    //einzusehen
    if (protocol_template_check_method_permission($method_id)) {

        //Sollte die ID der Methodendurchführung valide sein
        if (strlen($method_id) > 0) {

            //Die Methodendurchführung wird aus der Datenbank geladen
            $current_method = protocol_template_get_method_by_id($method_id);

            //Sollte das Laden aus der Datenbank erfolgreich sein
            if (!empty($current_method)) {

                $current_method = protocol_template_get_method_by_id($method_id);

                $form['problemraum'] = array(

                    '#type' => 'fieldset',
                    '#title' => t('Auswahl der passenden Methode'),
                );


                $form['problemraum']["raum"]["plugin_select3"] = array(
                    "#type" => "select",
                    "#title" => t("Bitte passenden Raum aussuchen"),
                    "#default_value" => $current_method->room,
                    "#disabled" => TRUE,
                    "#options" => array(
                        "Vorbereitung" => t("Vorbereitung"),
                        "Problemraum" => t("Problemraum"),
                        "Lösungsraum" => t("Lösungsraum"),
                        "Implementierungsraum" => t("Implementierungsraum"),
                        "Projektmanagement" => t("Projektmanagement")
                    ),
                    "#description" => t("Wähle Raum."),
                );

                $form['problemraum']["phase"]["plugin_select2"] = array(
                    "#type" => "select",
                    "#title" => t("Bitte passende Phase aussuchen"),
                    "#disabled" => TRUE,
                    "#default_value" => $current_method->phase,
                    "#options" => array(
                        "konvergent" => t("konvergent"),
                        "divergent" => t("divergent"),
                    ),
                    "#description" => t("Wähle Phase."),
                    '#states' => array(
                        'visible' => array(
                            ':input[name="plugin_select3"]' => array('value' => t('Problemraum'), t('Lösungsraum'), t('Implementierungsraum')),
                        ),
                    ),
                );

                $form['problemraum']["method"]["plugin_select1"] = array(
                    "#type" => "select",
                    "#title" => t("Bitte passende Methode aussuchen"),
                    "#disabled" => TRUE,
                    "#options" => array(1 => "$current_method->name"),
                    "#description" => t("Wähle Methode."),
                );

                $form['main'] = array(

                    '#type' => 'fieldset',
                    '#title' => t('Angabe aller nötigen Informationen'),
                );

                $form['main']['reason'] = array(
                    '#title' => t('Grund der Methodenwahl'),
                    '#resizable' => FALSE,
                    "#default_value" => $current_method->reason,
                    "#disabled" => TRUE,
                    '#type' => 'textarea'
                );

                $form['main']['ziel'] = array(
                    '#title' => t('Ziel der Methode'),
                    '#resizable' => FALSE,
                    "#default_value" => $current_method->goal,
                    "#disabled" => TRUE,
                    '#type' => 'textarea'
                );

                $form['main']['ausfuehrung'] = array(
                    '#title' => t('Ausführung der Methode'),
                    '#resizable' => FALSE,
                    "#default_value" => $current_method->execution,
                    "#disabled" => TRUE,
                    '#type' => 'textarea'
                );

                $form['main']['rating'] = array(
                    '#type' => 'fivestar',
                    '#stars' => 5,
                    "#disable" => true,
                    '#default_value' => $current_method->rating,
                    '#title' => t('Bewertung der Methode.'),
                );

                $form['main']['bewertung'] = array(
                    '#title' => t('Bewertung der Ergebnisse'),
                    '#resizable' => FALSE,
                    "#default_value" => $current_method->evaluation,
                    '#type' => 'textarea',
                    "#disabled" => TRUE,
                );

                $types = array("Problemraum" => "Problemraum", "Lösungsraum" => "Lösungsraum", "Implementierungsraum" => "Implementierungsraum");

                $form['end'] = array(

                    '#type' => 'fieldset',
                    '#title' => t('Angabe der Position nach Bearbeitung der Methode'),
                );

                $form['end']["raum2"]["plugin_select_raum_end"] = array(
                    "#type" => "select",
                    "#title" => t("Raum am Ende der Methodendurchführung (In welchem Raum befindet ihr euch nach dem Durchführen der Methode?)"),
                    "#options" => $types,
                    "#default_value" => $current_method->raum_end,
                    "#disabled" => TRUE,
                );

                $types2 = array("konvergent" => "konvergent", "divergent" => "divergent");


                $form['end']["phase3"]["plugin_select_phase_end"] = array(
                    "#type" => "select",
                    "#title" => t("Phase am Ende der Methodendurchführung (In welcher Phase befindet ihr euch nach dem Durchführen der Methode?)"),
                    "#options" => $types2,
                    "#default_value" => $current_method->phase_end,
                    "#disabled" => TRUE,
                );

                $images = protocol_template_get_images_from_method($current_method->method_execution_id, false);

                $form['form_item'] = array(
                    '#type' => 'markup',
                    '#title' => t('My Form Item'),
                    '#prefix' => $images,
                );

                $form['back'] = array(
                    '#type' => 'submit',
                    '#value' => 'Zurück',
                    '#submit' => array('back_to_overview')
                );

                $form['change'] = array(
                    '#type' => 'submit',
                    '#value' => 'Bearbeiten',
                    '#submit' => array('protocol_template_show_protocol_change')
                );

            } else {
                drupal_set_message("Die angeforderte Methode existiert leider nicht", "error");
                $form['back'] = array(
                    '#type' => 'submit',
                    '#value' => 'Zurück',
                    '#submit' => array('back_to_overview')
                );
            }

        } else {
            drupal_set_message("Die angeforderte Methode existiert leider nicht", "error");
            $form['back'] = array(
                '#type' => 'submit',
                '#value' => 'Zurück',
                '#submit' => array('back_to_overview')
            );
        }


        return $form;
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Methode ansehen zu können! <a href='" . $base_url . "'>Zurück zum Hauptmenü</a>", "error");
    }


}

/*
 * Diese Funktion leitet den Benutzer weiter zu einem Formular, in dem die Methodendurchführung verändert
 * werden kann
 */
function protocol_template_show_protocol_change()
{
    //Die ID der Methodendurchführung wird übergeben
    $method_id = arg(1);

    //Der Passcode wird übergeben, um den Weg zum nächsten Formular nachvollziehen zu können
    $check_pw = arg(2);

    drupal_goto("/protokoll_methode_aendern/$method_id/$check_pw");

}

/*
 * Diese Funktion lädt alle hochgeladenen Dateien zu einer Methodendurchführung aus der Datenbank aus
 * und wandelt sie in HTML-Code um. Weiterhin wird über eine Boolean-Variable überprüft, ob der Nutzer
 * die Möglichkeit bekommen soll, die Datei aus der Datenbank zu löschen oder nicht.
 */
function protocol_template_get_images_from_method($method_id, $delete_check)
{

    global $base_url;

    //Die Daten zur Methodendurchführung werden aus der Datenbank geladen
    $sql = "SELECT * FROM {method_execution_data} WHERE method_execution_id = " . $method_id;
    $method_data_result = db_query($sql);

    //Die Überschrift für die Dateien wird erstellt
    $method_data_string = "<h2>Anhang</h2><br><br>";

    //Die Aufzählung für die Dateinen wird begonnen
    $method_data_string .= "<ul>";

    //Es wird über alle Dateien iteriert
    foreach ($method_data_result as $method_data) {

        //Sollte es sich um eine Bilddatei handeln, kann dieses angezeigt werden.
        //Andernfalls wird nur der Link zur Datei dargestellt
        $allowed = array('gif', 'png', 'jpg');
        $filename = $method_data->path;
        $basename = preg_replace('/^.+[\\\\\\/]/', '', $filename);
        $ext = pathinfo($filename, PATHINFO_EXTENSION);

        //Sollte es außerdem einen Link geben, mit dem die Datei gelöscht werden kann, wird
        //dieser ebenfalls hinzugefügt
        if ($delete_check) {
            if (in_array($ext, $allowed)) {
                $path = $base_url . "/sites/default/files/" . $method_data->path;

                $method_data_string .= "<li><img style=\"max-width:540px;\" src='" . $path . "'alt=Selfhtml><br><b>$basename</b><a href='" . $base_url . "/delete_file/$method_id/$basename/zqjvEz44Qxyy'\">&nbsp;&nbsp;Löschen</a></li><br><br>";
            } else {
                $method_data_string .= '<li><a href="' . $base_url . '/sites/default/files/' . $method_data->path . '" target="_blank">' . $basename . '</a><a href="' . $base_url . '/delete_file/' . $method_id . '/' . $basename . '/zqjvEz44Qxyy">&nbsp;&nbsp;Löschen</a></li><br><br>';
            }

            //Sollte das Löschen der Datei nicht erlaubt sein, wird lediglich die Datei angezeigt
        } else {
            if (in_array($ext, $allowed)) {
                $path = $base_url . "/sites/default/files/" . $method_data->path;

                $method_data_string .= "<li><img style=\"max-width:540px;\" src='" . $path . "'alt=Selfhtml></li><br><b>$basename</b><br><br>";
            } else {
                $method_data_string .= '<li><a href="' . $base_url . '/sites/default/files/' . $method_data->path . '" target="_blank">' . $basename . '</a></li><br><br>';
            }
        }

    }

    //Die Aufzählung wird beendet
    $method_data_string .= "</ul>";

    return $method_data_string;

}

//Diese Funktion leitet den Benutzer zurück zur vorigen Seite. Diese wird anhand eines Passcodes aus der URL
//bestimmt.
function back_to_overview()
{
    //Der Passcode wird aus der URL ausgelesen
    $pass = arg(2);

    //Sollte es sich bei der vorigen Seite um die Methodenübersicht im Rahmen des Protokoll-Templates handeln
    if (!strcmp($pass, "pXs8hYVUqe")) {
        drupal_goto('/protokoll_methoden_uebersicht');

        //Sollte es sich um die Protokollübersicht handeln
    } else if (!strcmp($pass, "t8HVhPRlPZ")) {
        drupal_goto('/protokoll_uebersicht');

        //Sollte es sich um die Methodenübersicht handeln
    } else if (!strcmp($pass, "zqjvEz44Qx")) {
        drupal_goto('/methoden_uebersicht');
    } else {
        drupal_set_message("Ein Weiterleitungsfehler ist aufgetreten. Sie wurden zur Startseite weitergeleitet", "error");
        drupal_goto('');
    }
}

/*
 * Diese Funktion bekommt als Parameter einen Raum und eine Phase. Im Anschluss lädt sie alle Methoden,
 * die innerhalb dieses Raums und dieser Phase angewendet werden können, aus der Datenbank
 */
function protocol_template_get_methods_from_room_and_phase($room, $phase)
{
    //Die Raum-ID wird als Übergabeparameter übergeben
    $raum_id = $room;

    //Die Phasen-ID wird als Übergabeparameter übergeben
    $phase_id = $phase;

    //In diese Variable werden die Methoden eingespeichert
    $method_array = array();

    //Die Methoden werden aus der Datenbank geladen
    $method_result = db_query("SELECT * FROM {node} AS node INNER JOIN {field_data_field_raum} AS raum ON node.nid = raum.entity_id INNER JOIN {field_data_field_phase} AS phase ON raum.entity_id = phase.entity_id WHERE field_raum_target_id = :raum_id AND field_phase_target_id = :phase_id ORDER BY node.title ASC", array(
        ':raum_id' => $raum_id,
        ':phase_id' => $phase_id,
    ));

    //Es wird über jede Methode iteriert. Jeder Methodentitel wird in das Array gespeichert
    foreach ($method_result as $r) {
        $method_array[$r->title] = $r->title;
    }

    return $method_array;
}