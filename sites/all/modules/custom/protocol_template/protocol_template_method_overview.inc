<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.10.16
 * Time: 15:55
 */

/*
 * Diese Funktion erstellt eine Übersicht über die bisher durchgeführten Methoden innerhalb des Protokolls
 */
function protocol_template_get_method_overview_form($form, &$form_submit)
{

    //Es wird überprüft, ob schon ein offenes Protokoll existiert
    $reminder = protocol_template_check_current_open_protocol();

    //Sollte ein offenes Protokoll existieren...
    if ($reminder["exist"] == 1) {

        //Die Tabelle mit der Methodenübersicht wird erstellt
        $method_table = protocol_template_methods_show_methods_table();

        $form['method_overview'] = array(
            '#title' => "Übersicht bearbeiteter Methoden",
            '#type' => 'fieldset',
            '#description' => "In diesem Schritt werden alle im Rahmen dieses Treffens bearbeitete Methoden aufgelistet."
        );

        $form['method_overview']['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $method_table,
        );

        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('protocol_template_methods_back_to_open_tasks')
        );

        $form['new_method'] = array(
            '#type' => 'submit',
            '#value' => 'neue Methode durchführen',
            '#submit' => array('protocol_template_methods_new_method_execution')
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Weiter',
            '#submit' => array('protocol_template_methods_new_tasks')
        );

    } else {
        drupal_set_message('Es existiert noch kein offenes Protokoll. Bitte
        <a href="protokoll_erstellen">hier ein neues Protokoll erstellen.</a>
        ', "error");
    }


    return $form;
}

/*
 * Diese Funktion erstellt die HTML-Tabelle, in der alle bisher durchgeführten Methoden vorhanden sind.
 */
function protocol_template_methods_show_methods_table()
{
    //Die Methoden, die innerhalb des Protokolls durchgeführt wurden, werden geladen
    $method_array = protocol_template_get_methods_from_current_protocol();

    //Der Kopf der Tabelle wird erstellt
    $method_table_string = "
        <table>
            <th>Methodenname</th>
            <th> Bewertung</th>
            <th> Aktion</th>";

    //Es wird über jede Methode iteriert
    foreach ($method_array as $method) {

        //Die ID wird ausgelesen
        $method_id = $method->method_execution_id;
        $image = "";

        //Die Darstellung der Methodenbewertung wird ermittelt
        switch ($method->rating) {
            case 20:
                $image = "one.png";
                break;
            case 40:
                $image = "two.png";
                break;
            case 60:
                $image = "three.png";
                break;
            case 80:
                $image = "four.png";
                break;
            case 100:
                $image = "five.png";
                break;
            default:
                break;
        }

        global $base_url;

        //Jede Methode wird in einer neuen Zeile angezeigt
        $method_table_string = $method_table_string . "
            <tr width=\"10%\"><td><a href='$base_url/methodenanzeige/$method_id/pXs8hYVUqe'>$method->name</a></td>
                <td style=\" text-align:center\"><img src=" . $image . " height=\"120\" width=\"120\"> </td>
                <td><a href='$base_url/protokoll_methode_aendern/$method_id/pXs8hYVUqe'>bearbeiten</a></td>
            </tr>";
    }

    //Die Tabelle wird geschlossen
    $method_table_string .= "</table>";

    return $method_table_string;
}

/*
 * Diese Funktion lädt alle durchgeführten Methoden der Gruppe innerhalb des aktuellen Protokolls aus der Datenbank
 */
function protocol_template_get_methods_from_current_protocol()
{
    //Die Gruppenmitglieder werden geladen
    $users = protocol_template_get_participants_from_logged_in_user_group();

    //Die ID des aktuellen Protokolls wird ermittelt
    $protcol_id = protocol_template_get_current_open_group_protocol($users);

    //Die Methoden werden aus der Datenbank geladen
    $sql = "SELECT * FROM {method_execution} as m WHERE m.protocol_id = $protcol_id";
    $result = db_query($sql);

    return $result;
}

/*
 * Diese Funktion führt den Nutzer zurück zur Aufgabenübersicht
 */
function protocol_template_methods_back_to_open_tasks()
{
    drupal_goto('/protokoll_offene_aufgaben');
}

/*
 * Diese Funktion leitet den Nutzer weiter zum Formular, in das eine neue Methodendurchführung eingetragen werden kann
 */
function protocol_template_methods_new_method_execution()
{
    $participants = protocol_template_get_participants_from_logged_in_user_group();
    $protocol_id = protocol_template_get_current_open_group_protocol($participants);
    drupal_goto('/protokoll_neue_Methodendurchführung/'.$protocol_id);
}

/*
 * Diese Funktion leitet den Benutzer weiter zur Erstellung von neuen Aufgaben
 */
function protocol_template_methods_new_tasks()
{
    drupal_goto('/protokoll_neue_aufgaben');
}