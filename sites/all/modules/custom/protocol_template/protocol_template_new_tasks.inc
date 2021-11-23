<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:13
 */

/*
 * Diese Funktion erstellt ein Formular, in das die neu angelegten Aufgaben innerhalb eines Protokolls angezeigt werden.
 */
function protocol_template_get_new_task_form($form, &$form_submit)
{

    //Es wird geprüft, ob bereits ein offenes Protokoll existiert
    $open_protocol = protocol_template_check_current_open_protocol();

    //Sollte bereits ein offenes Protokoll existieren, wird die Tabelle angezeigt
    if ($open_protocol["exist"] == 1) {

        //die Tabelle mit allen bisher angelegten Aufgaben innerhalb des Protokolls wird erstellt
        $task_table = protocol_template_get_new_task_table();

        $form['tasks'] = array(
            '#title' => "Übersicht über neue Aufgaben",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt werden alle im Rahmen dieses Treffens erstellten Aufgaben aufgelistet."
        );

        $form['tasks']['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $task_table,
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('protocol_template_back_to_method_overview')
        );

        $form['new_task'] = array(
            '#type' => 'submit',
            '#value' => 'neue Aufgabe anlegen',
            '#submit' => array('protocol_template_new_task_execution')
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('protocol_template_submit_method_overview')
        );

        //Sollte kein offenes Protokoll exisitieren, wird darauf hingewiesen, zunächst ein neues anzulegen
    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
            <a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
            ', "error");
    }

    return $form;

}

/*
 * Diese Funktion baut die Übersicht über neu angelegte Aufgaben innerhalb des offenen Protokolls
 */
function protocol_template_get_new_task_table()
{

    global $base_url;

    //Die Gruppenmitglieder des aktuell angemeldeten Nutzers werden geladen
    global $user;
    $user_list = protocol_template_get_participants_from_logged_in_user_group();

    //Das betreffende Protokoll, welches offen ist, wird geladen
    $sql = "SELECT * FROM {protocol} AS pro WHERE pro.uid IN ($user_list) AND pro.finished = 0";
    $protocol_result = db_query($sql);

    //Die Protokoll-ID des Protokolls wird ermittelt
    $protocol_id = 0;
    foreach ($protocol_result as $protocol) {
        $protocol_id = $protocol->protocol_id;
    }

    //Die Aufgaben der Gruppe aus diesem Protokoll werden geladen
    $sql = "SELECT * FROM {task} AS t WHERE t.protocol_id = :pro_id ORDER BY t.deadline";
    $new_task_result = db_query($sql, array(
        ':pro_id' => $protocol_id,
    ));

    //Der Kopf der Tabelle wird erstellt
    $new_task_string = "
    <table>
        <th>Kurzbezeichnung</th>
        <th>zugehöriger Benutzer (Wer?)</th>
        <th> Aufgabe (Was?)</th>
        <th>Deadline (Wann?)</th>
        <th>Aktionen</th>";

    //Es wird über jede Aufgabe iteriert
    foreach ($new_task_result as $new_task) {

        //sofern die Aufgabe offen ist...
        if ($new_task->done == 0) {

            //Der zugewiesene Benutzer wird geladen
            $user = user_load($new_task->uid);

            //Das Fälligkeitsdatum wird ermtittelt und konvertiert
            $deadline = new DateTime($new_task->deadline);
            $converted_deadline = $deadline->format('d.m.Y H:i:s');

            $new_task_string = $new_task_string . "
            <tr>
                <td>$new_task->short_description</td>
                <td>" . $user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'] . "</td>
                <td>$new_task->task</td><td>$converted_deadline</td>
                <td><a href='" . $base_url . "/delete_task/$new_task->task_id/t8HVhPRlPZ'>löschen</a></td>
            </tr>";
        }
    }

    //Die Tabelle wird geschlossen
    $new_task_string .= "</table>";


    return $new_task_string;
}

/*
 * Diese Funktion leitet den Benutzer zur Erstellung einer neuen Aufgabe weiter
 */
function protocol_template_new_task_execution($form, &$form_state)
{
    drupal_goto('/protokoll_neue_Aufgabendurchführung');
}

/*
 * Diese Funktion leitet den Benutzer weiter im Protokoll-Template zum nächsten Schritt
 */
function protocol_template_submit_method_overview($form, &$form_state)
{
    drupal_goto('/protokoll_beschreibungen');
}

/*
 * Diese Funktion leitet den Benutzer zur Methodenübersicht weiter
 */
function protocol_template_back_to_method_overview($form, &$form_state)
{
    drupal_goto('protokoll_methoden_uebersicht');
}
