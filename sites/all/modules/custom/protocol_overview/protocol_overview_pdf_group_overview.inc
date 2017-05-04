<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 29.04.17
 * Time: 15:57
 */

/*
 * Diese Funktion baut die Gruppenübersicht, die am Anfang der vollständigen Dokumentation angezeigt wird. Diese enthält
 * einerseits die Design-Thinking-Grafik mit den dynamischen Punkten und andererseits eine Tabelle, in der alle
 * durchgeführten Methoden verzeichnet sind
 */
function protocol_overview_pdf_group_overview($role_id)
{

    //Zunächst werden die Gruppenmitglieder ermittelt
    $users = array();
    $sql = "SELECT * FROM {users_roles} WHERE rid = ".$role_id;
    $user_roles_result = db_query($sql);

    foreach ($user_roles_result as $user_role) {
        array_push($users, $user_role->uid);
    }

    $participants = implode(',', $users);

    //Die Methodendurchführungen der Gruppe werden ermittelt
    $sql = "SELECT * FROM {method_execution} WHERE uid in (" . $participants . ") ORDER BY creation_date";
    $method_execution_result = db_query($sql);

    //Die Überschrift der Seite wird erstellt
    $group_overview_string = '<h2 style="text-align:center">Gruppenübersicht</h2>';

    //Die Design-Thinking-Grafik wird erstellt
    $group_overview_string .= protocol_overview_generate_graphic($method_execution_result);

    //Der Kopf der Tabelle wird erstellt
    $group_overview_string .= '
        <br>
        <br>
        <table border="1" width="100%" align="center">
            <tr>
                <th font color="#fff" bgcolor="#444444" align="left">Nr.</th>
                <th font color="#fff" bgcolor="#444444" align="left">Datum</th>
                <th font color="#fff" bgcolor="#444444" align="left">Methodenname</th>
                <th font color="#fff" bgcolor="#444444" align="left">Bewertung</th>
            </tr>';

    //Die Methoden werden für die Weiterverarbeitung erneut geladen
    $method_execution_result = db_query($sql);

    //Um die Anzahl der gezeigten Protokolle korrekt darzustellen, wird eine Hilfsvariable genutzt
    $number_of_protocol = 1;

    //Über jede Methode wird iteriert
    foreach ($method_execution_result as $method_execution) {
        $image = "";

        //Die Darstellung der Methodenbewertung wird ermittelt
        switch ($method_execution->rating) {
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

        $creation_date = new DateTime($method_execution->creation_date);
        $converted_creation_date = $creation_date->format('d.m.Y');

        //Jede Methodendurchführung wird in eine neue Zeile der Tabelle geschrieben
        $group_overview_string .= '
            <tr>
                <td>' . $number_of_protocol . '</td>
                <td>' . $converted_creation_date . '</td>
                <td>' . $method_execution->name . '</td>
                <td>
                    <img src="' . $image . '" height="20" width="100">
                </td>
            </tr>';

        $number_of_protocol++;
    }

    //Die Tabelle wird geschlossen
    $group_overview_string .= '</table>';

    return $group_overview_string;
}