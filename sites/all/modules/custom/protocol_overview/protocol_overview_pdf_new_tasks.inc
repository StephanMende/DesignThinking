<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.04.17
 * Time: 17:13
 */

/*
 * Diese Funktion erzeugt eine Tabelle, in der alle Aufgaben aufgelistet werden, die innerhalb des Meetings für das
 * Protokoll angelegt wurden. Hierfür wird die Protokoll-ID als Parameter übergeben
 */
function protocol_overview_pdf_new_task_string($protocol)
{

    //Die Aufgaben werden ermittelt
    $new_tasks = protocol_overview_get_relevant_new_tasks($protocol);

    //Die Tabelle wird erstellt
    $new_task_string = '
    <div>
    <br>
    <h3 style="text-align:center">neue Aufgabenzuweisungen</h3>
        <table border="1" width="100%">
            <tr>
                <th font color="#fff" bgcolor="#444444" align="left">Nr.</th>
                <th font color="#fff" bgcolor="#444444" align="left">Kurzbezeichnung</th>
                <th font color="#fff" bgcolor="#444444" align="left">Teilnehmer</th>
                <th font color="#fff" bgcolor="#444444" align="left">Aufgabe</th>
                <th font color="#fff" bgcolor="#444444" align="left">Deadline</th>
            </tr>';

    //Um die Anzahl der Aufgaben korrekt darzustellen, wird diese Hilfsvariable benötigt
    $task_number = 1;

    //Sollten keine Aufgaben vorhanden sein, wird eine entsprechende Meldung ausgegeben
    if (!$new_tasks->rowCount()) {


        $new_task_string .= '
        <tr>
            <td colspan="5" align="center">keine neuen Aufgaben vorhanden</td>
        </tr>';

    } else {

        //Es wird über alle Aufgaben iteriert und jeweils eine neue Zeile angelegt
        foreach ($new_tasks as $t) {

            $start_date = new DateTime($t->deadline);
            $converted_start_date_for_table = $start_date->format('d.m.Y H:i:s');

            $new_task_string .= '
            <tr>
                <td width="5%">' . $task_number . '</td>
                <td>' . $t->short_description . '</td>
                <td width="15%">' . protocol_overview_get_username($t->uid) . '</td>
                <td>' . $t->task . '</td>
                <td width="25%">' . $converted_start_date_for_table . '</td>
            </tr>';

            $task_number = $task_number + 1;
        }
    }

    //Die Tabelle und der Div-Container wird geschlossen
    $new_task_string .= '</table></div>';

    return $new_task_string;
}