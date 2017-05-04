<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 18.11.16
 * Time: 12:56
 */

function test_module_show_protocol_form($form, &$form_submit) {


    $proof = false;

      global $user;
    if (in_array('administrator', $user->roles)) {
        $proof = true;
    }



    $protocol_id =  arg(1);
    $protocol = "";

    $sql = "SELECT * FROM {protocol} WHERE protocol_id = $protocol_id";
    $result = db_query($sql);

    foreach($result as $r) {
        $protocol = $r;
    }

    $users = getParticipants2();
    $list = explode(',', $users);

    if (in_array($protocol->uid, $list) || $proof) {
        $date = new DateTime($protocol->date);
        $deadline2 = $date->format('d.m.Y');

        //style="margin-top:0;margin-left:40px;margin-right:40px"

        $general_information = '
<html>
    <style>
        table {
            border-collapse: collapse;
        }

        table, td, th {
            border: 2px solid black;
        }
    </style>
    <body>
        <br><h1 style="text-align:center">Tagesbericht Design-Thinking Gruppe vom '.$deadline2.'</h1><br>
';

        $date = new DateTime($protocol->start_date);
        $date_end = new DateTime($protocol->end_date);
        $deadline2 = $date->format('d.m.Y H:i:s');
        $deadline_end = $date_end->format('d.m.Y H:i:s');

        $arra = array();

        $users = getParticipants2();
        $sql = "SELECT * FROM {protocol} WHERE uid IN ($users) ORDER BY protocol_id";
        $result = db_query($sql);

        foreach ( $result as $item) {
            array_push($arra, $item->protocol_id);
        }

        $protocol_nr = array_search($protocol->protocol_id, $arra);
        $protocol_nr++;


        $generel_information_string = '        <div>
            <table border="1" width="100%">
            <tr>
            <th>Bezeichnung</th>
            <th>Information</th>
        </tr>
                <tr>
                    <td width="25%">Protokoll Nr.</td>
                    <td>'.$protocol_nr.'</td>
                </tr>
                <tr>
                    <td>Teilnehmer</td>
                    <td>'.parse_participants90($protocol->participants).'<br></td>
                </tr>
                <tr>
                    <td>Ort</td>
                    <td>'.$protocol->place.'</td>
                </tr>
                <tr>
                    <td>Art</td>
                    <td>'.$protocol->type.'</td>
                </tr>
                <tr>
                    <td>Datum</td>
                    <td>'.$deadline2.' Uhr - '.$deadline_end.' Uhr</td>
                </tr>
                <tr>
                    <td>Autor</td>
                    <td>'.get_username90($protocol->uid).'</td>
                </tr>
            </table>
        </div>';

        $open_tasks = get_relevant_open_tasks90($protocol);
        $number = 1;

        /*
        $open_task_string = '
    <div>
        <h3>zurzeit laufende Aufgaben vor dem Meeting</h3>
            <table border="1" width="100%">
            <tr>
                <th>Nr.</th>
                <th>Kurzbezeichnung</th>
                <th>Teilnehmer</th>
                <th>Aufgabe</th>
                <th>Deadline</th>
            </tr>';

        foreach($open_tasks as $t) {
            $date = new DateTime($t->deadline);
            $deadline2 = $date->format('d.m.Y H:i:s');
            $open_task_string .= '
            <tr>
                <td width="5%">'.$number.'</td>
                <td>'.$t->short_description.'</td>
                <td width="15%">'.get_username90($t->uid).'</td>
                <td>'.$t->task.'</td>
                <td width="25%">'.$deadline2.'</td>
            </tr>';
            $number = $number + 1;
        }
        $open_task_string .= '</table></div>';*/

        $new_tasks = get_relevant_new_tasks90($protocol);

        $new_task_string = '
<div>
    <br><h2 style="text-align:center">neue Aufgaben</h2>
        <table border="1" width="100%">
            <tr>
                <th>Nr.</th>
                <th>Kurzbezeichnung</th>
                <th>Teilnehmer</th>
                <th>Aufgabe</th>
                <th>Deadline</th>
            </tr>';

        $number = 1;

        $stuff = $new_tasks->rowCount();

        if ($stuff == null) {
            $new_task_string .= '
        <tr>
            <td colspan="5" style="text-align:center">keine neuen Aufgaben vorhanden</td>
        </tr>';

        } else {
            foreach($new_tasks as $t) {
                $date = new DateTime($t->deadline);
                $deadline2 = $date->format('d.m.Y H:i:s');
                $new_task_string .= '
        <tr>
            <td width="5%">'.$number.'</td>
            <td>'.$t->short_description.'</td>
            <td width="15%">'.get_username90($t->uid).'</td>
            <td>'.$t->task.'</td>
            <td width="25%">'.$deadline2.'</td>
        </tr>';
                $number = $number + 1;
            }
        }


        $new_task_string .= '</table></div>';

        $end = '
    <br></body>
</html>';


        $methods = get_relevant_methods90($protocol->protocol_id);

        $newString = "";

        global $base_url;

        foreach ($methods as $m) {



            $newString .= $m->name . "<br>";
        }

        $content_string = '
<div>
    <table border="1" width="100%">
    <tr>
            <th>Bezeichnung</th>
            <th>Information</th>
        </tr>
        <tr>
            <td width="15%"><b>Inhalt</b></td>
            <td>'.$protocol->preparation.'</td>
        </tr>
        <tr>
            <td width="15%"><b>angewandte<br>Methoden</b></td>
            <td>'.$newString.'</td>
        </tr>
        <tr>
            <td width="15%"><b>Probleme</b></td>
            <td>'.$protocol->problems.'</td>
        </tr>
        <tr>
            <td width="15%"><b>Fazit</b></td>
            <td>'.$protocol->conclusion.'</td>
        </tr>
        <tr>
            <td width="15%"><b>Ausblick</b></td>
            <td>'.$protocol->outlook.'</td>
        </tr>
    </table>
</div>';

        $methods = get_relevant_methods90($protocol->protocol_id);
        $method_string = '
<div>';

        if($methods->rowCount() >0) {
            $method_string .= '<br><h2 style="text-align:center">Übersicht der durchgeführten Methoden</h2>';

        }
        foreach($methods as $m) {

            $image = $base_url;
            if($m->rating == 20) {
                $image .= "/one.png";
            } else if($m->rating == 40) {
                $image .= "/two.png";
            } else if($m->rating == 60) {
                $image .= "/three.png";
            } else if($m->rating == 80) {
                $image .= "/four.png";
            } else if($m->rating == 100) {
                $image .= "/five.png";
            }

            $method_string .= '
    <table border="1" width="100%">
    <tr>
                <th colspan="2" style="text-align:center">'.$m->name.'</th>
            </tr>
        <tr>
            <td width="18%"><b>Methodenname</b></td>
            <td>'.$m->name.'</td>
        </tr>
        <tr>
            <td width="18%"><b>Raum</b></td>
            <td>'.$m->room.'</td>
        </tr>
        <tr>
            <td width="18%"><b>Phase</b></td>
            <td>'.$m->phase.'</td>
        </tr>
        <tr>
            <td width="18%"><b>Ziel</b></td>
            <td>'.$m->goal.'</td>
        </tr>
        <tr>
            <td width="18%"><b>Durchführung</b></td>
            <td>'.$m->execution.'</td>
        </tr>
        <tr>
            <td width="18%"><b>Bewertung</b></td>
            <td><img src='.$image.' height=30px. width="150px"></td>
        </tr>
        <tr>
            <td width="18%"><b>Bewertung</b></td>
            <td>'.$m->evaluation.'</td>
        </tr>
        <tr>
            <td width="18%"><b>Bilder</b></td><td align="center">

        ';


            $path = explode(",",$m->path);



            $numItems = count($path);
            $i = 0;

            $method_string .= test_module_get_images_for_pdf($m->method_execution_id);

            /*
            foreach($path as $p) {

                if (strlen($p) > 0) {
                    $filepath='/design_thinking_tool/sites/default/files/'.$p.'';

                    $basename = "Abbildung x: " . preg_replace('/^.+[\\\\\\/]/', '', $filepath);

                    $method_string .= "<img src='$filepath' height='200' width='200' alt=Selfhtml><br><b>$basename</b><br>";
                    if(++$i != $numItems) {
                        $method_string .="<br>";
                    }

                }


            }*/

            global $base_url;


            $method_string .= '
    </td></tr></table>
    <a href="'.$base_url.'/protokoll_methode_aendern/'.$m->method_execution_id.'/r21dPaUZRy"">Bearbeiten</a>
    <br><br>';
        }

        $method_string .= '
</div>';

        $users = getParticipants2();
        $sql = "SELECT * FROM {protocol} WHERE uid IN ($users) AND protocol_id < ".$protocol_id." ORDER BY date";
        $result = db_query($sql);

        $sql2 = "SELECT * FROM {protocol} WHERE uid IN ($users) AND protocol_id <= ".$protocol_id." ORDER BY date";
        $result2 = db_query($sql2);

        $test = array();
        $test2 = array();

        foreach ($result as $r) {
            array_push($test, $r);
        }

        foreach ($result2 as $r2) {
            array_push($test2, $r2);
        }

        $position_before_meeting = '<br><h2 style="text-align:center">Phase im Design-Thinking-Prozess vor dem Meeting</h2><br><div>'.show_protocol_generate_graphic($test).'</div>';
        $position_after_meeting = '<h2 style="text-align:center">Phase im Design-Thinking-Prozess nach dem Meeting</h2><br><div>'.show_protocol_generate_graphic($test2).'</div>';


        /*
        $form['form_item'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $general_information .$generel_information_string. $position_before_meeting."<br>"  .$open_task_string. "<br><h3>Inhalt des Tagesberichts</h3>" .$content_string."<br>".$new_task_string . "<br>" . $method_string .$position_after_meeting."<br>" .  $end
        );*/

        $form['form_item2'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $general_information
        );



        $form['form_item3'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $generel_information_string
        );

        $form['change1'] = array(
            '#type' => 'submit',
            '#value' => 'Informationen bearbeiten',
            '#submit' => array('go_to_change_general_information')
        );


        $form['form_item4'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $position_before_meeting
        );
        $form['form_item5'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $open_task_string
        );

        $form['form_item6'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => '<br><h2 style="text-align:center">Inhalt des Tagesberichts</h2>' .$content_string
        );
        $form['change2'] = array(
            '#type' => 'submit',
            '#value' => 'Inhalt bearbeiten',
            '#submit' => array('go_to_change_preparation_information')
        );

        $form['form_item7'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $new_task_string
        );

        $form['form_item8'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $method_string
        );
        $form['new'] = array(
            '#type' => 'submit',
            '#value' => 'Neue Methode durchführen',
            '#submit' => array('test_module_change_protocol_new_method_execution')
        );


        $form['form_item9'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $position_after_meeting
        );

        $form['form_item10'] = array(
            '#type' => 'markup',
            '#title' => t('My Form Item'),
            '#prefix' => $end
        );

        $form['back'] = array(
            '#type' => 'submit',
            '#value' => 'Zurück',
            '#submit' => array('back_to_protocol_overview')
        );

        $form['delete'] = array(
            '#type' => 'submit',
            '#value' => 'Löschen',
            '#submit' => array('test_module_delete_protocol')
        );

        return $form;

    } else {
        global $base_url;
        drupal_set_message("Sie haben nicht die Berechtigung, dieses Dokument aufzurufen!<a href='".$base_url."'> Zurück zum Hauptmenü</a>" , "error");
    }

}

function test_module_change_protocol_new_method_execution() {
    $protocol_id =  arg(1);
    drupal_goto('/protokoll_neue_Methodendurchführung/'.$protocol_id);
}


function test_module_delete_protocol() {
    $protocol_id = arg(1);

    $sql = "DELETE FROM {protocol} WHERE protocol_id = ".$protocol_id;
    $sql2 = "DELETE FROM {method_execution} WHERE protocol_id = ".$protocol_id;

    db_query($sql);
    db_query($sql2);

    drupal_set_message("Protokoll wurde gelöscht");
    drupal_goto("protokoll_uebersicht");
}

function test_module_get_images_for_pdf($id) {

    $sql = "SELECT * FROM {method_execution_data} WHERE method_execution_id = ".$id;

    $result = db_query($sql);

    $temp = "";

    foreach($result as $r) {
        $allowed =  array('gif','png' ,'jpg');
        $filename = $r->path;
        $basename = preg_replace('/^.+[\\\\\\/]/', '', $filename);
        $ext = pathinfo($filename, PATHINFO_EXTENSION);
        if(in_array($ext,$allowed) ) {
            global $base_url;
            $path = $base_url."/sites/default/files/".$r->path;

            $temp .= "<img style=\"max-width:550px;\" src='".$path."'alt=Selfhtml><br><b>$basename</b><br><br>";
        }
    }

    return $temp;
}

function go_to_change_general_information() {
    $protocol_id =  arg(1);
    drupal_goto('/change_protocol_general_information/' . $protocol_id);
}

function go_to_change_preparation_information() {
    $protocol_id =  arg(1);
    drupal_goto('change_protocol_content_information/' . $protocol_id);
}

function back_to_protocol_overview() {
    drupal_goto('/protokoll_uebersicht');
}

function get_username90($uid) {
    $user=user_load($uid);
    $username=$user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'];
    return $username;
}

function parse_participants90($participants) {
    $users = explode(",",$participants);

    $numItems = count($users);
    $i = 0;

    $string = "";
    foreach($users as $u) {
        $u = get_username90($u);
        $string .= $u;
        if(++$i != $numItems) {
            $string .= "<br>";
        }
    }
    return $string;
}

function get_relevant_methods90($protocol_id) {
    $sql = "SELECT * FROM {method_execution} WHERE protocol_id = $protocol_id";
    $result = db_query($sql);

    return $result;
}


function get_relevant_open_tasks90($protocol) {
    $users = getParticipants2();
    $sql = "SELECT * FROM {task} WHERE uid IN ($users) AND protocol_id < $protocol->protocol_id AND done = 0";
    $result = db_query($sql);

    return $result;
}

function get_relevant_new_tasks90($protocol) {
    $users = getParticipants2();
    $sql = "SELECT * FROM {task} WHERE uid IN ($users) AND protocol_id = $protocol->protocol_id AND done = 0";
    $result = db_query($sql);

    return $result;
}

function show_protocol_generate_graphic($protocols) {

        $test = array();

        foreach ($protocols as $r) {
            array_push($test, $r->raum_end . " " . $r->phase_end);
        }

        $array = test_module_create_small_chart($test);

        $circles = "";
        $lines = "";


        for($i=0;$i<sizeof($array);$i++) {

            $circles .= $array[$i]['circle'];
            $x = "";
            $y = "";
            $x_old = "";
            if($i > 0) {
                $lines .= '<line class="link" style="stroke: #000 stroke-width: 1.5px;" x1="'.$array[$i-1]['x'].'" y1="'.$array[$i-1]['y'].'" x2="'.$array[$i]['x'].'" y2="'.$array[$i]['y'].'"></line>';
            }
        }

    $chart = '
<style>

.link {
  stroke: #000;

}


</style><svg width="1030.5992" height="445">
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
    '.$circles.$lines.'

    </svg>';

    return $chart;
}

function test_module_create_small_chart($result) {

    $test = array();
    $protocols = $result;
    foreach ($protocols as $protocol) {
        array_push($test, $protocol);
    }

    $stuff = array();
    $stuff_array = array();

    $counter = 0;
    $referenz_array = array();
    $y = 105;
    $x = 50;

    for ($i = 0; $i < sizeof($test); $i++) {
        $name = $test[$i];
        switch ($name) {
            case 'Problemraum divergent':
                if ($counter == 0) {
                    $stuff_array['circle'] = '<circle class="node" style="fill: green; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: green;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "1");
                    $counter++;
                } else {
                    if($referenz_array[$counter-1] <= 1) {
                        $x = $x+30;
                        if($x > 170) {
                            $x = 50;
                            $y = $y+22;
                        }
                        $stuff_array['circle'] = '<circle class="node" style="fill: green; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: green;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "1");
                        $counter++;
                    } else {
                        $y = $y+22;
                        $x = 50;
                        $stuff_array['circle'] = '<circle class="node" style="fill: green; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: green;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "1");
                        $counter++;
                    }
                }
                break;

            case 'Problemraum konvergent':
                if ($counter == 0) {
                    $x = 203;
                    $y = 105;
                    $stuff_array['circle'] = '<circle class="node" style="fill: red; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: red;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "2");
                    $counter++;
                } else {
                    if($referenz_array[$counter-1] <2) {
                        $x = 203;
                        $stuff_array['circle'] = '<circle class="node" style="fill: red; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: red;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "2");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 2) {
                        $x = $x+30;
                        if($x > 323) {
                            $x = 203;
                            $y = $y+22;
                        }
                        $stuff_array['circle'] = '<circle class="node" style="fill: red; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: red;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "2");
                        $counter++;
                    } else if ($referenz_array[$counter-1] >2) {
                        $x = 203;
                        $y = $y+22;
                        $stuff_array['circle'] = '<circle class="node" style="fill: red; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: red;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "2");
                        $counter++;
                    }
                }
                break;

            case 'Lösungsraum divergent':
                if ($counter == 0) {
                    $x = 355;
                    $y = 105;
                    $stuff_array['circle'] = '<circle class="node" style="fill: blue; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: blue;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "3");
                    $counter++;
                } else {
                    if($referenz_array[$counter-1] < 3) {
                        $x = 355;
                        $stuff_array['circle'] = '<circle class="node" style="fill: blue; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: blue;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "3");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 3) {
                        $x = $x+30;
                        if($x > 475) {
                            $x = 355;
                            $y = $y+22;
                        }
                        $stuff_array['circle'] = '<circle class="node" style="fill: blue; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: blue;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "3");
                        $counter++;
                    } else if ($referenz_array[$counter-1] >3) {
                        $x = 355;
                        $y = $y+22;
                        $stuff_array['circle'] = '<circle class="node" style="fill: blue; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: blue;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "3");
                        $counter++;
                    }
                }
                break;

            case "Lösungsraum konvergent":
                if ($counter == 0) {
                    $x = 505;
                    $y = 105;
                    $stuff_array['circle'] = '<circle class="node" style="fill: yellow; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: yellow;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "4");
                    $counter++;
                } else {
                    if($referenz_array[$counter-1] < 4) {
                        $x = 505;
                        $stuff_array['circle'] = '<circle class="node" style="fill: yellow; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: yellow;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "4");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 4) {
                        $x = $x+30;

                        if($x > 654) {
                            $x = 505;
                            $y = $y+22;
                        }
                        $stuff_array['circle'] = '<circle class="node" style="fill: yellow; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: yellow;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "4");
                        $counter++;
                    } else if ($referenz_array[$counter-1] >4) {
                        $x = 505;
                        $y = $y+22;
                        $stuff_array['circle'] = '<circle class="node" style="fill: yellow; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: yellow;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "4");
                        $counter++;
                    }
                }
                break;

            case "Implementierungsraum divergent":
                if ($counter == 0) {
                    $x = 655;
                    $y = 105;
                    $stuff_array['circle'] = '<circle class="node" style="fill: black; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: black;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "5");
                    $counter++;
                } else {
                    if($referenz_array[$counter-1] < 5) {
                        $x = 655;
                        $stuff_array['circle'] = '<circle class="node" style="fill: black; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: black;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "5");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 5) {
                        $x = $x+30;
                        if($x > 775) {
                            $x = 655;
                            $y = $y+22;
                        }
                        $stuff_array['circle'] = '<circle class="node" style="fill: black; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: black;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "5");
                        $counter++;
                    } else if ($referenz_array[$counter-1] >5) {
                        $x = 655;
                        $y = $y+22;
                        $stuff_array['circle'] = '<circle class="node" style="fill: black; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: black;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "5");
                        $counter++;
                    }
                }
                break;

            case "Implementierungsraum konvergent":
                if ($counter == 0) {
                    $x = 806;
                    $y = 105;
                    $stuff_array['circle'] = '<circle class="node" style="fill: brown; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: brown;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "6");
                    $counter++;

                } else {
                    if($referenz_array[$counter-1] < 6) {
                        $x = 806;
                        $stuff_array['circle'] = '<circle class="node" style="fill: brown; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: brown;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "6");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 6) {
                        $x = $x+30;
                        if($x > 926) {
                            $x = 806;
                            $y = $y+22;
                        }
                        $stuff_array['circle'] = '<circle class="node" style="fill: brown; stroke: #000; stroke-width: 1.5px;" r="5" cx="'.$x.'" cy="'.$y.'" style="fill: brown;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "6");
                        $counter++;
                    }
                }
                break;
        }

        if($i>0) {
            //array_push($stuff['links'], array('source' => $i-1, 'target' => $i));
        }
    }

    $chart = array(
        'id' => 'visualization',
        'type' => 'gantt',
        'data' => ($stuff),
    );

    //$d3_chart = d3_draw($chart);

    return $stuff;

}


