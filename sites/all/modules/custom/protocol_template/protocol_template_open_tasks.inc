<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:43
 */

/*
 * Diese Fuktion erstellt zwei Tabellen. Innerhalb der ersten Tabelle werden alle Aufgaben angezeigt, die innerhalb
 * des letzten Protokolls erstellt wurden. In der zweiten Tabelle werden alle noch offenen Aufgaben der Gruppe präsentiert
 */
function protocol_template_get_open_task_form($form, &$form_submit)
{

    //Es wird geschaut, ob bereits ein offenes Protokoll existiert
    $reminder = protocol_template_check_current_open_protocol();

    //Sollte bereits ein offenes Protokoll existieren, können die Tabellen erstellt werden
    if ($reminder["exist"] == 1) {

        //Die Tabelle mit den Aufgaben aus dem letzten Protokoll wird erstellt
        $table_task_last_protocol = protocol_template_get_table_task_last_protocol();

        //Die Tabelle mit allen offenen Aufgaben der Gruppe wird erstellt
        $table_all_open_protocols = protocol_template_get_table_all_open_tasks();


        $form['open_tasks_last_protocol_fieldset'] = array(
            '#title' => "Übersicht über Aufgaben aus dem letzten Protokoll",
            '#type' => 'fieldset',
            '#description' => "In dieser Tabelle werden alle Aufgaben angezeigt, die innerhalb des letzten Gruppentreffens angelegt wurden."
        );

        $form['open_tasks_last_protocol_fieldset']['form_item_last_protocol'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $table_task_last_protocol
        );


        $form['open_tasks_all_open_fieldset'] = array(
            '#title' => "Übersicht über alle noch offenen Aufgaben",
            '#type' => 'fieldset',
            '#description' => "In dieser Tabelle werden alle Aufgaben angezeigt, die bis zu diesem Zeitpunkt noch nicht erledigt wurden."
        );

        $form['open_tasks_all_open_fieldset']['form_item_all_open_protocol'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $table_all_open_protocols
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('protocol_template_open_task_back_to_general_information')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('protocol_template_open_task_submit_method_overview')
        );

    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
            <a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
            ', "error");
    }

    return $form;
}

/*
 * Diese Funktion liest alle Aufgaben der Gruppe, die innerhalb des letzten Protokolls angelegt wurden, aus der Datenbank
 * und speichert sie in eine HTML-Tabelle. Diese Tabelle wird am Ende zurückgegeben
 */
function protocol_template_get_table_task_last_protocol()
{

    global $base_url;

    //Die Gruppenmitglieder werden ermittelt
    global $user;
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //Die ID des aktuell offenen Protokolls wird ermittelt
    $protocol_id = protocol_template_get_current_open_group_protocol($users);

    $sql2 = "SELECT * FROM {protocol} WHERE uid IN ($users)";
    $protocol_result = db_query($sql2);

    //Diese Variable zählt die Durchläufe in der Foreach-Schleife
    $counter = 0;

    //Diese Variable hilft bei der Ermittlung der ID des letzten Protokolls
    $helper_id = 0;

    //Diese Variable speichert die ID des letzten Protokolls
    $last_protocol_id = 0;

    //Es wird über alle Protokolle der Gruppe iteriert
    foreach ($protocol_result as $protocol) {

        //Sollte die ID des gefundenen Protokolls mit dem des aktuell offenen Protokolls übereinstimmen...
        if ($protocol_id == $protocol->protocol_id) {
            //wird die ID des Protokolls davor gepeichert
            $last_protocol_id = $helper_id;
        }

        //Andersnfalls wird die ID des gefundenen Protokolls gespeichert
        $helper_id = $protocol->protocol_id;
        $counter++;
    }

    //Der Kopf der Tabelle wird erstellt
    $task_table_string = "
        <table>
            <th>Kurzbezeichnung</th>
            <th>zugehöriger Benutzer (Wer?)</th>
            <th>Aufgabe (Was?</th>
            <th>Deadline (Wann?)</th>
            <th>Status</th>
            <th>Aktionen</th>";


    //Es werden alle Aufgaben geladen, die als ID die des letzten Protokolls besitzen
    $sql = "SELECT * FROM {task} AS t WHERE t.uid IN ($users) AND t.protocol_id = " . $last_protocol_id . " ORDER BY t.deadline";
    $protocol_task_result = db_query($sql);

    //Es wird über alle Aufgaben iteriert
    foreach ($protocol_task_result as $last_protocol_task) {

        //Die Deadline der Aufgabe wird für die Anzeige konvertiert
        $deadline = new DateTime($last_protocol_task->deadline);
        $converted_deadline = $deadline->format('d.m.Y H:i:s');

        //Das User-Element des zugewiesenen Nutzers wird ermittelt
        $user = user_load($last_protocol_task->uid);

        //Die Aufgaben-ID wird ermittelt
        $task_id = $last_protocol_task->task_id;

        //Sofern die Aufgabe bereits erledigt wurde...
        if ($last_protocol_task->done) {
            $done = "<font color=\"green\">erledigt</font><td></td>";
        } else {
            $done = "<font color=\"red\">offen</font></td><td><a href='" . $base_url . "/delete_task/$task_id/pXs8hYVUqe'/>erledigt</a></td></tr></tr>";
        }

        //Jede Aufgabe wird in eine neue Zeile der Tabelle geschrieben
        $task_table_string = $task_table_string . "
            <tr>
                <td>$last_protocol_task->short_description</td>
                <td>" . $user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'] . "</td>
                <td>$last_protocol_task->task</td><td>$converted_deadline</td>
                <td>" . $done .
            "</tr>";
    }

    //Die Tabelle wird geschlossen
    $task_table_string .= "</table><br>";

    return $task_table_string;
}

/*
 * Diese Funktion liest alle noch offenen Aufgaben der Gruppe, aus der Datenbank
 * und speichert sie in eine HTML-Tabelle. Diese Tabelle wird am Ende zurückgegeben
 */
function protocol_template_get_table_all_open_tasks()
{

    global $base_url;

    //Die Gruppenmitglieder werden geladen
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //Die ID des aktuell offenen Protokolls wird geladen
    $protocol_id = protocol_template_get_current_open_group_protocol($users);

    //Die offenen Aufgaben der Gruppe werden aus der Datenbank geladen
    $sql = "SELECT * FROM {task} AS t WHERE t.uid IN ($users) AND t.done = 0 ORDER BY t.deadline";
    $task_result = db_query($sql);

    //Der Kopf der Tabelle wird erstellt
    $task_table_string = "
        <table>
            <th>Kurzbezeichnung</th>
            <th>zugehöriger Benutzer (Wer?)</th>
            <th>Aufgabe (Was?</th>
            <th>Deadline (Wann?)</th>
            <th>Status</th>
            <th>Aktionen</th>";

    //Es wird über alle Aufgaben iteriert
    foreach ($task_result as $open_task) {

        //Die Deadline der Aufgabe wird für die Anzeige konvertiert
        $deadline = new DateTime($open_task->deadline);
        $converted_deadline = $deadline->format('d.m.Y H:i:s');

        //Die Aufgaben-ID der Aufgabe wird ermittelt
        $task_id = $open_task->task_id;

        //Sofern die Aufgabe noch offen ist und diese nicht aus dem aktuell offenen Protokoll stammt...
        if ($open_task->done == 0 && $open_task->protocol_id != $protocol_id) {

            //Das Nutzer-Objekt des zugewiesenen Nutzers wird geladen
            $user = user_load($open_task->uid);


            if ($open_task->done) {
                $done = "<font color=\"green\">erledigt</font>";
            } else {
                $done = "<font color=\"red\">offen</font></td><td><a href='" . $base_url . "/delete_task/$task_id/pXs8hYVUqe'>erledigt</a></td></tr></tr>";
            }

            //Der Aufgabe wird in eine neue Zeile der Tabelle geschrieben
            $task_table_string = $task_table_string . "
                <tr>
                    <td>$open_task->short_description</td>
                    <td>" . $user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'] . "</td>
                    <td>$open_task->task</td><td>$converted_deadline</td>
                    <td>" . $done .
                "</tr>";
        }
    }

    //Die Tabelle wird geschlossen
    $task_table_string .= "</table>";

    return $task_table_string;
}

/*
 * Diese Funktion leitet den Benutzer weiter zur Methodenübersicht
 */
function protocol_template_open_task_submit_method_overview($form, &$form_state)
{
    drupal_goto('/protokoll_methoden_uebersicht');
}

/*
 * Diese Funktion leitet den Benutzer zurück zum vorigen Schritt des Protokoll-Templates
 */
function protocol_template_open_task_back_to_general_information($form, &$form_state)
{
    drupal_goto('/protokoll_vorbereitung');
}

