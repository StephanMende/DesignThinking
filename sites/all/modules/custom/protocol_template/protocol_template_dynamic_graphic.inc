<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 03.05.17
 * Time: 17:43
 */

/*
 * Diese Funktion baut die Design-Thinking-Grafik mit den dynamischen Punkten. Die Protokolle dazu werden per
 * Parameter übergeben
 */
function protocol_template_generate_graphic_and_circles($protocols)
{

    $protocol_room_phase = array();

    //Für jedes Protokoll werden Raum und Phase nach Beendigung des Protokolls ausgelesen
    foreach ($protocols as $r) {
        array_push($protocol_room_phase, $r->raum_end . " " . $r->phase_end);
    }

    //Die Daten der Kreise und deren Verbindungen werden ermittelt
    $circle_information = protocol_template_create_dynamic_graphic_by_protocols($protocol_room_phase);

    $circles = "";
    $lines = "";

    //Es wird über alle Kreise iteriert
    for ($i = 0; $i < sizeof($circle_information); $i++) {

        //das Kreis-Element wird ausgelesen
        $circles .= $circle_information[$i]['circle'];

        //Ab der zweiten Iteration, wird zwischen dem aktuellen und dem vorigen Kreis eine Verbindung gezogen
        if ($i > 0) {
            $lines .= '<line class="link" style="stroke: #000 stroke-width: 1.5px;" x1="' . $circle_information[$i - 1]['x'] . '" y1="' . $circle_information[$i - 1]['y'] . '" x2="' . $circle_information[$i]['x'] . '" y2="' . $circle_information[$i]['y'] . '"></line>';
        }
    }

    //Die Grafik wird erstellt
    $dynamic_graphic = '
        <style>

            .link {
            stroke: #000;
            }

        </style>

        <svg width="1030.5992" height="445">
            <g>
                <rect x="40" y="1" width="902" height="30" style="fill: rgb(89, 89, 89);"></rect>
                <text x="425" y="23" font-family="sans-serif" font-size="22px" fill="white">Vorbereitung</text>
            </g>
            <g>
                <rect x="40" y="32" width="300" height="30" style="fill: rgb(173, 145, 190);"></rect>
                <text x="125" y="53" font-family="sans-serif" font-size="22px" fill="white">Problemraum</text>
            </g>
            <g>
                <rect x="341" y="32" width="300" height="30" style="fill: rgb(122, 155, 185);"></rect>
                <text x="425" y="53" font-family="sans-serif" font-size="22px" fill="white">Lösungsraum</text>
            </g>
            <g>
                <rect x="642" y="32" width="300" height="30" style="fill: rgb(112, 196, 111);"></rect>
                <text x="675" y="53" font-family="sans-serif" font-size="22px" fill="white">Implementierungsraum</text>
            </g>
            <g>
                <rect x="40" y="63" width="149.5" height="30" style="fill: rgb(173, 145, 190);"></rect>
                <text dy="0em" x="64" y="83" font-family="sans-serif" font-size="15px" fill="white">Recherchieren</text>
            </g>
            <g>
                <rect x="190.5" y="63" width="149.5" height="30" style="fill: rgb(173, 145, 190);"></rect>
                <text dy="0em" x="236" y="76" font-family="sans-serif" font-size="13px" fill="white">Problem</text>
                <text dy="0em" x="233" y="88" font-family="sans-serif" font-size="13px" fill="white">definieren</text>
            </g>
            <g>
                <rect x="341" y="63" width="149.5" height="30" style="fill: rgb(122, 155, 185);"></rect>
                <text dy="0em" x="395" y="76" font-family="sans-serif" font-size="13px" fill="white">Ideen</text>
                <text dy="0em" x="382" y="88" font-family="sans-serif" font-size="13px" fill="white">generieren</text>
            </g>
            <g>
                <rect x="491.5" y="63" width="149.5" height="30" style="fill: rgb(122, 155, 185);"></rect>
                <text dy="0em" x="548" y="76" font-family="sans-serif" font-size="13px" fill="white">Ideen</text>
                <text dy="0em" x="533" y="88" font-family="sans-serif" font-size="13px" fill="white">ausarbeiten</text>
            </g>
            <g>
                <rect x="642" y="63" width="149.5" height="30" style="fill: rgb(112, 196, 111);"></rect>
                <text dy="0em" x="685" y="76" font-family="sans-serif" font-size="13px" fill="white">Prototypen</text>
                <text dy="0em" x="682" y="88" font-family="sans-serif" font-size="13px" fill="white">ausarbeiten</text>
            </g>
            <g>
                <rect x="792" y="63" width="150" height="30" style="fill: rgb(112, 196, 111);"></rect>
                <text dy="0em" x="840" y="76" font-family="sans-serif" font-size="13px" fill="white">Lösung</text>
                <text dy="0em" x="835" y="88" font-family="sans-serif" font-size="13px" fill="white">bewerten</text>
            </g>
            <g>
                <rect x="40" y="420" width="902" height="30" style="fill: rgb(89, 89, 89);"></rect>
                <text x="395" y="439" font-family="sans-serif" font-size="22px" fill="white">Projektmanagement</text>
            </g>

            <rect x="40" y="94" width="150" height="325" style="fill: rgb(173, 145, 190);"></rect>
            <rect x="190" y="94" width="150" height="325" style="fill: rgb(173, 145, 190);"></rect>
            <rect x="341" y="94" width="150" height="325" style="fill: rgb(122, 155, 185);"></rect>
            <rect x="491" y="94" width="150" height="325" style="fill: rgb(122, 155, 185);"></rect>
            <rect x="642" y="94" width="150" height="325" style="fill: rgb(112, 196, 111);"></rect>
            <rect x="792" y="94" width="150" height="325" style="fill: rgb(112, 196, 111);"></rect>

            <path d="M 190 94 L40 244 L190 394 z" style="fill: rgb(143, 86, 178);"></path>
            <path d="M 190 94 L340 244 L190 394 z" style="fill: rgb(120, 61, 153);"></path>

            <path d="M 491 94 L341 244 L491 394 z" style="fill: rgb(68, 111, 149);"></path>
            <path d="M 491 94 L641 244 L491 394 z" style="fill: rgb(51, 94, 135);"></path>

            <path d="M 792 94 L641 244 L792 394 z" style="fill: rgb(58, 160, 57);"></path>
            <path d="M 792 94 L941 244 L792 394 z" style="fill: rgb(50, 129, 49);"></path>
            ' . $circles . $lines . '
        </svg>';

    return $dynamic_graphic;
}

/*
 * Diese Funktion ermittelt die Daten, die für die Generierung der Design-Thinking-Grafik nötig sind. Dafür werden alle
 * Protokolle der Gruppe per Parameter übergeben
 */
function protocol_template_create_dynamic_graphic_by_protocols($protocol_room_phase)
{

    //Die Protokolle werden aus dem Übergabeparameter ausgelesen
    $protocols = $protocol_room_phase;

    //In diese Variable werden die dynamischen Punkte eingespeichert
    $dynamic_points = array();

    //In dieses Array werden in der Schleife jedes Mal der HTML-Code für einen Kreis und feste x und y Werte gespeichert
    $circle_array = array();

    //Hilfsvariable, die die Durchläufe durch die Protokolle zählt
    $counter = 0;

    //Diese Variable speichert den Verlauf der Methodendurchführungen in Form von Zahlen ab. 1 == Problemraum divergent,
    // 6 == Implementierungsraum konvergent
    $reference_array = array();

    //Feste x und y Werte für den ersten Punkt im divergenten Problemraum
    $y = 105;
    $x = 50;

    //Es wird über alle Protokolle iteriert
    for ($i = 0; $i < sizeof($protocols); $i++) {

        //Raum und Phase der Methodendurchführung wird ausgelesen
        $name = $protocols[$i];

        switch ($name) {
            case 'Problemraum divergent':

                //Sollte es der erste Punkt der Grafik sein:
                if ($counter == 0) {
                    $circle_array['circle'] = '<circle class="node" style="fill: green; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: green;"></circle>';
                    $circle_array['x'] = $x;
                    $circle_array['y'] = $y;
                    array_push($dynamic_points, $circle_array);

                    //Protokoll befindet sich in der ersten der sechs Prozessstufen
                    array_push($reference_array, "1");
                    $counter++;
                } else {

                    //Wenn sich die vorige Methode in gleichem Raum bzw. gleicher Phase befindet...
                    if ($reference_array[$counter - 1] <= 1) {
                        $x = $x + 30;
                        if ($x > 170) {
                            $x = 50;
                            $y = $y + 22;
                        }
                        $circle_array['circle'] = '<circle class="node" style="fill: green; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: green;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der ersten der sechs Prozessstufen
                        array_push($reference_array, "1");
                        $counter++;

                        //Sollte sich das vorige Protokoll in einem späteren Prozessschritt befunden haben, wird eine neue
                        //Reihe eröffnet, also der y-Wert und x-Wert angepasst
                    } else {
                        $y = $y + 22;
                        $x = 50;
                        $circle_array['circle'] = '<circle class="node" style="fill: green; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: green;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der ersten der sechs Prozessstufen
                        array_push($reference_array, "1");
                        $counter++;
                    }
                }
                break;

            case 'Problemraum konvergent':

                //Sollte es der erste Punkt der Grafik sein:
                if ($counter == 0) {
                    $x = 203;
                    $y = 105;
                    $circle_array['circle'] = '<circle class="node" style="fill: red; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: red;"></circle>';
                    $circle_array['x'] = $x;
                    $circle_array['y'] = $y;
                    array_push($dynamic_points, $circle_array);

                    //Protokoll befindet sich in der zweiten der sechs Prozessstufen
                    array_push($reference_array, "2");
                    $counter++;
                } else {

                    //Sollte sich die vorige methode in einem früheren Prozessschritt befinden, bleibt der y-Wert gleich
                    if ($reference_array[$counter - 1] < 2) {
                        $x = 203;
                        $circle_array['circle'] = '<circle class="node" style="fill: red; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: red;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der zweiten der sechs Prozessstufen
                        array_push($reference_array, "2");
                        $counter++;

                        //Sollte sich das vorige Protokoll in demselben Prozessschritt befinden, bleibt der y-Wert gleich, sofern
                        //sofern noch keine fünf Punkte in einer Reihe vorhanden sind.
                    } else if ($reference_array[$counter - 1] == 2) {
                        $x = $x + 30;
                        if ($x > 323) {
                            $x = 203;
                            $y = $y + 22;
                        }
                        $circle_array['circle'] = '<circle class="node" style="fill: red; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: red;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der zweiten der sechs Prozessstufen
                        array_push($reference_array, "2");
                        $counter++;

                        //Sollte sich das vorige Protokoll in einem späteren Prozessschritt befunden haben, wird eine neue
                        //Reihe eröffnet, also der y-Wert und x-Wert angepasst
                    } else if ($reference_array[$counter - 1] > 2) {
                        $x = 203;
                        $y = $y + 22;
                        $circle_array['circle'] = '<circle class="node" style="fill: red; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: red;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der zweiten der sechs Prozessstufen
                        array_push($reference_array, "2");
                        $counter++;
                    }
                }
                break;

            case 'Lösungsraum divergent':

                //Sollte es der erste Punkt der Grafik sein:
                if ($counter == 0) {
                    $x = 355;
                    $y = 105;
                    $circle_array['circle'] = '<circle class="node" style="fill: blue; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: blue;"></circle>';
                    $circle_array['x'] = $x;
                    $circle_array['y'] = $y;
                    array_push($dynamic_points, $circle_array);

                    //Protokoll befindet sich in der dritten der sechs Prozessstufen
                    array_push($reference_array, "3");
                    $counter++;
                } else {

                    //Sollte sich die vorige methode in einem früheren Prozessschritt befinden, bleibt der y-Wert gleich
                    if ($reference_array[$counter - 1] < 3) {
                        $x = 355;
                        $circle_array['circle'] = '<circle class="node" style="fill: blue; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: blue;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der dritten der sechs Prozessstufen
                        array_push($reference_array, "3");
                        $counter++;

                        //Sollte sich das vorige Protokoll in demselben Prozessschritt befinden, bleibt der y-Wert gleich, sofern
                        //sofern noch keine fünf Punkte in einer Reihe vorhanden sind.
                    } else if ($reference_array[$counter - 1] == 3) {
                        $x = $x + 30;
                        if ($x > 475) {
                            $x = 355;
                            $y = $y + 22;
                        }
                        $circle_array['circle'] = '<circle class="node" style="fill: blue; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: blue;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der dritten der sechs Prozessstufen
                        array_push($reference_array, "3");
                        $counter++;

                        //Sollte sich das vorige Protokoll in einem späteren Prozessschritt befunden haben, wird eine neue
                        //Reihe eröffnet, also der y-Wert und x-Wert angepasst
                    } else if ($reference_array[$counter - 1] > 3) {
                        $x = 355;
                        $y = $y + 22;
                        $circle_array['circle'] = '<circle class="node" style="fill: blue; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: blue;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der dritten der sechs Prozessstufen
                        array_push($reference_array, "3");
                        $counter++;
                    }
                }
                break;

            case "Lösungsraum konvergent":

                //Sollte es der erste Punkt der Grafik sein:
                if ($counter == 0) {
                    $x = 505;
                    $y = 105;
                    $circle_array['circle'] = '<circle class="node" style="fill: yellow; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: yellow;"></circle>';
                    $circle_array['x'] = $x;
                    $circle_array['y'] = $y;
                    array_push($dynamic_points, $circle_array);

                    //Protokoll befindet sich in der vierten der sechs Prozessstufen
                    array_push($reference_array, "4");
                    $counter++;
                } else {

                    //Sollte sich die vorige methode in einem früheren Prozessschritt befinden, bleibt der y-Wert gleich
                    if ($reference_array[$counter - 1] < 4) {
                        $x = 505;
                        $circle_array['circle'] = '<circle class="node" style="fill: yellow; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: yellow;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der vierten der sechs Prozessstufen
                        array_push($reference_array, "4");
                        $counter++;

                        //Sollte sich das vorige Protokoll in demselben Prozessschritt befinden, bleibt der y-Wert gleich, sofern
                        //sofern noch keine fünf Punkte in einer Reihe vorhanden sind.
                    } else if ($reference_array[$counter - 1] == 4) {
                        $x = $x + 30;

                        if ($x > 654) {
                            $x = 505;
                            $y = $y + 22;
                        }
                        $circle_array['circle'] = '<circle class="node" style="fill: yellow; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: yellow;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der vierten der sechs Prozessstufen
                        array_push($reference_array, "4");
                        $counter++;

                        //Sollte sich das vorige Protokoll in einem späteren Prozessschritt befunden haben, wird eine neue
                        //Reihe eröffnet, also der y-Wert und x-Wert angepasst
                    } else if ($reference_array[$counter - 1] > 4) {
                        $x = 505;
                        $y = $y + 22;
                        $circle_array['circle'] = '<circle class="node" style="fill: yellow; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: yellow;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der vierten der sechs Prozessstufen
                        array_push($reference_array, "4");
                        $counter++;
                    }
                }
                break;

            case "Implementierungsraum divergent":

                //Sollte es der erste Punkt der Grafik sein:
                if ($counter == 0) {
                    $x = 655;
                    $y = 105;
                    $circle_array['circle'] = '<circle class="node" style="fill: black; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: black;"></circle>';
                    $circle_array['x'] = $x;
                    $circle_array['y'] = $y;
                    array_push($dynamic_points, $circle_array);

                    //Protokoll befindet sich in der fünften der sechs Prozessstufen
                    array_push($reference_array, "5");
                    $counter++;
                } else {

                    //Sollte sich die vorige methode in einem früheren Prozessschritt befinden, bleibt der y-Wert gleich
                    if ($reference_array[$counter - 1] < 5) {
                        $x = 655;
                        $circle_array['circle'] = '<circle class="node" style="fill: black; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: black;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der fünften der sechs Prozessstufen
                        array_push($reference_array, "5");
                        $counter++;

                        //Sollte sich das vorige Protokoll in demselben Prozessschritt befinden, bleibt der y-Wert gleich, sofern
                        //sofern noch keine fünf Punkte in einer Reihe vorhanden sind.
                    } else if ($reference_array[$counter - 1] == 5) {
                        $x = $x + 30;
                        if ($x > 775) {
                            $x = 655;
                            $y = $y + 22;
                        }
                        $circle_array['circle'] = '<circle class="node" style="fill: black; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: black;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der fünften der sechs Prozessstufen
                        array_push($reference_array, "5");
                        $counter++;

                        //Sollte sich das vorige Protokoll in einem späteren Prozessschritt befunden haben, wird eine neue
                        //Reihe eröffnet, also der y-Wert und x-Wert angepasst
                    } else if ($reference_array[$counter - 1] > 5) {
                        $x = 655;
                        $y = $y + 22;
                        $circle_array['circle'] = '<circle class="node" style="fill: black; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: black;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der fünften der sechs Prozessstufen
                        array_push($reference_array, "5");
                        $counter++;
                    }
                }
                break;

            case "Implementierungsraum konvergent":

                //Sollte es der erste Punkt der Grafik sein:
                if ($counter == 0) {
                    $x = 806;
                    $y = 105;
                    $circle_array['circle'] = '<circle class="node" style="fill: brown; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: brown;"></circle>';
                    $circle_array['x'] = $x;
                    $circle_array['y'] = $y;
                    array_push($dynamic_points, $circle_array);

                    //Protokoll befindet sich in der sechsten der sechs Prozessstufen
                    array_push($reference_array, "6");
                    $counter++;

                } else {

                    //Sollte sich die vorige methode in einem früheren Prozessschritt befinden, bleibt der y-Wert gleich
                    if ($reference_array[$counter - 1] < 6) {
                        $x = 806;
                        $circle_array['circle'] = '<circle class="node" style="fill: brown; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: brown;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der sechsten der sechs Prozessstufen
                        array_push($reference_array, "6");
                        $counter++;

                        //Sollte sich das vorige Protokoll in demselben Prozessschritt befinden, bleibt der y-Wert gleich, sofern
                        //sofern noch keine fünf Punkte in einer Reihe vorhanden sind.
                    } else if ($reference_array[$counter - 1] == 6) {
                        $x = $x + 30;
                        if ($x > 926) {
                            $x = 806;
                            $y = $y + 22;
                        }
                        $circle_array['circle'] = '<circle class="node" style="fill: brown; stroke: #000; stroke-width: 1.5px;" r="5" cx="' . $x . '" cy="' . $y . '" style="fill: brown;"></circle>';
                        $circle_array['x'] = $x;
                        $circle_array['y'] = $y;
                        array_push($dynamic_points, $circle_array);

                        //Protokoll befindet sich in der sechsten der sechs Prozessstufen
                        array_push($reference_array, "6");
                        $counter++;
                    }
                }
                break;
        }
    }

    return $dynamic_points;
}