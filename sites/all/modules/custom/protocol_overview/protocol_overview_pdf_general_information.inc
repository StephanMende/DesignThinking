<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.04.17
 * Time: 16:51
 */

/*
 * Diese Funktion baut den Teil des Protokolls, in dem allgemeine Informationen wie beispielswesie Protokollnummer,
 * Teilnehmer und Datum verziechnet sind
 */
function protocol_overview_pdf_general_information($group_name, $date_of_protocol, $start_time, $end_time, $protocol, $protocol_nr)
{

    $general_string = '

        <style>
            table {
                border-collapse: collapse;
            }

            table, td, th {
                border: 1px solid black;
            }
        </style>

        <h2 style="text-align:center">Tagesbericht ' . $group_name . ' vom ' . $date_of_protocol . '</h2>

        <div>
            <table border="1" width="100%">
                <tr>
                    <th font color="#fff" bgcolor="#444444" align="left">Bezeichnung</th>
                    <th font color="#fff" bgcolor="#444444" align="left">Information</th>
                </tr>
                <tr>
                    <td width="25%">Protokoll Nr.</td>
                    <td>' . $protocol_nr . '</td>
                </tr>
                <tr>
                    <td>Teilnehmer</td>
                    <td>' . protocol_overview_convert_participants($protocol->participants) . '<br></td>
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
                    <td>' . $start_time . ' - ' . $end_time . '</td>
                </tr>
                <tr>
                    <td>Autor</td>
                    <td>' . protocol_overview_get_username($protocol->uid) . '</td>
                </tr>
            </table>
        </div>';

    return $general_string;
}
