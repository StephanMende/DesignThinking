<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.05.17
 * Time: 18:00
 */

/*
 * Diese Funktion lädt alle Termine einer Gruppe aus der Datenbank und zeigt diese in Form einer Tabelle an
 */
function task_reminder_get_appointment_content()
{
    //Die Überschrift der Tabelle wird erstellt
    $tempString = "<h2>alle Termine der Gruppe</h2>";

    //Der Kopf der Tabelle wird erstellt
    $tempString .= "<table><th>Kurzbezeichnung</th><th>Terminbeschreibung</th> <th>Datum</th><th>Aktion</th>";

    //Die Gruppenmitglieder der Gruppe werden ermittelt
    $users = task_reminder_get_participants_from_logged_in_user_group();

    //Die Termine der Gruppe werden ermittelt
    $sql = "SELECT * FROM {appointment} WHERE uid IN (" . $users . ") ORDER BY date";
    $appointment_result = db_query($sql);

    global $base_url;

    //Es wird über alle Termine iteriert
    foreach ($appointment_result as $appointment) {

        //Für jeden Termin wird eine neue Zeile innerhalb der Tabelle erstellt
        $tempString = $tempString . "
            <tr>
                <td>$appointment->short_description</td>
                <td>$appointment->appointment</td>
                <td>$appointment->date</td>
                <td><a href='" . $base_url . "/delete_appointment/" . $appointment->appointment_id . "'>löschen</a></td>
            </tr>";
    }

    //Die Tabelle wird geschlossen
    $tempString .= "</table>";

    return $tempString;
}

/*
 * Diese Funktion löscht einen Termin aus der Datenbank
 */
function task_reminder_delete_appointment()
{

    //Die ID des Termins wird aus der URL geladen
    $appointment_id = arg(1);

    //Der betreffende Termin wird aus der Datenbank geladen
    $sql = "SELECT * FROM {appointment} WHERE appointment_id = " . $appointment_id;
    $appointment_result = db_query($sql);

    //Für den gefundenen Termin wird zunächst der entsprechende Node geöscht, damit der Termin nicht mehr im
    //Kalender angezeigt wird
    foreach ($appointment_result as $appointment) {
        $sql = "SELECT * FROM {node} WHERE type = 'event' AND title = '" . $appointment->short_description . "'";
        $appointment_node_result = db_query($sql);

        //der Node wird gelöscht
        foreach ($appointment_node_result as $appointment_node) {
            node_delete($appointment_node->nid);
        }
    }

    //Anschließend wird der Termin aus der dafür vorgesehenen Datenbank-Tabelle entfertn
    $sql = "DELETE FROM {appointment} WHERE appointment_id = " . $appointment_id;
    db_query($sql);

    drupal_set_message("Termin wurde gelöscht");
    drupal_goto("gruppentermine");
}