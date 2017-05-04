<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.04.17
 * Time: 17:14
 */

/*
 * Diese Funktion baut die Ansicht des Protokolls, die für allgemeine Informationen zum Inhalt des Protokolls dient.
 * Der gebaute HTML-Code wird am Ende zurückgegeben
 */
function protocol_overview_pdf_content_overview($protocol, $methods) {

    //In diese Variable wird der HTML-Code eingespeichert
    $content_string = "";

    //Jede übergebene Methode wird für die Anzeige untereinander geschrieben
    foreach ($methods as $m) {
        $content_string .= $m->name . "<br>";
    }

    //Hier wird der HTMl-Code gebaut. Es wird eine Tabelle in einen div-Container gebaut, in der alle Informationen
    //eingespeichert werden
    $content_string = '
        <div>
            <table border="1" width="100%">
                <tr>
                    <th font color="#fff" bgcolor="#444444" align="left">Bezeichnung</th>
                    <th font color="#fff" bgcolor="#444444" align="left">Information</th>
                </tr>
                <tr>
                    <td width="15%"><b>Inhalt</b></td>
                    <td>' . $protocol->preparation . '</td>
                </tr>
                <tr>
                    <td width="15%"><b>angewandte<br>Methoden</b></td>
                    <td>' . $content_string . '</td>
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

    //Der fertige HTML-Code wird zurückgegeben
    return $content_string;
}