<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 02.11.16
 * Time: 08:51
 */

require_once(realpath(".") . "/sites/all/libraries/dompdf-0.6.1/dompdf_config.inc.php");
require_once("protocol_overview.module");

//include('mpdf60/mpdf.php');

function create_protocol_pdf() {

    $protocol_id =  arg(1);

    $users = getParticipants2();

    $list = explode(',', $users);

    $sql = "SELECT * FROM {protocol} WHERE protocol_id = $protocol_id";
    $result = db_query($sql);

    foreach($result as $r) {
        $protocol = $r;
    }

    $proof = false;

    global $user;
    if (in_array('administrator', $user->roles)) {
        $proof = true;
    }

    if (in_array($protocol->uid, $list) || $proof) {
        $sql = "SELECT * FROM {protocol} WHERE protocol_id = $protocol_id";
        $result = db_query($sql);

        $mpdf=new mPDF();
        $mpdf->setFooter('{PAGENO}');

        foreach($result as $r) {
            $mpdf->writeHTML($html = create_pdf_document($r));
        }
        $mpdf->Output();
        $pdf = $dompdf->output();

        return "TEST";

    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um das Protokoll einsehen zu können! <a href='".$base_url."'>Zurück zum Hauptmenü</a>", "error");
        return "";
    }

}

function create_pdf_document($protocol) {

    $methods = get_relevant_methods($protocol->protocol_id);


    $awesome_pro = $protocol;

    $arra = array();

    $users = getParticipants2();
    $sql = "SELECT * FROM {protocol} WHERE uid IN ($users) ORDER BY protocol_id";
    $result = db_query($sql);

    foreach ($result as $item) {
        array_push($arra, $item->protocol_id);
    }

    $protocol_nr = array_search($awesome_pro->protocol_id, $arra);
    $protocol_nr++;


    $date_start = new DateTime($protocol->start_date);
    $date_end = new DateTime($protocol->end_date);
    $deadline2 = $date_start->format('d.m.Y');
    $deadline3 = $date_start->format('d.m.Y H:i');
    $deadline4 = $date_end->format('d.m.Y H:i');

    $group_uid = explode(",",$protocol->participants);
    $group_uid = $group_uid[0];

    $user = user_load($group_uid);
    $group = end($user->roles);


$general_information = '
    <style>
        table {
            border-collapse: collapse;
        }

        table, td, th {
            border: 1px solid black;
        }
    </style>
        <h2 style="text-align:center">Tagesbericht '.$group.' vom '.$deadline2.'</h2>
        <div>
            <table border="1" width="100%">
            <tr>
            <th font color="#fff" bgcolor="#444444" align="left">Bezeichnung</th>
            <th font color="#fff" bgcolor="#444444" align="left">Information</th>
        </tr>
                <tr>
                    <td width="25%">Protokoll Nr.</td>
                    <td>'.$protocol_nr.'</td>
                </tr>
                <tr>
                    <td>Teilnehmer</td>
                    <td>'.parse_participants($protocol->participants).'<br></td>
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
                    <td>'.$deadline3.' - '.$deadline4.'</td>
                </tr>
                <tr>
                    <td>Autor</td>
                    <td>'.get_username($protocol->uid).'</td>
                </tr>
            </table>
        </div>';

$open_tasks = get_relevant_open_tasks($protocol);
$number = 1;

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
    $deadline3 = $date->format('d.m.Y H:i:s');
    $open_task_string .= '
        <tr>
            <td width="5%">'.$number.'</td>
            <td>'.$t->short_description.'</td>
            <td width="15%">'.get_username($t->uid).'</td>
            <td>'.$t->task.'</td>
            <td width="25%">'.$deadline3.'</td>
        </tr>';
    $number = $number + 1;
}
$open_task_string .= '</table></div>';

$new_tasks = get_relevant_new_tasks($protocol);

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

$number = 1;

    if (!$new_tasks->rowCount()) {
        $new_task_string .= '
        <tr>
            <td colspan="5" align="center">keine neuen Aufgaben vorhanden</td>

        </tr>';

    } else {
        foreach($new_tasks as $t) {
            $date = new DateTime($t->deadline);
            $deadline3 = $date->format('d.m.Y H:i:s');
            $new_task_string .= '
        <tr>
            <td width="5%">'.$number.'</td>
            <td>'.$t->short_description.'</td>
            <td width="15%">'.get_username($t->uid).'</td>
            <td>'.$t->task.'</td>
            <td width="25%">'.$deadline3.'</td>
        </tr>';
            $number = $number + 1;
        }
    }


$new_task_string .= '</table></div>';


$methods = get_relevant_methods($protocol->protocol_id);

    $newString = "";

foreach ($methods as $m) {
    $newString .= $m->name . "<br>";
}

$content_string = '
<div>
    <table border="1" width="100%">
    <tr>
            <th font color="#fff" bgcolor="#444444" align="left">Bezeichnung</th>
            <th font color="#fff" bgcolor="#444444" align="left">Information</th>
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

$methods = get_relevant_methods($protocol->protocol_id);

$method_string = '
<br>
<div>';
    $method_string .= '<h3 style="text-align:center">Übersicht der durchgeführten Methoden</h3>';

    if (!$methods->rowCount()) {
        $method_string .= '
        <table border="1" width="100%">

            <tr>

            <td colspan="5" align="center">Es wurden keine Methoden durchgeführt</td>

        </tr></table>';

    }

foreach($methods as $m) {

    global $base_url;
    $image = $base_url."/";
    if($m->rating == 20) {
        $image .= "one.png";
    } else if($m->rating == 40) {
        $image .= "two.png";
    } else if($m->rating == 60) {
        $image .= "three.png";
    } else if($m->rating == 80) {
        $image .= "four.png";
    } else if($m->rating == 100) {
        $image .= "five.png";
    }

    $method_string .= '
    <table border="1" width="100%">
    <tr>
            <th colspan="2" font color="#fff" bgcolor="#444444" align="center" style="text-align:center">'.$m->name.'</th>
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
            <td><img src='.$image.' height=20></td>
        </tr>
        <tr>
            <td width="18%"><b>Bewertung</b></td>
            <td>'.$m->evaluation.'</td>
        </tr>
        <tr>
            <td width="18%"><b>Bilder</b></td><td align="center">

        ';


    $method_string .= protocol_overview_get_images_for_pdf($m->method_execution_id);




    $method_string .= '
    </td></tr></table>
    <br>
    <br>';
}

$method_string .= '
</div>';

    $users = getParticipants2();
    $sql = "SELECT * FROM {protocol} WHERE uid IN ($users) AND protocol_id < ".$awesome_pro->protocol_id." ORDER BY date";
    $result = db_query($sql);

    $sql2 = "SELECT * FROM {protocol} WHERE uid IN ($users) AND protocol_id <= ".$awesome_pro->protocol_id." ORDER BY date";
    $result2 = db_query($sql2);

    $test = array();
    $test2 = array();

    foreach ($result as $r) {
        array_push($test, $r);
    }

    foreach ($result2 as $r2) {
        array_push($test2, $r2);
    }


    $methods = get_all_relevant_methods_before_meeting($protocol->protocol_id);
    $methods2 = get_all_relevant_methods_after_meeting($protocol->protocol_id);


    $position_before_meeting = '<h3 style="text-align:center">Phase im Design-Thinking-Prozess vor dem Treffen</h3>'.protocol_overview_generate_graphic($methods);
    $position_after_meeting = '<h3 style="text-align:center">Phase im Design-Thinking-Prozess nach dem Meeting</h3>'.protocol_overview_generate_graphic($methods2);


    $testHTML = $general_information . '<br>'.$position_before_meeting.'<br><br><h3 style="text-align:center">Inhalt des Tagesberichts</h3>' .$content_string. $new_task_string . $method_string . $position_after_meeting."<br>";

    //$testHTML = "";
    foreach($methods as $m) {
        //$testHTML = $m->execution;
    }

    //$string = htmlentities($testHTML, ENT_COMPAT | ENT_HTML5, "UTF-8", false);
    //htmlentities($string, ENT_COMPAT | ENT_HTML5, "ISO8859-1", false);

return $testHTML;

}

function protocol_overview_get_images_for_pdf($id) {

    $sql = "SELECT * FROM {method_execution_data} WHERE method_execution_id = ".$id;

    $result = db_query($sql);

    $temp = "";

    if(!$result->rowCount()) {
        $temp .= "Keine Bilder vorhanden";
    } else {
        foreach($result as $r) {
            $allowed =  array('gif','png' ,'jpg');
            $filename = $r->path;
            $basename = preg_replace('/^.+[\\\\\\/]/', '', $filename);
            $ext = pathinfo($filename, PATHINFO_EXTENSION);
            if(in_array($ext,$allowed) ) {
                global $base_url;
                $path = $base_url."/sites/default/files/".$r->path;
                $temp .= "<img style=\"max-width:350px;\" src='".$path."' height='300' width='700' alt=Selfhtml><br><b>$basename</b><br><br>";
            }
        }
    }


    return $temp;
}

function insert_image() {
    $string = "";
    //$string = "<h4>test</h4>";
    //$string .= "<img src='graphic.png' alt='Selfhtml'>";
    return $string;
}

function get_relevant_open_tasks($protocol) {
    $sql = "SELECT * FROM {task} WHERE protocol_id < $protocol->protocol_id AND done = 0";
    $result = db_query($sql);

    return $result;
}

function get_relevant_new_tasks($protocol) {
    $sql = "SELECT * FROM {task} WHERE protocol_id = $protocol->protocol_id AND done = 0";
    $result = db_query($sql);

    return $result;
}

function get_username($uid) {
    $user=user_load($uid);
    $username=$user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'];
    return $username;
}

function parse_participants($participants) {
    $users = explode(",",$participants);

    $numItems = count($users);
    $i = 0;

    $string = "";
    foreach($users as $u) {
        $u = get_username($u);
        $string .= $u;
        if(++$i != $numItems) {
            $string .= "<br>";
        }
    }
    return $string;
}

function get_relevant_methods($protocol_id) {
    $sql = "SELECT * FROM {method_execution} WHERE protocol_id = $protocol_id";
    $result = db_query($sql);

    return $result;
}

function get_all_relevant_methods_before_meeting($protocol_id) {
    $users = getParticipants2();
    $sql = "SELECT * FROM {method_execution} WHERE uid IN ($users) AND protocol_id < $protocol_id";
    $result = db_query($sql);

    return $result;
}

function get_all_relevant_methods_after_meeting($protocol_id) {
    $users = getParticipants2();
    $sql = "SELECT * FROM {method_execution} WHERE uid IN ($users) AND protocol_id <= $protocol_id";
    $result = db_query($sql);

    return $result;
}


function get_user_array($users) {
    $usernames = "";
    $array = explode(",", $users);

    foreach ($array as $item) {
        $user=user_load($item);

        $first_name = $user->field_first_name['und'][0]['value'];
        $last_name =  $user->field_last_name['und'][0]['value'];
        $mat_nr = $user->field_matrikelnummer['und'][0]['value'];

        $usernames .='<h3 align="center">'.$first_name.' '.$last_name.' '.$mat_nr.'</h3>';
    }

    return $usernames;

}


function create_total_protocol() {
    $counter = 1;
    $heute = date("d.m.Y");

    $users = getParticipants2();
    $names = get_user_array($users);


    global $user;
    $group = end($user->roles);

    if (in_array('administrator', $user->roles)) {

        $id = arg(1);
        if($id == 0) {
            $id = 3;
        }

        $sql = "SELECT * FROM {role} WHERE rid = ".$id;
        $result = db_query($sql);
        foreach ($result as $item) {
            $group = $item->name;
        }

        $users = array();
        $sql = "SELECT * FROM {users_roles} WHERE rid = ".$id;
        $proresult = db_query($sql);

        foreach($proresult as $pro) {
            array_push($users, $pro->uid);
        }

        $users = implode(',', $users);
        $names = get_user_array($users);
    }


    $html = '<style>
..fblogo {
    display: inline-block;
    margin-left: auto;
    margin-right: auto;
    height: 30px;
}

#images{
    text-align:center;
}

        table {
            border-collapse: collapse;
        }

        table, td, th {
            border: 1px solid black;
        }

    </style>
    <br>
    <br><br>

   <div id="images">
    <img class="fblogo" src="DT_Logo_2.png" alt="Smiley face" style="height:150px;width:170px;">
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
        &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
    &nbsp;
        &nbsp;


    <img class="fblogo" src="uni_logo.png" alt="Smiley face" style="height:150px;width:170px;">
</div>

<br><br>
<h1 style="font-size: 3em;" align="center">Dokumentation '.$group.'</h1>
<br><br><br>

'.$names.'



<h3 align="center">'.$heute.'</h3>';

    $mpdf=new mPDF();

    //Titelblatt
    $mpdf->WriteHTML($html);

    //Inhaltsverzeichnis
    $mpdf->TOCpreHTML = '<h2>Contents - Portrait</h2>';
    $mpdf->WriteHTML('<tocpagebreak toc-preHTML="Inhaltsverzeichnis" />');



    if (in_array('administrator', $user->roles)) {
        $users = array();
        $sql = "SELECT * FROM {users_roles} WHERE rid = ".$id;
        $proresult = db_query($sql);

        foreach($proresult as $pro) {
            array_push($users, $pro->uid);
        }

        $users = implode(',', $users);
    } else {
        $users = getParticipants2();
    }

    $sql = "SELECT * FROM {protocol} WHERE uid IN ($users) AND finished = 1 ORDER BY date";
    $result = db_query($sql);

    $counter = 1;

    $users55 = getParticipants2();
    $sql55 = "SELECT * FROM {protocol} WHERE uid IN ($users) ORDER BY date";
    $result55 = db_query($sql55);

    $sql255 = "SELECT * FROM {protocol} WHERE uid IN ($users) ORDER BY date";
    $result255 = db_query($sql255);

    $test55 = array();
    $test255 = array();

    foreach ($result55 as $r55) {
        array_push($test55, $r55);
    }

    foreach ($result255 as $r255) {
        array_push($test255, $r255);
    }

    $participants = get_participants_for_methods();
    $sql = "SELECT * FROM {method_execution} WHERE uid in (" . $participants . ") ORDER BY creation_date";
    $result22 = db_query($sql);

    $position_before_meeting2 = '<h2 style="text-align:center">Gruppenübersicht</h2>';
    $position_before_meeting2 .= protocol_overview_generate_graphic($result22);

    $position_before_meeting2 .= '<br><br><table border="1" width="100%" align="center"><tr><th font color="#fff" bgcolor="#444444" align="left">Nr.</th><th font color="#fff" bgcolor="#444444" align="left">Datum</th><th font color="#fff" bgcolor="#444444" align="left">Methodenname</th><th font color="#fff" bgcolor="#444444" align="left">Bewertung</th></tr>';

    $result22 = db_query($sql);
    $id = 1;
    foreach ($result22 as $item66) {


        $image = "";
        if($item66->rating == 20) {
            $image = "one.png";
        } else if($item66->rating == 40) {
            $image = "two.png";
        } else if($item66->rating == 60) {
            $image = "three.png";
        } else if($item66->rating == 80) {
            $image = "four.png";
        } else if($item66->rating == 100) {
            $image = "five.png";
        }

        $date = new DateTime($item66->creation_date);
        $deadline2 = $date->format('d.m.Y');


        $position_before_meeting2 .= '<tr><td>'.$id.'</td><td>'.$deadline2.'</td><td>'.$item66->name.'</td><td><img src="'.$image.'" height="20" width="100"></td></tr>';
        $id++;
    }

    $position_before_meeting2 .= '</table>';

    $mpdf->setFooter('{PAGENO}');
    $mpdf->WriteHTML('<tocentry sheet-size="A4" content="Gruppenübersicht" />'.$position_before_meeting2);
    $mpdf->WriteHTML('<pagebreak  />');



    foreach($result as $res) {
        $html2 = test($res, $counter);
        $date = new DateTime($res->start_date);
        $deadline2 = $date->format('d.m.Y');

        $blabla = create_pdf_document($res);

        $mpdf->WriteHTML('<tocentry sheet-size="A4" content="Tagesbericht vom '.$deadline2.'" />'.$blabla);
        $mpdf->WriteHTML('<pagebreak  />');
        $counter = $counter + 1;

    }
    if (in_array('administrator', $user->roles)) {

        $users = array();
        $sql = "SELECT * FROM {users_roles} WHERE rid = 3";
        $proresult = db_query($sql);

        foreach($proresult as $pro) {
            array_push($users, $pro->uid);
        }

        $parts = implode(',', $users);
    } else {
        $parts = getParticipants900();
    }


    $sql = "SELECT * FROM {user_reflection} WHERE uid in (".$parts . ")";
    $reflectionResult = db_query($sql);
    foreach($reflectionResult as $refres) {
        $user = user_load($refres->uid);
        $mpdf->WriteHTML('<tocentry content="Reflexion von '.$user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'].'"/><h2 style="text-align:center">Reflexion von '.$user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'].'</h2><p>'.$refres->reflection.'</p>');
        $mpdf->WriteHTML('<pagebreak sheet-size="A4-P" />');
    }

    global $user;

    $group = end($user->roles);
    $role_id = 0;


    $sql = "SELECT * FROM {role} WHERE name = '".$group."'";
    $result = db_query($sql);

    foreach ($result as $item) {
        $role_id = $item->rid;
    }

    $sql = "SELECT * FROM {group_reflection} WHERE role_id = ".$role_id;
    $reflectionResult = db_query($sql);
    foreach($reflectionResult as $refres) {
        $mpdf->WriteHTML('<tocentry content="Reflexion der gesamten Gruppe"/><h2 style="text-align:center">Reflexion der gesamten Gruppe</h2><p>'.$refres->reflection.'</p>');
        $mpdf->WriteHTML('<pagebreak sheet-size="A4-P" />');
    }

    $mpdf->Output();
    $pdf = $dompdf->output();


    return "Test";

}

function protocol_overview_parse_participants($participants) {
    $users = explode(",",$participants);

    $numItems = count($users);
    $i = 0;

    $string = "";
    foreach($users as $u) {
        $u = protocol_overview_get_username($u);
        $string .= $u;
        if(++$i != $numItems) {
            $string .= "<br>";
        }
    }
    return $string;
}

function protocol_overview_get_username($uid) {
    $user=user_load($uid);
    $username=$user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'];
    return $username;
}

function protocol_overview_get_relevant_open_tasks($protocol) {
    $users = getParticipants2();
    $sql = "SELECT * FROM {task} WHERE uid IN ($users) AND protocol_id < $protocol->protocol_id AND done = 0";
    $result = db_query($sql);

    return $result;
}

function protocol_overview_get_relevant_new_tasks($protocol) {
    $users = getParticipants2();
    $sql = "SELECT * FROM {task} WHERE uid IN ($users) AND protocol_id = $protocol->protocol_id AND done = 0";
    $result = db_query($sql);

    return $result;
}

function protocol_overview_get_relevant_methods($protocol_id) {
    $sql = "SELECT * FROM {method_execution} WHERE protocol_id = $protocol_id";
    $result = db_query($sql);

    return $result;
}

function protocol_overview_generate_graphic($protocols) {

    $test = array();

    foreach ($protocols as $r) {
        array_push($test, $r->raum_end . " " . $r->phase_end);
    }

    $array = protocol_overview_create_small_chart($test);

    $circles = "";
    $lines = "";

    for($i=0;$i<sizeof($array);$i++) {
        $circles .= $array[$i]['circle'];
        if($i > 0) {
            $lines .= '<line class="link" style="stroke: #000 stroke-width: 1.5px;" x1="'.$array[$i-1]['x'].'" y1="'.$array[$i-1]['y'].'" x2="'.$array[$i]['x'].'" y2="'.$array[$i]['y'].'"></line>';
        }
    }

    $chart = '
<style>

.link {
  stroke: #000;

}


</style><svg width="1030.5992" height="420">
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
    <rect x="40" y="395" width="902" height="30" style="fill: rgb(89, 89, 89);"></rect>
    <text x="395" y="414" font-family="sans-serif" font-size="22px" fill="white">Projektmanagement</text>
</g>
    <rect x="40" y="94" width="150" height="300" style="fill: rgb(173, 145, 190);"></rect>
    <rect x="190" y="94" width="150" height="300" style="fill: rgb(173, 145, 190);"></rect>
    <rect x="341" y="94" width="150" height="300" style="fill: rgb(122, 155, 185);"></rect>
    <rect x="491" y="94" width="150" height="300" style="fill: rgb(122, 155, 185);"></rect>
    <rect x="642" y="94" width="150" height="300" style="fill: rgb(112, 196, 111);"></rect>
    <rect x="792" y="94" width="150" height="300" style="fill: rgb(112, 196, 111);"></rect>

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

function protocol_overview_create_small_chart($result) {

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
                    $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: green; stroke: #000; stroke-width: 1.5px;"></circle>';
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
                            $y = $y+25;
                        }
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: green; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "1");
                        $counter++;
                    } else {
                        $y = $y+25;
                        $x = 50;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: green; stroke: #000; stroke-width: 1.5px;"></circle>';
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
                    $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: red; stroke: #000; stroke-width: 1.5px;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "2");
                    $counter++;
                } else {
                    if($referenz_array[$counter-1] <2) {
                        $x = 203;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: red; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "2");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 2) {
                        $x = $x+30;
                        if($x > 323) {
                            $x = 203;
                            $y = $y+25;
                        }
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: red; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "2");
                        $counter++;
                    } else if ($referenz_array[$counter-1] >2) {
                        $x = 203;
                        $y = $y+25;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: red; stroke: #000; stroke-width: 1.5px;"></circle>';
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
                    $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: blue; stroke: #000; stroke-width: 1.5px;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "3");
                    $counter++;
                } else {
                    if($referenz_array[$counter-1] < 3) {
                        $x = 355;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: blue; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "3");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 3) {
                        $x = $x+30;
                        if($x > 475) {
                            $x = 355;
                            $y = $y+25;
                        }
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: blue; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "3");
                        $counter++;
                    } else if ($referenz_array[$counter-1] >3) {
                        $x = 355;
                        $y = $y+25;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: blue; stroke: #000; stroke-width: 1.5px;"></circle>';
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
                    $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: yellow; stroke: #000; stroke-width: 1.5px;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "4");
                    $counter++;
                } else {
                    if($referenz_array[$counter-1] < 4) {
                        $x = 505;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: yellow; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "4");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 4) {
                        $x = $x+30;

                        if($x > 654) {
                            $x = 505;
                            $y = $y+25;
                        }
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: yellow; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "4");
                        $counter++;
                    } else if ($referenz_array[$counter-1] >4) {
                        $x = 505;
                        $y = $y+25;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: yellow; stroke: #000; stroke-width: 1.5px;"></circle>';
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
                    $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: black; stroke: #000; stroke-width: 1.5px;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "5");
                    $counter++;
                } else {
                    if($referenz_array[$counter-1] < 5) {
                        $x = 655;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: black; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "5");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 5) {
                        $x = $x+30;
                        if($x > 775) {
                            $x = 655;
                            $y = $y+25;
                        }
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: black; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "5");
                        $counter++;
                    } else if ($referenz_array[$counter-1] >5) {
                        $x = 655;
                        $y = $y+25;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: black; stroke: #000; stroke-width: 1.5px;"></circle>';
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
                    $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: brown; stroke: #000; stroke-width: 1.5px;"></circle>';
                    $stuff_array['x'] = $x;
                    $stuff_array['y'] = $y;
                    array_push($stuff,$stuff_array);
                    array_push($referenz_array, "6");
                    $counter++;

                } else {
                    if($referenz_array[$counter-1] < 6) {
                        $x = 806;
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: brown; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "6");
                        $counter++;
                    } else if ($referenz_array[$counter-1] == 6) {
                        $x = $x+30;
                        if($x > 926) {
                            $x = 806;
                            $y = $y+25;
                        }
                        $stuff_array['circle'] = '<circle class="node" r="6" cx="'.$x.'" cy="'.$y.'" style="fill: brown; stroke: #000; stroke-width: 1.5px;"></circle>';
                        $stuff_array['x'] = $x;
                        $stuff_array['y'] = $y;
                        array_push($stuff,$stuff_array);
                        array_push($referenz_array, "6");
                        $counter++;
                    }
                }
                break;
        }

    }

    return $stuff;

}




