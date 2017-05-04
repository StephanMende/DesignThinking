<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.11.16
 * Time: 10:19
 */

require_once('protocol_template_new_method_execution.inc');

/*
 * Diese Funktion löscht eine Aufgabe aus der Datenbank. Die Aufgaben-ID wird über die URL vermittelt
 */
function protocol_template_delete_task_form()
{

    //Die Aufgaben-ID wird aus der URL ausgelesen
    $task_id = arg(1);

    //Es wird überprüft, ob der Nutzer die Berechtigung besitzt, die Aufgabe zu löschen
    if (protocol_template_check_task_permission($task_id)) {

        //Die Gruppenmitglieder des aktuell angemeldeten Nutzers werden geladen
        $parts = protocol_template_get_participants_from_logged_in_user_group();

        //Die Protokoll_ID des aktuell offenen Protokolls der Gruppe wird geladen
        $protocol_id = protocol_template_get_current_open_group_protocol($parts);

        //Die Aufgaben, die im Rahmen des geladenen Protokolls erstellt wurden, werden geladen
        $sql = "SELECT * FROM {task} as t WHERE t.task_id = $task_id AND t.uid IN ($parts)";
        $task_result = db_query($sql);

        foreach ($task_result as $task) {

            //Falls die Protokoll-ID mit der Protokoll-ID der Aufgabe übereinstimmt
            if ($task->protocol_id == $protocol_id) {

                //Das Node-Element zur Aufgabe wird gelöscht
                $sql = "SELECT * FROM {field_data_field_referenznummer} as t WHERE field_referenznummer_value = $task->task_id";
                $data_field_result = db_query($sql);

                $current_task = "";

                foreach ($data_field_result as $node) {
                    $current_task = $node->entity_id;
                }

                node_delete($current_task);

                //Die Aufgabe wird aus der entsprechenden Datenbanktabelle gelöscht
                $sql = "DELETE FROM {task} WHERE task_id = $task->task_id";
                db_query($sql);
                drupal_set_message("Aufgabe wurde gelöscht!");

                //Sollte die Aufgabe keinem Protokoll zugeordnet worden sein
            } else {

                //Das Node-Element zur Aufgabe wird gelöscht
                $sql = "SELECT * FROM {field_data_field_referenznummer} as t WHERE field_referenznummer_value = $task->task_id";
                $data_field_result = db_query($sql);

                $current_task = "";

                foreach ($data_field_result as $node) {
                    $current_task = $node->entity_id;
                }

                node_delete($current_task);

                //Die Aufgabe wird auf erledigt gesetzt
                $sql = "UPDATE {task} as t SET done = 1 WHERE t.task_id = $task_id AND t.uid IN ($parts)";
                db_query($sql);
                drupal_set_message("Aufgabe wurde erledigt!");
            }

            //Es wird ein Passcode aus der URL geladen, um den Nutzer nach Beendigung der Funktion zurück
            //zur vorigen Seite zu führen
            $user_pw = arg(2);

            //Der Nutzer wird auf die vorige Seite zurückgeführt
            if (!strcmp($user_pw, "pXs8hYVUqe")) {
                drupal_goto('/protokoll_offene_aufgaben');
            } else if (!strcmp($user_pw, "t8HVhPRlPZ")) {
                drupal_goto('/protokoll_neue_aufgaben');
            } else if (!strcmp($user_pw, "zqjvEz44Qx")) {
                drupal_goto('/gruppenaufgaben');
            }
        }
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Aufgabe ändern oder löschen zu können! <a href='" . $base_url . "'>Zurück zum Hauptmenü</a>", "error");
        return "";
    }


}

/*
 * Diese Funktion überprüft, ob der Nutzer die Berechtigung besitzt, die Aufgabe zu verändern. Die ID der Aufgabe
 * wird als Parameter übergeben
 */
function protocol_template_check_task_permission($task_id)
{
    //Diese Variable speichert ab, ob der Nutzer die Rechte besitzt oder nicht
    $proof = false;

    //Die Aufgabe wird aus dem Parameter ausgelesen
    $task = $task_id;

    $user_list = protocol_template_get_participants_from_logged_in_user_group();
    $user_list = explode(',', $user_list);

    //Die Aufgabe wird aus der Datenbank geladen
    $sql = "SELECT * FROM {task} WHERE task_id = " . $task;
    $task_result = db_query($sql);

    foreach ($task_result as $item) {

        //Sollte sich die ID des zugewiesenn Nutzers der Aufgabe mit der ID eines der Gruppenmitglieder gleichen
        //wird die Berechtigung erteilt
        $proof = in_array($item->uid, $user_list);
    }

    return $proof;

}

/*
 * Diese Funktion löscht eine hochgeladene Datei aus der Datenbank. Der Name der Datei wird über die URL vermittelt
 */
function protocol_template_delete_file()
{

    //Die ID der betreffenden Methodendurchführung wird aus der URL ausgelesen
    $method_id = arg(1);

    //Es wird überprüft, ob der angemeldete Nutzer die Berechtigung besitzt, die Methode zu verändern
    if (protocol_template_check_method_permission($method_id)) {

        //Der Pfad zur Datei wird ermittelt
        $file = "methods/" . arg(2);

        //Die Datei wird aus der Datenbank gelöscht
        $sql = "DELETE FROM {method_execution_data} WHERE method_execution_id = $method_id AND path='$file';";
        db_query($sql);

        //Es wird ein Passcode aus der URL geladen, um den Nutzer nach Beendigung der Funktion zurück
        //zur vorigen Seite zu führen
        $pass = arg(3);

        //Der Nutzer wird auf die vorige Seite zurückgeführt
        if (!strcmp($pass, "pXs8hYVUqe")) {
            drupal_set_message(arg(2) . " wurde erfolgreich gelöscht!");
            drupal_goto('/change_protocol_method_information/' . $method_id);

        } else if (!strcmp($pass, "t8HVhPRlPZ")) {
            drupal_goto('/protokoll_uebersicht');

        } else if (!strcmp($pass, "zqjvEz44Qx")) {
            drupal_set_message(arg(2) . " wurde erfolgreich gelöscht!");
            drupal_goto('/change_method_execution/' . $method_id);

        } else if (!strcmp($pass, "zqjvEz44Qxyy")) {
            drupal_set_message(arg(2) . " wurde erfolgreich gelöscht!");
            drupal_goto('protokoll_methode_aendern/' . $method_id . '/pXs8hYVUqe');

        } else {
            drupal_set_message("Ein Weiterleitungsfehler ist aufgetreten. Sie wurden zur Startseite weitergeleitet", "error");
            drupal_goto('');
        }
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Datei ändern oder löschen zu können! <a href='" . $base_url . "'>Zurück zum Hauptmenü</a>", "error");
        return "";
    }
}

