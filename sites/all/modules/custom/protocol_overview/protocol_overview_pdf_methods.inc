<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.04.17
 * Time: 17:12
 */

/*
 * Diese Funktion baut für jede Methode eine Detail-Übersicht. Dafür werden die entsprechenden Methioden als Array
 * übergeben
 */
function protocol_overview_pdf_method_overview($method_array)
{

    //Es wird ein neuer Div-Container erstellt
    $method_string = '
    <br>
    <div>';

    //Die Überschrift für die Methodenübersicht wird erstellt
    $method_string .= '<h3 style="text-align:center">Übersicht der durchgeführten Methoden</h3>';

    //Sollten keine Methoden durchgeführt worden sein, wird eine entsprechende Meldung ausgegeben
    if (!$method_array->rowCount()) {
        $method_string .= '
        <table border="1" width="100%">
            <tr>
                <td colspan="5" align="center">Es wurden keine Methoden durchgeführt</td>
            </tr>
        </table>';
    }

    //Es wird über jede Methode iteriert
    foreach ($method_array as $method) {

        global $base_url;
        $image = $base_url . "/";
        //Die Darstellung der Methodenbewertung wird ermittelt
        switch ($method->rating) {
            case 20:
                $image .= "one.png";
                break;
            case 40:
                $image .= "two.png";
                break;
            case 60:
                $image .= "three.png";
                break;
            case 80:
                $image .= "four.png";
                break;
            case 100:
                $image .= "five.png";
                break;
            default:
                break;

        }

        //Es wird für jede Methode eine Tabelle gebaut, die alle wichtigen Informationen enthält
        $method_string .= '
        <table border="1" width="100%">
            <tr>
                <th colspan="2" font color="#fff" bgcolor="#444444" align="center" style="text-align:center">' . $method->name . '</th>
            </tr>
            <tr>
                <td width="18%"><b>Methodenname</b></td>
                <td>' . $method->name . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Raum</b></td>
                <td>' . $method->room . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Phase</b></td>
                <td>' . $method->phase . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Ziel</b></td>
                <td>' . $method->goal . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Durchführung</b></td>
                <td>' . $method->execution . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Bewertung</b></td>
                <td>
                    <img src=' . $image . ' height=20>
                </td>
            </tr>
            <tr>
                <td width="18%"><b>Bewertung</b></td>
                <td>' . $method->evaluation . '</td>
            </tr>
            <tr>
                <td width="18%"><b>Bilder</b></td><td align="center">

        ';

        //Es werden all Dateien geladen, die zu der Methode hochgeladen wurden
        $method_string .= "</td>
            </tr>
        </table>".protocol_overview_get_images_for_pdf($method->method_execution_id);

        $method_string .= '

        <br>
        <br>';
    }

    //Der Div-Container wird geschlossen
    $method_string .= '
    </div>';

    return $method_string;
}