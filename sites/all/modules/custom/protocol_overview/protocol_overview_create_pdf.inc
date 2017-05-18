<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 02.11.16
 * Time: 08:51
 */

require_once(realpath(".") . "/sites/all/libraries/dompdf-0.6.1/dompdf_config.inc.php");
require_once("protocol_overview.module");
require_once("protocol_overview_pdf_new_tasks.inc");
require_once("protocol_overview_pdf_content_overview.inc");
require_once("protocol_overview_pdf_methods.inc");
require_once("protocol_overview_pdf_front_page.inc");
require_once("protocol_overview_pdf_group_overview.inc");
require_once("protocol_overview_pdf_reflections.inc");
require_once("protocol_overview_pdf_dynamic_graphic.inc");

/*
 * Diese Funktion generiert ein PDF-Protokoll aus einem Protokoll, welches als ID übergeben bekommen wird.
 */
function create_protocol_pdf()
{
    //Die ID des entsprechenden Protokolls wird übergeben
    $protocol_id = arg(1);

    //Die Gruppenmitglieder werden ermittelt und für die Weiterverarbeitung konvertiert
    $users = protocol_overview_get_participants_from_logged_in_user_group();
    $user_list = explode(',', $users);

    //Der aktuell-angemeldete Benutzer wird geladen
    global $user;

    //Die Erstellung eines PDF-Protokolls kann entweder von den einzelnen Gruppenmitgliedern oder vom
    //Administrator durchgeführt werden. Die Variable überprüft diese Einschränkung und legt fest, ob
    //das PDF erstellt wird oder nicht
    $proof = false;

    //Sollte der aktuell angemeldete Benutzer Administrator sein, wird die PDF-Erstellung erlaubt
    if (in_array('administrator', $user->roles)) {
        $proof = true;
    }

    //Das Protokoll-Element wird als Variable angelegt
    $protocol = "";

    //Das zu konvertierende Protokoll wird aus der Datenbank geladen
    $sql = "SELECT * FROM {protocol} WHERE protocol_id = $protocol_id";
    $result = db_query($sql);

    foreach ($result as $r) {
        $protocol = $r;
    }

    $group = end($user->roles);
    $role_id = 0;
    $sql = "SELECT * FROM {role} WHERE name = '" . $group . "'";
    $result = db_query($sql);

    foreach ($result as $item) {
        $role_id = $item->rid;
    }

    //Sollte sich die Nutzer ID des Autors des Protokolls in der user_list befinden oder der aktuell
    //angemeldete Benutzer Administrator sein,
    if (in_array($protocol->uid, $user_list) || $proof) {

        //mpdf ist eine Bibliothek, welche es ermöglicht, ein PDF zu generieren. Zunächst wird ein
        //neues Element angelegt
        $mpdf = new mPDF();

        //Die Seitenzahlen werden eingefügt und beginnen auf der ersten Seite
        $mpdf->setFooter('{PAGENO}');

        //Das Protokoll wird einer Funktion übergeben, welche dieses in HTML-Code umwandelt und zurückgibt
        $mpdf->writeHTML(create_pdf_document_from_a_protocol($protocol, $role_id));

        //Zum Schluss wird das PDF ausgegeben
        $mpdf->Output();
        $pdf = $dompdf->output();

        //Sollte der Nutzer nicht die Befugnis haben, ein PDF zu erstellen, wird eine Fehlermeldung ausgegeben
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um das Protokoll einsehen zu können! <a href='" . $base_url . "'>Zurück zum Hauptmenü</a>", "error");
        return "";
    }
}

/*
 * Diese Funktion wandelt ein übergebenes Protokoll-Element in HTMl-Code um und gibt diesen zurück
 */
function create_pdf_document_from_a_protocol($protocol, $role_id)
{

    //Das übergebene Protokoll wird in eine Variable für die Weiterbenutzung gespeichert
    $protocol_element = $protocol;

    $temp_protocol_array = array();

    //Um das passende Protokoll zu finden, werden zunächst alle Protokolle der Gruppe geladen. Im Anschluss
    //werden diese in ein Array gespeichert und nach der Protokoll_ID des übergebenen Protokolls gesucht, um
    //die Nummer des Protokolls zu ermitteln

    //$users = protocol_overview_get_participants_from_logged_in_user_group();
    $users = protocol_overview_get_participants_by_protocol($protocol_element->protocol_id);
    $sql = "SELECT * FROM {protocol} WHERE uid IN ($users) ORDER BY protocol_id";
    $before_meeting_result = db_query($sql);

    foreach ($before_meeting_result as $item) {
        array_push($temp_protocol_array, $item->protocol_id);
    }

    $protocol_nr = array_search($protocol_element->protocol_id, $temp_protocol_array);
    $protocol_nr++;

    //Start und Endzeitpunkt des Protokolls werden ermittelt und für die Anzeige im PDF konvertiert
    $start_date = new DateTime($protocol->start_date);
    $end_date = new DateTime($protocol->end_date);
    $converted_start_date_for_headline = $start_date->format('d.m.Y');
    $converted_start_date_for_table = $start_date->format('d.m.Y H:i');
    $converted_end_date_for_table = $end_date->format('d.m.Y H:i');

    //Anhand der Gruppenmitglieder wird der Name der Gruppe ermittelt
    $group_participants = explode(",", $protocol->participants);
    $group_participants = $group_participants[0];
    $group_user = user_load($group_participants);
    $group_name = end($group_user->roles);

    //Allgemeine Informationen des Protokolls werden geladen
    $general_information = protocol_overview_pdf_general_information($group_name, $converted_start_date_for_headline, $converted_start_date_for_table, $converted_end_date_for_table, $protocol, $protocol_nr);

    //Alle bisher durchgeführten Methoden exklusive die des aktuellen Meetings werden geladen
    $methods_before_meeting = protocol_overview_get_all_relevant_methods_before_meeting($protocol->protocol_id);

    //Die dazugehörige Grafik wird geladen
    $position_before_meeting = '<h3 style="text-align:center">Phase im Design-Thinking-Prozess vor dem Treffen</h3>' . protocol_overview_generate_graphic($methods_before_meeting);

    //Die durchgeführten Methoden innerhalb des Meetings werden geladen
    $methods_in_meeting = protocol_overview_get_relevant_methods($protocol->protocol_id);

    //Informationen zum Treffen (Inhalt, Fazit, Probleme etc.) werden geladen
    $content_string = protocol_overview_pdf_content_overview($protocol, $methods_in_meeting);

    //Durchgeführte Methoden innerhalb des Meetings werden erneut geladen, um sie im Anschluss detailliert anzuzeigen
    $methods = protocol_overview_get_relevant_methods($protocol->protocol_id);
    $method_string = protocol_overview_pdf_method_overview($methods);

    //Aufgaben, die während des Treffens angelegt wurden, werden geladen
    $new_task_string = protocol_overview_pdf_new_task_string($protocol);

    //Alle bisher durchgeführten Methoden inklusive die des aktuellen Meetings werden geladen
    $methods_after_meeting = protocol_overview_get_all_relevant_methods_after_meeting($protocol->protocol_id);

    //Die dazugehörige Grafik wird geladen
    $position_after_meeting = '<h3 style="text-align:center">Phase im Design-Thinking-Prozess nach dem Meeting</h3>' . protocol_overview_generate_graphic($methods_after_meeting);

    //Die einzelnen Bestandteile werden zusammengesetzt
    $protocol_string =
        $general_information . '<br>' .
        $position_before_meeting . '<br><br><h3 style="text-align:center">Inhalt des Tagesberichts</h3>' .
        $content_string .
        $new_task_string .
        $method_string .
        $position_after_meeting .
        "<br>";

    //Der HTML-Code des Protokoll-Elements wird zurückgegeben
    return $protocol_string;

}

/*
 * Diese Funktion lädt alle hochgeladenen Dateien zu einem Protokoll aus der Datenbank
 */
function protocol_overview_get_images_for_pdf($id)
{

    //Die Daten zur übergebenen Protokoll_ID werden geladen
    $sql = "SELECT * FROM {method_execution_data} WHERE method_execution_id = " . $id;
    $result = db_query($sql);

    //Die String Variable wird angelegt, um sie später mit den Dateien zu befüllen
    $data_string = "<div>";

    //Sollte es keine Dateien geben, wird eine Info zurückgegeben
    if (!$result->rowCount()) {
        $data_string .= "Keine Bilder vorhanden";

    } else {

        //Es wird über die Daten iteriert
        foreach ($result as $r) {
            $allowed = array('gif', 'png', 'jpg');

            //Die Dateiendung wird ermittelt, um sie zu überprüfen
            $filename = $r->path;
            $basename = preg_replace('/^.+[\\\\\\/]/', '', $filename);
            $ext = pathinfo($filename, PATHINFO_EXTENSION);

            //Sollte es sich um eine Bilddatei handeln, kann sie angezeigt werden
            if (in_array($ext, $allowed)) {
                global $base_url;

                //Der Dateipfad der Datei wird ermittelt
                $path = $base_url . "/sites/default/files/" . $r->path;

                //style="text-align: center; text-decoration: underline;"

                $data_string .= "<img style=\"max-width:550px;\"  src='" . $path . "' height='300' width='700' alt=Selfhtml><br><b>$basename</b><br><br>";
            }
        }
    }

    $data_string .= "</div>";

    return $data_string;
}

/*
 * Diese Funktion empfängt einen String, in dem Nutzer-IDs mit Kommata getrennt werden. Im Anschluss werden die passenden
 * Vor- und Nachnamen ermittelt und ebenfalls in einen String geschrieben, welcher am Ende zurückgegeben wird.
 */
function protocol_overview_convert_participants($participant_ids)
{
    //Die User-IDs werden in ein Array geschrieben und gezählt
    $user_id_array = explode(",", $participant_ids);
    $user_counter = count($user_id_array);
    $i = 0;

    //Diese Variable wird erstellt und später mit den Nutzernamen befüllt
    $user_string = "";

    //Über die Nutzerliste wird iteriert
    foreach ($user_id_array as $user) {

        //Vor- und Nachname des Nutzers wird ermittelt
        $user = protocol_overview_get_username($user);

        //Der Nutzer wird dem Rückgabe-String hinzugefügt
        $user_string .= $user;

        //Sollte es sich nicht um den ersten Eintrag handeln, soll kein Zeilenumbruch gemacht werden
        if (++$i != $user_counter) {
            $user_string .= "<br>";
        }
    }

    return $user_string;
}

/*
 * Diese Funktion lädt alle Methodendurchführungen der Gruppe aus der Datenbank, exklusive der Durchführungen, die
 * innerhalb des aktuellen Protokolls angelegt wurden
 */
function protocol_overview_get_all_relevant_methods_before_meeting($protocol_id)
{
    $users = protocol_overview_get_participants_from_logged_in_user_group();
    $sql = "SELECT * FROM {method_execution} WHERE uid IN ($users) AND protocol_id < $protocol_id";
    $result = db_query($sql);

    return $result;
}

/*
 * Diese Funktion lädt alle Methodendurchführungen der Gruppe aus der Datenbank, inklusive der Durchführungen, die
 * innerhalb des aktuellen Protokolls angelegt wurden
 */
function protocol_overview_get_all_relevant_methods_after_meeting($protocol_id)
{
    $users = protocol_overview_get_participants_from_logged_in_user_group();
    $sql = "SELECT * FROM {method_execution} WHERE uid IN ($users) AND protocol_id <= $protocol_id";
    $result = db_query($sql);

    return $result;
}

/*
 * Für das Titelblatt der Dokumentation wird ein übergebener User-String so konvertiert, dass der Rückgabestring
 * Vorname, Nachname und Matrikelnummer der Benutzer enthält
 */
function protocol_overview_get_user_array($user_string)
{
    //Die String-Variable wird angelegt, später mit Daten gefüllt und am Ende zurückgegeben
    $user_information = "";
    $user_array = explode(",", $user_string);

    foreach ($user_array as $u) {

        //Das User-Element wird anhand der ID geladen
        $user = user_load($u);

        //Benötigte Informationen des Users werden geladen
        $first_name = $user->field_first_name['und'][0]['value'];
        $last_name = $user->field_last_name['und'][0]['value'];
        $mat_nr = $user->field_matrikelnummer['und'][0]['value'];

        //Informationen des Users werden in HTML-Code verpackt
        $user_information .= '<h3 align="center">' . $first_name . ' ' . $last_name . ' ' . $mat_nr . '</h3>';
    }

    return $user_information;

}

/*
 * Diese Funktion baut eine vollständige Dokumentation. Diese enthält ein Deckblatt, Inhaltsverzeichnis, Kurzübersicht,
 * alle Protokolle und Reflexionen der Gruppe und den einzelnen Teilnehmern
 */
function create_total_documentation()
{

    //Das heutige Datum wird ermittelt
    $heute = date("d.m.Y");

    //Die Teilnehmer-IDs der Gruppe werden geladen
    $users = protocol_overview_get_participants_from_logged_in_user_group();

    //Die Namen der Teilnehmer werden geladen
    $names = protocol_overview_get_user_array($users);

    //der aktuell angemeldete Benutzer wird geladen
    global $user;

    //Die Rolle (also der Gruppenname) wird bestimmt
    $group = end($user->roles);

    //Sollte es sich bei dem Nutzer um den Administrator handeln, wird ihm der Zugriff auf die Dokumentation
    //ebenfalls gewährt. Ansonsten ist dies nur der entsprechenden Gruppe vorbehalten. In diesem Fall wird der
    //Übergabeparameter (die Rollen-ID) aus der URL ausgelesen und verarbeitet
    if (in_array('administrator', $user->roles)) {

        //Da anhand der übegebenen Rollen-ID die Daten aus der Datenbank geladen werden, wird die Rollen-ID des
        //Administrators (0) in die der Rolle des Administrators (3) umgewandelt
        $id = arg(1);

        if ($id == 0) {
            $id = 3;
        }

        //Der Name der Gruppe wird ermittelt
        $sql = "SELECT * FROM {role} WHERE rid = " . $id;
        $result = db_query($sql);
        foreach ($result as $item) {
            $group = $item->name;
        }

        //Die Teilnehmer der Gruppe werden ermittelt
        $users = array();
        $sql = "SELECT * FROM {users_roles} WHERE rid = " . $id;
        $user_roles_result = db_query($sql);

        foreach ($user_roles_result as $user_role) {
            array_push($users, $user_role->uid);
        }

        $users = implode(',', $users);

        //Die Namen der Teilnehmer werden ermittelt und in einen HTML-String konvertiert
        $names = protocol_overview_get_user_array($users);
    }

    //ein neues mpdf-Element wird erstellt, welches befüllt und am Ende ausgegeben wird
    $mpdf = new mPDF();

    //die Titelseite wird erstelt und zurückgegeben
    $front_page = protocol_overview_pdf_front_page($group, $names, $heute);

    //Titelblatt wird dem mpdf-Element hinzugefügt
    $mpdf->WriteHTML($front_page);

    //Inhaltsverzeichnis wird dem mpdf-Element hinzugefügt
    $mpdf->TOCpreHTML = '<h2>Contents - Portrait</h2>';
    $mpdf->WriteHTML('<tocpagebreak toc-preHTML="Inhaltsverzeichnis" />');

    //Sollte der aktuell angemeldete Benutzer der Administrator sein, muss bei der Bestimmung der
    //Gruppenteilnehmer getrickst werden, damit die Namen der eigentlichen Gruppe geladen werden. Sonst würden
    //hier die Namen der Administrator-Gruppe geladen
    if (in_array('administrator', $user->roles)) {
        $users = array();
        $sql = "SELECT * FROM {users_roles} WHERE rid = " . $id;
        $user_roles_result = db_query($sql);

        foreach ($user_roles_result as $user_role) {
            array_push($users, $user_role->uid);
        }

        $users = implode(',', $users);
    } else {
        $users = protocol_overview_get_participants_from_logged_in_user_group();
    }

    //die Gruppenübersicht wird geladen. Sie zeigt die Design-Thinking-Grafik mit allen durchgeführten Methoden.
    //Diese Methoden werden im Anschluss zusätzlich innerhalb einer Tabelle angezeigt
    $group_overview = protocol_overview_pdf_group_overview(arg(1));

    //Die Gruppenübersicht wird dem mpdf-Element hinzugefügt
    $mpdf->setFooter('{PAGENO}');
    $mpdf->WriteHTML('<tocentry sheet-size="A4" content="Gruppenübersicht" />' . $group_overview);
    $mpdf->WriteHTML('<pagebreak  />');


    //Es werden alle Protokolle der Gruppe geladen
    $sql = "SELECT * FROM {protocol} WHERE uid IN ($users) AND finished = 1 ORDER BY date";
    $protocol_result = db_query($sql);

    //Im Anschluss wird über alle Protokolle iteriert
    foreach ($protocol_result as $protocol) {

        //das Protokoll wird in HTML-Code umgewandelt
        $acutal_html_protocol = create_pdf_document_from_a_protocol($protocol, arg(1));

        //Das Startdatum des Protokolls wird ermittelt
        $start_date = new DateTime($protocol->start_date);
        $converted_start_date = $start_date->format('d.m.Y');

        //Der HTML-Code des Protokolls wird dem mpdf-Element hinzugefügt
        $mpdf->WriteHTML('<tocentry sheet-size="A4" content="Tagesbericht vom ' .
            $converted_start_date . '" />' . $acutal_html_protocol);
        $mpdf->WriteHTML('<pagebreak  />');
    }

    //Hier wieder der Sonderfall, falls es sich bei dem aktuell angemeldeten Benutzer um den Administrator handelt
    if (in_array('administrator', $user->roles)) {

        $role_id = arg(1);

        $users = array();
        $sql = "SELECT * FROM {users_roles} WHERE rid = " . $role_id;
        $user_roles_result = db_query($sql);

        foreach ($user_roles_result as $user_role) {
            array_push($users, $user_role->uid);
        }

        $parts = implode(',', $users);

    } else {
        $parts = protocol_overview_get_participants_from_logged_in_user_group();
    }

    //Die Reflexionen der Gruppenmitglieder werden geladen
    $sql = "SELECT * FROM {user_reflection} WHERE uid in (" . $parts . ")";
    $reflection_result = db_query($sql);

    //Die Reflexion jedes Gruppenmitglieds wird in HTML-Code umgewandelt und zum mpdf-ELement hinzugefügt
    foreach ($reflection_result as $reflection) {
        $user = user_load($reflection->uid);
        $mpdf->WriteHTML('<tocentry content="Reflexion von ' . protocol_overview_get_username($reflection->uid) . '"/>'
            . protocol_overview_pdf_user_reflection($user, $reflection));
        $mpdf->WriteHTML('<pagebreak sheet-size="A4-P" />');
    }

    //Die Rollen-ID (Gruppen-ID) der Gruppe wird ermittelt
    global $user;
    $group = end($user->roles);
    $role_id = 0;
    $sql = "SELECT * FROM {role} WHERE name = '" . $group . "'";
    $result = db_query($sql);

    foreach ($result as $item) {
        $role_id = $item->rid;
    }

    //Die Gruppenreflexion wird anhand der ID aus der Datenbank geladen , HTML-Code umgewandelt und zum mdf-Element
    //hinzugefügt
    $sql = "SELECT * FROM {group_reflection} WHERE role_id = " . $role_id;
    $group_reflection_result = db_query($sql);
    foreach ($group_reflection_result as $group_reflection) {
        $mpdf->WriteHTML('<tocentry content="Reflexion der gesamten Gruppe"/>' .
            protocol_overview_pdf_group_reflection($group_reflection));
        $mpdf->WriteHTML('<pagebreak sheet-size="A4-P" />');
    }

    //Die fertige Dokumentation wird ausgegeben
    $mpdf->Output();
    $pdf = $dompdf->output();
}

//Diese Funktion erhält eine User-ID und gibt den passenden Vor- und Nachnamen des Nutzers zurück
function protocol_overview_get_username($uid)
{
    $user = user_load($uid);
    $username = $user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'];
    return $username;
}

//Diese Funktion lädt alle Aufgaben, die innerhalb eines Protokolls angelegt wurden, aus der Datenbank. Das Protokoll-
//Element wird der Funktion übergeben
function protocol_overview_get_relevant_new_tasks($protocol)
{
    $users = protocol_overview_get_participants_from_logged_in_user_group();
    $sql = "SELECT * FROM {task} WHERE uid IN ($users) AND protocol_id = $protocol->protocol_id AND done = 0";
    $result = db_query($sql);

    return $result;
}

//Diese Funktion lädt alle Methodendurchführungen eines Protokolls aus der Datenbank. Die ID des Protokolls wird
//der Funktion übergeben
function protocol_overview_get_relevant_methods($protocol_id)
{
    $sql = "SELECT * FROM {method_execution} WHERE protocol_id = $protocol_id";
    $result = db_query($sql);

    return $result;
}

//Diese Funktion baut die Design-Thinking-Grafik mit den dynamischen Punkten. Ein Array mit den Protokollen wird
//dazu der Funktion übergeben
function protocol_overview_generate_graphic($methods)
{

    $method_room_phase = array();

    //Der Raum und die Phase nach der Methodendurchführung wird aus dem Methoden-Element ausgelesen
    foreach ($methods as $method) {

    $validation = $method->raum_end . " " . $method->phase_end;

    if (!strcmp($method->room, "Vorbereitung") || !strcmp($method->room, "Projektmanagement")) {

    } else {
        array_push($method_room_phase, $validation);
    }

}

    //Die Daten für die Grafik werden ermittelt
    $graphic_data = protocol_overview_get_data_for_dynamic_graphic($method_room_phase);

    $circles = "";
    $lines = "";

    //Die Grafik-Daten werden ausgewertet und in HTML-Code konvertiert. Dieser enthält svg-Tags, sodass sie innerhalb
    //des PDFs angezeigt werden können
    for ($i = 0; $i < sizeof($graphic_data); $i++) {
        $circles .= $graphic_data[$i]['circle'];
        if ($i > 0) {
            $lines .= '<line class="link" style="stroke: #000 stroke-width: 1.5px;" x1="' . $graphic_data[$i - 1]['x'] . '" y1="' . $graphic_data[$i - 1]['y'] . '" x2="' . $graphic_data[$i]['x'] . '" y2="' . $graphic_data[$i]['y'] . '"></line>';
        }
    }

    //Die Grafik wird anhand der Daten generiert
    $graphic = protocol_overview_pdf_create_dynamic_graphic($circles, $lines);

    return $graphic;
}

//Diese Funktion lädt alle Gruppenmitglieder des angemeldeten Nutzers aus der Datenbank
function protocol_overview_get_participants_by_protocol($protocol_id)
{
    $protocol = protocol_overview_get_protocol_from_id($protocol_id);

    $users = array();

    $author = $protocol->uid;
    $user = user_load($author);
    $user_id = $user->uid;

    $sql = "SELECT ur.rid, r.name, ur.uid FROM {role} AS r INNER JOIN {users_roles} AS ur ON r.rid = ur.rid WHERE ur.rid = (SELECT usro.rid FROM {users_roles} as usro WHERE usro.uid = $user_id)";
    $role_result = db_query($sql);

    foreach ($role_result as $role) {
        array_push($users, $role->uid);
    }

    $user_list = implode(',', $users);

    return $user_list;
}

function protocol_overview_get_protocol_from_id($protocol_id)
{

    //Diese Variable speichert das Protokoll ab
    $protocol = "";

    //Das Protokoll wird aus der Datenbank geladen
    $sql = "SELECT * FROM {protocol} WHERE protocol_id = $protocol_id";
    $protocol_result = db_query($sql);

    //Das geladene Protokoll wird in die dafür angelegte Variable gespeichert
    foreach ($protocol_result as $pro) {
        $protocol = $pro;
    }

    return $protocol;
}