<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:13
 */

function test_module_new_method_execution_form($form, &$form_state)
{

    $protocol_id = arg(1);

    $reminder = check_current_open_protocol();

    if($protocol_id != 0) {
        $reminder["exist"] = 1;
    }

    if ($reminder["exist"] == 1) {
        $methods = get_available_methods();

        $form['room_type'] = array(
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('Vorbereitung'), t('Problemraum'), t('Lösungsraum'),
                t('Implementierungsraum'), t('Projektmanagement'),)),
            '#title' => t('Bitte wählen Sie einen passenden Raum'),
            '#default_value' => t('Problemraum'),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['vorbereitung'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Vorbereitungsmethode auswählen:'),
            '#states' => array(
                'visible' => array (
                    ':input[name="room_type"]' => array('value' => t('Vorbereitung')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['vorbereitung']['methodenx'] = array (
            '#type' => 'select',
            '#options' => get_right_method(51,142),
            '#title' => t('Methoden für die Vorbereitung'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Vorbereitung')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );


        $form['problemraum'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Phase im Problemraum auswählen:'),
            '#states' => array(
                'visible' => array (
                    ':input[name="room_type"]' => array('value' => t('Problemraum')),
                ),
            ),
        );

        $form['problemraum']['choose'] = array (
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('konvergent'), t('divergent'))),
            '#title' => t('Bitte Phase im Problemraum wählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Problemraum')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['problemraum']['konvergent_problemraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(29,28),
            '#title' => t('Methoden Problemraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose"]' => array('value' => t('konvergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['problemraum']['divergent_problemraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(29,27),
            '#title' => t('Methoden Problemraum divergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose"]' => array('value' => t('divergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),


        );

        $form['loesungsraum'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Phase im Lösungsraum auswählen:'),
            '#states' => array(
                'visible' => array (
                    ':input[name="room_type"]' => array('value' => t('Lösungsraum')),
                ),
            ),
        );

        $form['loesungsraum']['choose2'] = array (
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('konvergent'), t('divergent'))),
            '#title' => t('Bitte Phase im Lösungsraum wählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Lösungsraum')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['loesungsraum']['konvergent_loesungsraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(30,28),
            '#title' => t('Methoden Lösungsraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose2"]' => array('value' => t('konvergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['loesungsraum']['divergent_loesungsraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(30,27),
            '#title' => t('Methoden Lösungsraum divergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose2"]' => array('value' => t('divergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['implementierungsraum'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Phase im Implementierungsraum auswählen:'),
            '#states' => array(
                'visible' => array (
                    ':input[name="room_type"]' => array('value' => t('Implementierungsraum')),
                ),
            ),

        );

        $form['implementierungsraum']['choose3'] = array (
            '#type' => 'select',
            '#options' => drupal_map_assoc(array(t('konvergent'), t('divergent'))),
            '#title' => t('Bitte Phase im Implementierungsraum wählen:'),
            '#states' => array(
                'visible' => array(
                    ':input[name="room_type"]' => array('value' => t('Implementierungsraum')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['implementierungsraum']['konvergent_implementierungsraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(31,28),
            '#title' => t('Methoden Implementierungsraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose3"]' => array('value' => t('konvergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['implementierungsraum']['divergent_implementierungsraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(31,27),
            '#title' => t('Methoden Implementierungsraum divergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose3"]' => array('value' => t('divergent')),
                ),
            ),
            '#ajax' => array(
                'callback' => 'test_module_autocheckboxes_callback',
                'wrapper' => 'checkboxes-div',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );

        $form['projektmanagement'] = array(

            '#type' => 'fieldset',
            '#title' => t('bitte passende Projektmanagement-Methode auswählen:'),
            '#states' => array(
                'visible' => array (
                    ':input[name="room_type"]' => array('value' => t('Projektmanagement')),
                ),
            ),
        );

        $form['projektmanagement']['methoden'] = array (
            '#type' => 'select',
            '#options' => get_right_method(52,143),
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

        if(!strcmp($raum, "Problemraum")) {
            $phase = $form_state['values']['choose'];
        } else if(!strcmp($raum, "Lösungsraum")) {
            $phase = $form_state['values']['choose2'];
        } else {
            $phase = $form_state['values']['choose3'];
        }

        switch($raum) {
            case "Vorbereitung":
                $method = $form_state['values']['methodenx'];
                $number_room = 51;
                $number_phase = 142;
                break;
            case "Problemraum":
                if(!strcmp($phase, "konvergent")) {
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
                if(!strcmp($phase, "konvergent")) {
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
                if(!strcmp($phase, "konvergent")) {
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
        $sql = "SELECT * FROM {node} WHERE title = '".$method."'";
        $result = db_query($sql);
        foreach ($result as $item) {
            $node = $item->nid;
        }
        $test = "Bitte eine Methode auswählen";

        if($node != 0) {
            $test = "<a target=\"_blank\" href=\"".$base_url."/node/".$node."/Methodenauswahl/".$number_room."/".$number_phase."\">Link zur Methodenbeschreibung (".$method.")</a>";
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
            '#submit' => array('back_to_method_execution')
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Speichern',
            '#submit' => array('save_method_execution')
        );


    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
<a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
', "error");
    }


    return $form;
}

/**
 * Callback element needs only select the portion of the form to be updated.
 * Since #ajax['callback'] return can be HTML or a renderable array (or an
 * array of commands), we can just return a piece of the form.
 */
function test_module_autocheckboxes_callback($form, $form_state) {
    return $form['checkboxes_fieldset'];
}

function save_method_execution($form, &$form_state)
{
    save_data($form, $form_state);
}

function back_to_method_execution()
{
    $protocol_id = 0;
    $method_id= arg(1);
    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = ".$method_id;
    $result = db_query($sql);

    foreach ($result as $item) {
        $protocol_id = $item->protocol_id;
    }


    $pass = arg(2);
    if (!strcmp($pass, "pXs8hYVUqe")) {
        drupal_goto('/protokoll_methoden_uebersicht');
    } else if (!strcmp($pass, "t8HVhPRlPZ")) {
        drupal_goto('/protokoll_uebersicht');

    } else if (!strcmp($pass, "zqjvEz44Qx")) {
        drupal_goto('/methoden_uebersicht');
    } else if (!strcmp($pass, "r21dPaUZRy")) {
        drupal_goto('/protokollanzeige/'.$protocol_id);
    }

    else {
        //drupal_set_message("Ein Weiterleitungsfehler ist aufgetreten. Sie wurden zur Startseite weitergeleitet", "error");
        //drupal_goto('');
        drupal_goto('/protokoll_methoden_uebersicht');
    }


}

function get_available_methods()
{
    $methods = array();
    $sql = "SELECT title, nid FROM {node} WHERE type = 'methode' ORDER BY title";
    $result = db_query($sql);
    foreach ($result as $r) {
        $methods[$r->title] = $r->title;
    }
    return $methods;
}


function save_data($form, &$form_state)
{
    $raum = $form_state['values']['room_type'];
    $phase = "";

    if(!strcmp($raum, "Problemraum")) {
        $phase = $form_state['values']['choose'];
    } else if(!strcmp($raum, "Lösungsraum")) {
        $phase = $form_state['values']['choose2'];
    } else {
        $phase = $form_state['values']['choose3'];
    }

    switch($raum) {
        case "Vorbereitung":
            $method = $form_state['values']['methodenx'];
            break;
        case "Problemraum":
            if(!strcmp($phase, "konvergent")) {
                $method = $form_state['values']['konvergent_problemraum'];

            } else {
                $method = $form_state['values']['divergent_problemraum'];
            }
        break;
        case "Lösungsraum":
            if(!strcmp($phase, "konvergent")) {
                $method = $form_state['values']['konvergent_loesungsraum'];
            } else {
                $method = $form_state['values']['divergent_loesungsraum'];
            }
        break;
        case "Implementierungsraum":
            if(!strcmp($phase, "konvergent")) {
                $method = $form_state['values']['konvergent_implementierungsraum'];

            } else {
                $method = $form_state['values']['divergent_implementierungsraum'];
            }
        break;
        case "Projektmanagement":
            $method = $form_state['values']['methoden'];
            break;
    }

    $parts = getParticipants2();

    $protocol_parameter = arg(1);

    if($protocol_parameter != 0) {
        $protocol_id = $protocol_parameter;
    } else {
        $protocol_id = get_open_protocol2($parts);
    }

    global $user;
    $user_id = $user->uid;

    $ausfuehrung = "";
    $fazit = "";
    $ziel = "";
    $grund = "";

    $ziel .= $form_state['values']['ziel'];
    $ausfuehrung .= $form_state['values']['ausfuehrung'];
    $fazit .= $form_state['values']['bewertung'];
    $grund .= $form_state['values']['grund'];
    $rating = $form_state['values']['rating'];

    $raum2 = $form_state['values']['plugin_select_raum_end'];
    $phase2 = $form_state['values']['plugin_select_phase_end'];

    $heute = date("Y-m-d H:i:s");

    if(!strcmp($raum, "Vorbereitung") || !strcmp($raum, "Projektmanagement")) {
        $phase = "neutral";
    }


    $sql = "INSERT INTO {method_execution} (uid, protocol_id, name, evaluation, room, phase, goal, execution, rating, raum_end, phase_end, reason, creation_date)
VALUES
(
$user_id,
$protocol_id,
'$method',
'$fazit',
'$raum',
'$phase',
'$ziel',
'$ausfuehrung',
'$rating',
'$raum2',
'$phase2',
'$grund',
'$heute');";


    $method_id = db_query($sql, array(), array('return' => Database::RETURN_INSERT_ID));


    $num_files = count($_FILES['files']['name']);

    $main_path = "";

    for ($i = 0; $i < $num_files; $i++) {
        $file = file_save_upload($i, array(
            'file_validate_is_audio' => array(),
            'file_validate_extensions' => array('png pdf jpg jpeg mp3 mp4'),
            'file_validate_size' => array(20 * 1024 * 1024),
        ));
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

    if($protocol_parameter != 0) {
        $sql = "UPDATE {protocol} SET raum_end = '".$raum2."', phase_end = '".$phase2."' WHERE protocol_id = ".$protocol_parameter;
        db_query($sql);
        drupal_goto('/protokollanzeige/'.$protocol_parameter);

    } else {
        drupal_goto('/protokoll_methoden_uebersicht');
    }

}

function test_module_check_method_permission($method_id) {

    $proof = false;
    $method = $method_id;

    $list = getParticipants2();
    $list = explode(',', $list);

    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = ".$method;
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


function test_module_change_method_execution_form($form, &$form_submit)
{
    $method_id = arg(1);
    $pass = arg(2);

    if(test_module_check_method_permission($method_id)) {
        $current_method = get_current_method($method_id);

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
                'visible' => array (
                    ':input[name="room_type"]' => array('value' => t('Problemraum')),
                ),
            ),
        );

        $form['problemraum']['choose'] = array (
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

        $form['problemraum']['konvergent_problemraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(29,28),
            '#title' => t('Methoden Problemraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose"]' => array('value' => t('konvergent')),
                ),
            ),
            "#default_value" => $current_method->name,
        );

        $form['problemraum']['divergent_problemraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(29,27),
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
                'visible' => array (
                    ':input[name="room_type"]' => array('value' => t('Lösungsraum')),
                ),
            ),
        );

        $form['loesungsraum']['choose2'] = array (
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

        $form['loesungsraum']['konvergent_loesungsraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(30,28),
            '#title' => t('Methoden Lösungsraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose2"]' => array('value' => t('konvergent')),
                ),
            ),
            "#default_value" => $current_method->name,
        );

        $form['loesungsraum']['divergent_loesungsraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(30,27),
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
                'visible' => array (
                    ':input[name="room_type"]' => array('value' => t('Implementierungsraum')),
                ),
            ),
        );

        $form['implementierungsraum']['choose3'] = array (

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

        $form['implementierungsraum']['konvergent_implementierungsraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(31,28),
            '#title' => t('Methoden Implementierungsraum konvergent'),
            '#states' => array(
                'visible' => array(
                    ':input[name="choose3"]' => array('value' => t('konvergent')),
                ),
            ),
            "#default_value" => $current_method->name,
        );

        $form['implementierungsraum']['divergent_implementierungsraum'] = array (
            '#type' => 'select',
            '#options' => get_right_method(31,27),
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




        $test = get_images_from_method($current_method->method_execution_id, true);

        /*
        $t = explode(",", $current_method->path);

        $test = "";

        foreach ($t as $te) {
            $filepath = '/design_thinking_tool/sites/default/files/' . $te . '';
            $basename = preg_replace('/^.+[\\\\\\/]/', '', $filepath);

            $test .= "<img src='$filepath' height='200' width='200' alt=Selfhtml><br><b>$basename</b><br>";


        }
        */


        $form['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $test,
        );


        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Speichern',
            '#submit' => array('update_method_execution')
        );

        $form['delete'] = array(
            '#type' => 'submit',
            '#value' => 'Löschen',
            '#submit' => array('delete_method_execution')
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück zur Methodenübersicht',
            '#submit' => array('back_to_method_execution')
        );


        return $form;

    } else {
        global $base_url;
    drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Methode ändern zu können! <a href='".$base_url."'>Zurück zum Hauptmenü</a>", "error");
    }
}

function update_method_execution($form, &$form_state)
{
    $method_id = arg(1);
    $pass = arg(2);
    $rating = $form_state['values']['rating'];
    $ziel = $form_state['values']['ziel'];
    $ausfuehrung = $form_state['values']['ausfuehrung'];
    $fazit = $form_state['values']['bewertung'];
    $grund = $form_state['values']['grund'];
    $raum2 = $form_state['values']['plugin_select_raum_end'];
    $phase2 = $form_state['values']['plugin_select_phase_end'];

    $raum = $form_state['values']['room_type'];
    $phase = "";

    if(!strcmp($raum, "Problemraum")) {
        $phase = $form_state['values']['choose'];
    } else if(!strcmp($raum, "Lösungsraum")) {
        $phase = $form_state['values']['choose2'];
    } else {
        $phase = $form_state['values']['choose3'];
    }

    switch($raum) {
        case "Problemraum":
            if(!strcmp($phase, "konvergent")) {
                $method = $form_state['values']['konvergent_problemraum'];

            } else {
                $method = $form_state['values']['divergent_problemraum'];
            }
            break;
        case "Lösungsraum":
            if(!strcmp($phase, "konvergent")) {
                $method = $form_state['values']['konvergent_loesungsraum'];
            } else {
                $method = $form_state['values']['divergent_loesungsraum'];
            }
            break;
        case "Implementierungsraum":
            if(!strcmp($phase, "konvergent")) {
                $method = $form_state['values']['konvergent_implementierungsraum'];

            } else {
                $method = $form_state['values']['divergent_implementierungsraum'];
            }
            break;
    }

    $sql = "UPDATE {method_execution} SET name = '$method', evaluation = '$fazit', room = '$raum', phase = '$phase', goal = '$ziel', execution = '$ausfuehrung', path = '', rating = '$rating', raum_end = '$raum2', phase_end ='$phase2', reason = '$grund' WHERE method_execution_id = '$method_id'";


    $result = db_query($sql);

    $num_files = count($_FILES['files']['name']);
    $main_path = "";

    for ($i = 0; $i < $num_files; $i++) {
        $file = file_save_upload($i, array(
            'file_validate_is_audio' => array(),
            'file_validate_extensions' => array('png pdf jpg jpeg mp3 mp4'),
            'file_validate_size' => array(20 * 1024 * 1024),
        ));
        if ($file) {
            $file->status = FILE_STATUS_PERMANENT;
            //$mydir = 'public://methods/';
            //file_prepare_directory($mydir, FILE_CREATE_DIRECTORY);
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


    $protocol_id = 0;
    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = ".$method_id;
    $result = db_query($sql);

    foreach ($result as $item) {
        $protocol_id = $item->protocol_id;
    }

    drupal_set_message("Dokumentation zur Methode wurde geändert");
    if (!strcmp($pass, "pXs8hYVUqe")) {
        drupal_goto('/protokoll_methoden_uebersicht');
    } else if (!strcmp($pass, "t8HVhPRlPZ")) {
        drupal_goto('/protokoll_uebersicht');

    } else if (!strcmp($pass, "zqjvEz44Qx")) {
        drupal_goto('/methoden_uebersicht');

    } else if (!strcmp($pass, "r21dPaUZRy")) {
        drupal_goto('/protokollanzeige/'.$protocol_id);
    }



    else {
        drupal_set_message("Ein Weiterleitungsfehler ist aufgetreten. Sie wurden zur Startseite weitergeleitet", "error");
        drupal_goto('');
    }
}

function get_current_method($id)
{
    $method = "";
    $sql = "SELECT * FROM {method_execution} WHERE method_execution_id = $id";
    $result = db_query($sql);
    foreach ($result as $r) {
        $method = $r;
    }
    return $method;
}

function delete_method_execution()
{
    $method_id = arg(1);
    $sql = "DELETE FROM {method_execution} WHERE method_execution_id = $method_id";
    $result = db_query($sql);

    $sql2 = "DELETE FROM {method_execution_data} WHERE method_execution_id = $method_id";
    db_query($sql2);
    drupal_set_message("Objekt wurde gelöscht");

    $pass = arg(2);


    if (!strcmp($pass, "pXs8hYVUqe")) {
        drupal_goto('/protokoll_methoden_uebersicht');
    } else if (!strcmp($pass, "t8HVhPRlPZ")) {
        drupal_goto('/protokoll_uebersicht');

    } else if (!strcmp($pass, "zqjvEz44Qx")) {
        drupal_goto('/methoden_uebersicht');
    } else {
        drupal_set_message("Ein Weiterleitungsfehler ist aufgetreten. Sie wurden zur Startseite weitergeleitet", "error");
        drupal_goto('');
    }

}

function test_module_show_method_execution_form($form, &$form_submit)
{
    $method_id = arg(1);

    if(test_module_check_method_permission($method_id)) {
        if (strlen($method_id) > 0) {

            $temp = get_current_method($method_id);

            if (!empty($temp)) {
                $current_method = get_current_method($method_id);

                drupal_set_message($current_method->room);

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
                        'visible' => array (
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

                $test = get_images_from_method($current_method->method_execution_id, false);

                $form['form_item'] = array(
                    '#type' => 'markup',
                    '#title' => t('My Form Item'),
                    '#prefix' => $test,
                );

                $form['back'] = array(
                    '#type' => 'submit',
                    '#value' => 'Zurück',
                    '#submit' => array('back_to_overview')
                );

                $form['change'] = array(
                    '#type' => 'submit',
                    '#value' => 'Bearbeiten',
                    '#submit' => array('test_module_show_protocol_change')
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
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Methode ansehen zu können! <a href='".$base_url."'>Zurück zum Hauptmenü</a>", "error");
    }






}

function test_module_show_protocol_change() {
    $method_id = arg(1);
    $check_pw = arg(2);

    drupal_set_message($check_pw);

    drupal_goto("/protokoll_methode_aendern/$method_id/$check_pw");

}

function get_images_from_method($id, $delete) {

    global $base_url;

    $sql = "SELECT * FROM {method_execution_data} WHERE method_execution_id = ".$id;

    $result = db_query($sql);

    $temp = "<h2>Anhang</h2><br><br>";

    $temp .="<ul>";

    foreach($result as $r) {
        $allowed =  array('gif','png' ,'jpg');
        $filename = $r->path;
        $basename = preg_replace('/^.+[\\\\\\/]/', '', $filename);
        $ext = pathinfo($filename, PATHINFO_EXTENSION);

        if($delete) {
            if(in_array($ext,$allowed) ) {
                $path = $base_url."/sites/default/files/".$r->path;

                $temp .= "<li><img style=\"max-width:540px;\" src='".$path."'alt=Selfhtml><br><b>$basename</b><a href='".$base_url."/delete_file/$id/$basename/zqjvEz44Qxyy'\">&nbsp;&nbsp;Löschen</a></li><br><br>";
            } else {
                $temp .= '<li><a href="'.$base_url.'/sites/default/files/'.$r->path.'" target="_blank">'.$basename.'</a><a href="'.$base_url.'/delete_file/'.$id.'/'.$basename.'/zqjvEz44Qxyy">&nbsp;&nbsp;Löschen</a></li><br><br>';
            }
        } else {
            if(in_array($ext,$allowed) ) {
                $path = $base_url."/sites/default/files/".$r->path;

                $temp .= "<li><img style=\"max-width:540px;\" src='".$path."'alt=Selfhtml></li><br><b>$basename</b><br><br>";
            } else {
                $temp .= '<li><a href="'.$base_url.'/sites/default/files/'.$r->path.'" target="_blank">'.$basename.'</a></li><br><br>';
            }
        }


    }
    $temp .="</ul>";

    return $temp;

}

function back_to_overview()
{
    $method_id = arg(1);
    $pass = arg(2);
    if (!strcmp($pass, "pXs8hYVUqe")) {
        drupal_goto('/protokoll_methoden_uebersicht');
    } else if (!strcmp($pass, "t8HVhPRlPZ")) {
        drupal_goto('/protokoll_uebersicht');

    } else if (!strcmp($pass, "zqjvEz44Qx")) {
        drupal_goto('/methoden_uebersicht');
    } else {
        drupal_set_message("Ein Weiterleitungsfehler ist aufgetreten. Sie wurden zur Startseite weitergeleitet", "error");
        drupal_goto('');
    }
}

function get_right_method($raum, $phase) {



    $raum_id = $raum;
    $phase_id = $phase;
    $array = array();

    $result = db_query("SELECT * FROM {node} AS node INNER JOIN {field_data_field_raum} AS raum ON node.nid = raum.entity_id INNER JOIN {field_data_field_phase} AS phase ON raum.entity_id = phase.entity_id WHERE field_raum_target_id = :raum_id AND field_phase_target_id = :phase_id ORDER BY node.title ASC", array(
        ':raum_id' => $raum_id,
        ':phase_id' => $phase_id,
    ));

    foreach ($result as $r) {
        $array[$r->title] = $r->title;
    }

    return $array;
}