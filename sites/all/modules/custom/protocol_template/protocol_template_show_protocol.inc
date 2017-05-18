<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 18.11.16
 * Time: 12:56
 */

require_once("protocol_template_dynamic_graphic.inc");

/*
 * Diese Funktion erstellt eine Anzeige in Tabellenform, in der ein gesamtes Gruppenprotokoll
 * angezeigt wird
 */
function protocol_template_show_protocol_form($form, &$form_submit)
{

    //Diese Variable speichert, ob der aktuell angemeldete Benutzer ein Administrator ist.
    $admin_check = false;

    //Der aktuell angemeldete Benutzer wird aus der Datenbank geladen
    global $user;
    if (in_array('administrator', $user->roles)) {
        $admin_check = true;
    }

    //Die ID des anzuzeigenden Protokolls wird aus der Datenbank geladen
    $protocol_id = arg(1);

    //Diese Variable speichert die Protokollanzeige als HTML-Code ab und gibt diesen zurück
    $protocol = "";

    //Das Protokoll wird aus der Datenbank geladen
    $sql = "SELECT * FROM {protocol} WHERE protocol_id = $protocol_id";
    $protocol_before_meeting_result = db_query($sql);

    //Das geladene Protokoll wird in die dafür angelegte Variable gespeichert
    foreach ($protocol_before_meeting_result as $pro) {
        $protocol = $pro;
    }

    //Die Gruppenmitglieder werden aus der Datenbank geladen
    if (in_array('administrator', $user->roles)) {
        $users = protocol_template_get_participants_by_protocol($protocol_id);
    } else {
        $users = protocol_template_get_participants_from_logged_in_user_group();
    }

    $user_list = explode(',', $users);

    //Sollte sich der Autor des Protokolls mit der ID eines der Gruppenmitglieder gleichen
    //oder der aktuell angemeldete Nutzer ein Administrator sein, wird der Zugriff auf das
    //Protokoll gewährt
    if (in_array($protocol->uid, $user_list) || $admin_check) {

        //Erstellungsdatum des Protokolls wird ermittelt und für die Anzeige konvertiert
        $start_date = new DateTime($protocol->start_date);
        $converted_start_date = $start_date->format('d.m.Y');

        //Die Überschrift der Anzeige wird erstellt
        $headline = protocol_template_get_display_headline($converted_start_date);

        //Start- und Endzeitpunkt des Protokolls wird erstellt und für die Anzeige konvertiert
        $start_date = new DateTime($protocol->start_date);
        $end_date = new DateTime($protocol->end_date);
        $converted_start_date = $start_date->format('d.m.Y H:i:s');
        $converted_end_date = $end_date->format('d.m.Y H:i:s');

        //In dieses Array werden alle Protokoll-IDs der Gruppe gespeichert
        $protocol_array = array();

        //Die Protokolle der Gruppe werden ermittelt
        $sql = "SELECT * FROM {protocol} WHERE uid IN ($users) ORDER BY protocol_id";
        $protocol_before_meeting_result = db_query($sql);

        //Es wird über alle Protokolle iteriert
        foreach ($protocol_before_meeting_result as $protocol2) {
            array_push($protocol_array, $protocol2->protocol_id);
        }
        
        //Die ID des anzuzeigenden Protokolls wird aus der Datenbank geladen
        $protocol_id = arg(1);

        //die Protokollnummer des letzten Protokolls wird ermittelt
        $protocol_nr = array_search($protocol_id, $protocol_array);

        //die Protokollnummer des aktuellen Protokolls wird erstellt
        $protocol_nr++;

        //Die Tabelle mit allgemeinen Informationen zum Protokoll wird erstellt
        $general_information_string = protocol_template_get_display_general_information($protocol_nr, $protocol_id, $converted_start_date, $converted_end_date);

        //Aufgaben, die innerhalb des Protokolls neu angelegt wurden, werden geladen
        $new_tasks = protocol_template_get_new_tasks_from_protocol($protocol);

        //Die Anzeige für Aufgaben, die innerhalb des Protokolls neu angelegt wurden, wird erstellt
        $new_task_string = protocol_template_get_display_new_tasks($new_tasks);

        //Methoden, die innerhalb des Protokolls durchgeführt wurden, werden geladen
        $method_array = protocol_template_get_methods_from_protocol($protocol_id);

        //Für die Anzeige der durchgeführten Methoden werden die Methodennamen untereinander dargestellt
        $method_list = "";

        foreach ($method_array as $m) {
            $method_list .= $m->name . "<br>";
        }

        //Die Anzeige für Informationen zum Inhalt des Protokolls wird erstellt
        $content_string = protocol_template_get_display_content_information($protocol, $method_list);

        //Methoden, die innerhalb des Protokolls durchgeführt wurden, werden erneut geladen
        $method_array = protocol_template_get_methods_from_protocol($protocol_id);

        //Für jede Methodendurchführung wird eine Tabelle erstellt. Diese Tabellen werden zurückgegeben
        $method_string = protocol_template_get_display_method_information($method_array);

        //Die Gruppenmitglieder werden aus der Datenbank geladen
        if (in_array('administrator', $user->roles)) {
            $users = protocol_template_get_participants_by_protocol($protocol_id);
        } else {
            $users = protocol_template_get_participants_from_logged_in_user_group();
        }

        //Alle Protokolle exklusiv des aktuellen Protokolls werden aus der Datenbank geladen
        $sql = "SELECT * FROM {protocol} WHERE uid IN ($users) AND protocol_id < " . $protocol_id . " ORDER BY date";
        $protocol_before_meeting_result = db_query($sql);

        //In dieses Array werden die gefundenen Protokolle eingespeichert
        $before_meeting_protocols = array();

        //Die gefundenen Protokolle werden eingespeichert
        foreach ($protocol_before_meeting_result as $pro) {
            array_push($before_meeting_protocols, $pro);
        }

        //Alle Protokolle inklusive des aktuellen Protokolls werden aus der Datenbank geladen
        $sql2 = "SELECT * FROM {protocol} WHERE uid IN ($users) AND protocol_id <= " . $protocol_id . " ORDER BY date";
        $protocol_after_meeting_result = db_query($sql2);

        //In dieses Array werden die gefundenen Protokolle eingespeichert
        $after_meeting_protocols = array();

        //Die gefundenen Protokolle werden eingespeichert
        foreach ($protocol_after_meeting_result as $r2) {
            array_push($after_meeting_protocols, $r2);
        }

        //Die Design-Thinking-Grafik mit den dynamischen Punkten für die Anzeige der Protokolle exklusive des aktuellen wird erstellt
        $dynamic_graphic_before_meeting = '
            <br>
            <h2 style="text-align:center">Phase im Design-Thinking-Prozess vor dem Meeting</h2>
            <br>
            <div>' . protocol_template_generate_graphic_and_circles($before_meeting_protocols) . '</div>';

        //Die Design-Thinking-Grafik mit den dynamischen Punkten für die Anzeige der Protokolle inklusive des aktuellen wird erstellt
        $dynamic_graphic_after_meeting = '
            <h2 style="text-align:center">Phase im Design-Thinking-Prozess nach dem Meeting</h2>
            <br>
            <div>' . protocol_template_generate_graphic_and_circles($after_meeting_protocols) . '</div>';

        //Der HTML-Code für die Protokollanzeige wird geschlossen
        $close_protocol_string = '
            <br>
            </body>
        </html>';

        $form['headline'] = array(
            '#type' => 'markup',
            '#prefix' => $headline
        );

        $form['general_information'] = array(
            '#type' => 'markup',
            '#prefix' => $general_information_string
        );

        $form['change_general_information'] = array(
            '#type' => 'submit',
            '#value' => 'Informationen bearbeiten',
            '#submit' => array('protocol_template_go_to_change_general_information')
        );

        $form['dynamic_graphic_before_meeting'] = array(
            '#type' => 'markup',
            '#prefix' => $dynamic_graphic_before_meeting
        );

        $form['content'] = array(
            '#type' => 'markup',
            '#prefix' => '<br><h2 style="text-align:center">Inhalt des Tagesberichts</h2>' . $content_string
        );
        $form['change_content'] = array(
            '#type' => 'submit',
            '#value' => 'Inhalt bearbeiten',
            '#submit' => array('protocol_template_go_to_change_preparation_information')
        );

        $form['new_task'] = array(
            '#type' => 'markup',
            '#prefix' => $new_task_string
        );

        $form['methods'] = array(
            '#type' => 'markup',
            '#prefix' => $method_string
        );
        $form['new_method'] = array(
            '#type' => 'submit',
            '#value' => 'Neue Methode durchführen',
            '#submit' => array('protocol_template_change_protocol_new_method_execution')
        );

        $form['dynamic_graphic_after_meeting'] = array(
            '#type' => 'markup',
            '#prefix' => $dynamic_graphic_after_meeting
        );

        $form['close_protocol'] = array(
            '#type' => 'markup',
            '#prefix' => $close_protocol_string
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('protocol_template_back_to_protocol_overview')
        );

        $form['delete'] = array(
            '#type' => 'submit',
            '#value' => 'Löschen',
            '#submit' => array('protocol_template_delete_protocol')
        );

        return $form;

    } else {
        global $base_url;
        drupal_set_message("Sie haben nicht die Berechtigung, dieses Dokument aufzurufen!<a href='" . $base_url . "'> Zurück zum Hauptmenü</a>", "error");
    }

}

/*
 * Diese Funktion erstellt die Überschrift der Protokollanzeige. Diese wird in Form von HTML-Code zurückgegeben
 */
function protocol_template_get_display_headline($start_date)
{

    $headline_string = '
        <html>
            <style>
                table {
                    border-collapse: collapse;
                }

                table, td, th {
                    border: 2px solid black;
                }
            </style>
            <body>
            <br>
            <h1 style="text-align:center">Tagesbericht Design-Thinking Gruppe vom ' . $start_date . '</h1>
            <br>';

    return $headline_string;
}

/*
 * Diese Funktion erstellt die Anzeige für die allgemeinen Informationen zum Protokoll. Diese wird in Form von
 * HTML-Code zurückgegeben
 */
function protocol_template_get_display_general_information($protocol_nr, $protocol_id, $converted_start_date, $converted_end_date)
{

    $protocol = protocol_template_get_protocol_from_id($protocol_id);

    $general_information_string = '
        <div>
            <table border="1" width="100%">
                <tr>
                    <th>Bezeichnung</th>
                    <th>Information</th>
                </tr>
                <tr>
                    <td width="25%">Protokoll Nr.</td>
                    <td>' . $protocol_nr . '</td>
                </tr>
                <tr>
                    <td>Teilnehmer</td>
                    <td>' . protocol_template_convert_user_id_to_username($protocol->participants) . '<br></td>
                </tr>
                <tr>
                    <td>Ort</td>
                    <td>' . $protocol->place . '</td>
                </tr>
                <tr>
                    <td>Art</td>
                    <td>' . $protocol->type . '</td>
                </tr>
                <tr>
                    <td>Datum</td>
                    <td>' . $converted_start_date . ' Uhr - ' . $converted_end_date . ' Uhr</td>
                </tr>
                <tr>
                    <td>Autor</td>
                    <td>' . protocol_template_get_username_from_user_id($protocol->uid) . '</td>
                </tr>
            </table>
        </div>';

    return $general_information_string;
}

/*
 * Diese Funktion erstellt die Anzeige für die im Protokoll angelegten neuen Aufgaben. Diese wird in Form von
 * HTML-Code zurückgegeben
 */
function protocol_template_get_display_new_tasks($new_tasks)
{

    //Die Anzahl der Aufgaben wird ermittelt
    $number_of_tasks = $new_tasks->rowCount();

    //Diese Hilfsvariable dient zur Zählweise der Aufgaben innerhalb der Anzeige
    $task_counter = 1;

    //Der Kopf der Tabelle wird erstellt
    $new_task_string = '
    <div>
        <br>
        <h2 style="text-align:center">neue Aufgaben</h2>
        <table border="1" width="100%">
            <tr>
                <th>Nr.</th>
                <th>Kurzbezeichnung</th>
                <th>Teilnehmer</th>
                <th>Aufgabe</th>
                <th>Deadline</th>
            </tr>';

    //Sollten keine Aufgaben angelegt worden sein, wird eine entsprechende Meldung angezeigt
    if ($number_of_tasks == null) {

        $new_task_string .= '
        <tr>
            <td colspan="5" style="text-align:center">keine neuen Aufgaben vorhanden</td>
        </tr>';

        //Sollten Aufgaben vorhanden sein...
    } else {

        //Es wird über jede Aufgabe iteriert
        foreach ($new_tasks as $task) {

            $start_date = new DateTime($task->deadline);
            $converted_start_date = $start_date->format('d.m.Y H:i:s');

            //Jede Aufgabe wird in eine neue Zeile geschrieben
            $new_task_string .= '
            <tr>
                <td width="5%">' . $task_counter . '</td>
                <td>' . $task->short_description . '</td>
                <td width="15%">' . protocol_template_get_username_from_user_id($task->uid) . '</td>
                <td>' . $task->task . '</td>
                <td width="25%">' . $converted_start_date . '</td>
            </tr>';

            $task_counter = $task_counter + 1;
        }
    }

    //Die Tabelle wird geschlossen
    $new_task_string .= '
        </table>
    </div>';

    return $new_task_string;
}

/*
 * Diese Funktion erstellt die Anzeige für die inhaltlichen Informationen zum Protokoll. Diese wird in Form von
 * HTML-Code zurückgegeben
 */
function protocol_template_get_display_content_information($protocol, $methods)
{

    $content_string = '
    <div>
        <table border="1" width="100%">
            <tr>
                <th>Bezeichnung</th>
                <th>Information</th>
            </tr>
            <tr>
                <td width="15%"><b>Inhalt</b></td>
                <td>' . $protocol->preparation . '</td>
            </tr>
            <tr>
                <td width="15%"><b>angewandte<br>Methoden</b></td>
                <td>' . $methods . '</td>
            </tr>
            <tr>
                <td width="15%"><b>Probleme</b></td>
                <td>' . $protocol->problems . '</td>
            </tr>
            <tr>
                <td width="15%"><b>Fazit</b></td>
                <td>' . $protocol->conclusion . '</td>
            </tr>
            <tr>
                <td width="15%"><b>Ausblick</b></td>
                <td>' . $protocol->outlook . '</td>
            </tr>
        </table>
    </div>';

    return $content_string;
}

/*
 * Diese Funktion erstellt die Anzeige für die im Protokoll durchgeführten Methoden. Diese wird in Form von
 * HTML-Code zurückgegeben
 */
function protocol_template_get_display_method_information($method_array)
{

    global $base_url;

    //Es wird ein Div-Container für die Tabellen geöffnet
    $method_execution_string = '
    <div>';

    //Sollten keine Methoden durchgeführt worden sein, wird eine entsprechende Meldung angezeigt
    if ($method_array->rowCount() > 0) {
        $method_execution_string .= '
        <br>
        <h2 style="text-align:center">Übersicht der durchgeführten Methoden</h2>';
    }

    //Sollten Aufgaben vorhanden sein, wird über jede Methode iteriert
    foreach ($method_array as $method_execution) {

        $rating_image = $base_url;

        //Die Darstellung der Methodenbewertung wird ermittelt
        switch ($method_execution->rating) {
            case 20:
                $rating_image .= "/one.png";
                break;
            case 40:
                $rating_image .= "/two.png";
                break;
            case 60:
                $rating_image .= "/three.png";
                break;
            case 80:
                $rating_image .= "/four.png";
                break;
            case 100:
                $rating_image .= "/five.png";
                break;
            default:
                break;
        }

        //Die Tabelle wird erstellt
        $method_execution_string .= '
        <table border="1" width="100%">
            <tr>
                <th colspan="2" style="text-align:center">' . $method_execution->name . '</th>
            </tr>
            <tr>
                <td width="18%"><b>Methodenname</b></td>
                <td>' . $method_execution->name . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Raum</b></td>
                <td>' . $method_execution->room . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Phase</b></td>
                <td>' . $method_execution->phase . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Ziel</b></td>
                <td>' . $method_execution->goal . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Durchführung</b></td>
                <td>' . $method_execution->execution . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Bewertung</b></td>
                <td><img src=' . $rating_image . ' height=30px. width="150px"></td>
            </tr>
            <tr>
                <td width="18%"><b>Bewertung</b></td>
                <td>' . $method_execution->evaluation . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Bilder</b></td><td align="center">
        ';

        //Die hochgeladenen Dateien zur Methodendurchführung werden geladen
        $method_execution_string .= protocol_template_get_data_for_protocol_display($method_execution->method_execution_id);

        global $base_url;

        //Die Tabelle wird geschlossen
        $method_execution_string .= '
                </td>
            </tr>
        </table>
        <a href="' . $base_url . '/protokoll_methode_aendern/' . $method_execution->method_execution_id . '/r21dPaUZRy"">Bearbeiten</a>
        <br>
        <br>';
    }

    //Der Div-Container wird geschlossen
    $method_execution_string .= '
    </div>';

    return $method_execution_string;
}

/*
 * Diese Funktion leitet den Benutzer weiter zum Formular, in das eine neue Methodendurchführung eingetragen werden kann
 */
function protocol_template_change_protocol_new_method_execution()
{
    $protocol_id = arg(1);
    drupal_goto('/protokoll_neue_Methodendurchführung/' . $protocol_id);
}

/*
 * Diese Funktion löscht das gesamte Protokoll-Element aus der Datenbank
 */
function protocol_template_delete_protocol()
{
    $protocol_id = arg(1);

    //das Protokoll-Element wird aus der Datenbank gelöscht
    $sql = "DELETE FROM {protocol} WHERE protocol_id = " . $protocol_id;
    db_query($sql);

    //Die Methodendurchführungenn zum Protokoll werden aus der Datenbank gelöscht
    $sql2 = "DELETE FROM {method_execution} WHERE protocol_id = " . $protocol_id;
    db_query($sql2);

    drupal_set_message("Protokoll wurde gelöscht");
    drupal_goto("protokoll_uebersicht");
}

/*
 * Diese Funktion lädt alle zur Methodendurchführung hochgeladenen Dateien aus der Datenbank
 */
function protocol_template_get_data_for_protocol_display($id)
{

    //Die hochgeladenen Dateien werden geladen
    $sql = "SELECT * FROM {method_execution_data} WHERE method_execution_id = " . $id;
    $method_data_result = db_query($sql);

    //In diese Variable wird der HTML-Code für die Datei-Anzeige geschrieben
    $data_string = "";

    //Es wird über alle Dateien iteriert
    foreach ($method_data_result as $data) {

        //Sollte es sich um eine Bilddatei handeln, wird diese angezeigt. Andernfalls wird der Link zur Datei angezeigt
        $allowed = array('gif', 'png', 'jpg');
        $filename = $data->path;
        $basename = preg_replace('/^.+[\\\\\\/]/', '', $filename);
        $ext = pathinfo($filename, PATHINFO_EXTENSION);
        if (in_array($ext, $allowed)) {
            global $base_url;
            $path = $base_url . "/sites/default/files/" . $data->path;

            $data_string .= "<img style=\"max-width:550px;\" src='" . $path . "'alt=Selfhtml><br><b>$basename</b><br><br>";
        }
    }

    return $data_string;
}

/*
 * Diese Funktion leitet den Benutzer weiter zu einem Formular, in dem allgemeine Informationen zum Protokoll verändert
 * werden können
 */
function protocol_template_go_to_change_general_information()
{
    $protocol_id = arg(1);
    drupal_goto('/change_protocol_general_information/' . $protocol_id);
}

/*
 * Diese Funktion leitet den Benutzer weiter zu einem Protokoll, in dem inhaltliche Informationen zum Protokoll verändert
 * werden können
 */
function protocol_template_go_to_change_preparation_information()
{
    $protocol_id = arg(1);
    drupal_goto('change_protocol_content_information/' . $protocol_id);
}

/*
 * Diese Funktion leitet den Benutzer zurück zur Protokollübersicht
 */
function protocol_template_back_to_protocol_overview()
{
    drupal_goto('/protokoll_uebersicht');
}

/*
 * Diese Funktion gibt Vor- und Nachname eines Nutzers zurück. Der Nutzer wird anhand der Nutzer-ID bestimmt
 */
function protocol_template_get_username_from_user_id($uid)
{
    $user = user_load($uid);
    $username = $user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'];
    return $username;
}

/*
 * Diese Funktion erählt eine Liste mit Nutzer-IDs, lädt den Vor- und Nachnamen des jeweiligen Nutzers und schreibt
 * diese in eine HTML-Variable. Dabei werden die Namen untereinander geschrieben
 */
function protocol_template_convert_user_id_to_username($participants)
{
    //Die Liste mit Nutzer-IDs wird zu einem Array umgewandelt
    $user_array = explode(",", $participants);

    //Die Anzahl der Nutzer wird bestimmt
    $number_of_users = count($user_array);

    //Diese Hilfsvariable wird für die Zählung der Iterationen innerhalb der Foreach-Schleife benötigt
    $iteration_counter = 0;

    //In diese Variable werden die Nutzernamen untereinander gespeichert
    $username_string = "";

    //Es wird über jede Nutzer-ID iteriert
    foreach ($user_array as $user_id) {
        $username = protocol_template_get_username_from_user_id($user_id);
        $username_string .= $username;

        //Solange es nicht die letzte Iteration ist, wird ein Zeilenumbruch gemacht
        if (++$iteration_counter != $number_of_users) {
            $username_string .= "<br>";
        }
    }
    return $username_string;
}

/*
 * Diese Funktion lädt alle Methodendurchführungen zu einem Protokoll aus der Datenbank. Das Protokoll wird anhand
 * der Protokoll-ID als Übergabeparameter bestimmt
 */
function protocol_template_get_methods_from_protocol($protocol_id)
{
    $sql = "SELECT * FROM {method_execution} WHERE protocol_id = $protocol_id";
    $method_execution_result = db_query($sql);

    return $method_execution_result;
}

/*
 * Diese Funktion lädt alle neuen Aufgaben zu einem Protokoll aus der Datenbank. Das Protokoll wird anhand
 * der Protokoll-ID als Übergabeparameter bestimmt
 */
function protocol_template_get_new_tasks_from_protocol($protocol_id)
{
    $users = protocol_template_get_participants_from_logged_in_user_group();
    $sql = "SELECT * FROM {task} WHERE uid IN ($users) AND protocol_id = $protocol_id->protocol_id AND done = 0";
    $task_result = db_query($sql);

    return $task_result;
}

function protocol_template_get_protocol_from_id($protocol_id)
{

    //Diese Variable speichert das Protokoll ab
    $protocol = "";

    //Das Protokoll wird aus der Datenbank geladen
    $sql = "SELECT * FROM {protocol} WHERE protocol_id = $protocol_id";
    $protocol_result = db_query($sql);

    //Das geladene Protokoll wird in die dafür angelegte Variable gespeichert
    foreach ($protocol_result as $pro) {
        $protocol = $pro;
    }

    return $protocol;
}