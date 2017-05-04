<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 01.11.16
 * Time: 10:19
 */

require_once('test_module_new_method_execution.inc');

function test_module_delete_task_form() {

    $task_id = arg(1);

    if(test_module_check_task_permission($task_id)) {
        $task = "";

        $pw = arg(2);


        $parts = getParticipants2();
        $protocol_id = get_open_protocol2($parts);

        $sql = "SELECT * FROM {task} as t WHERE t.task_id = $task_id AND t.uid IN ($parts)";

        $result = db_query($sql);

        foreach ($result as $r) {
            if ($r->protocol_id == $protocol_id) {

                $sql = "SELECT * FROM {field_data_field_referenznummer} as t WHERE field_referenznummer_value = $r->task_id";
                $temp = db_query($sql);

                foreach($temp as $te) {
                    $task = $te->entity_id;
                }

                node_delete($task);
                $sql = "DELETE FROM {task} WHERE task_id = $r->task_id";
                db_query($sql);
                drupal_set_message("Aufgabe wurde gelöscht!");

            } else {
                $sql = "SELECT * FROM {field_data_field_referenznummer} as t WHERE field_referenznummer_value = $r->task_id";
                $temp = db_query($sql);

                foreach($temp as $te) {
                    $task = $te->entity_id;
                }

                node_delete($task);
                $sql = "UPDATE {task} as t SET done = 1 WHERE t.task_id = $task_id AND t.uid IN ($parts)";
                db_query($sql);
                drupal_set_message("Aufgabe wurde erledigt!");
            }

            if(!strcmp($pw, "pXs8hYVUqe")) {
                drupal_goto('/protokoll_offene_aufgaben');
            } else if(!strcmp($pw, "t8HVhPRlPZ")) {
                drupal_goto('/protokoll_neue_aufgaben');
            } else if(!strcmp($pw, "zqjvEz44Qx")) {
                drupal_goto('/gruppenaufgaben');
            }
        }
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Aufgabe ändern oder löschen zu können! <a href='".$base_url."'>Zurück zum Hauptmenü</a>", "error");
        return "";
    }



}

function test_module_check_task_permission($task_id) {

    $proof = false;
    $task = $task_id;

    $list = getParticipants2();
    $list = explode(',', $list);

    $sql = "SELECT * FROM {task} WHERE task_id = ".$task;
    $result = db_query($sql);

    foreach ($result as $item) {
        $proof = in_array($item->uid, $list);
    }

    return $proof;

}

function test_module_delete_file() {


    $method_id = arg(1);

    if(test_module_check_method_permission($method_id)) {
        $file = "methods/".arg(2);
        $pass = arg(3);

        $sql = "DELETE FROM {method_execution_data} WHERE method_execution_id = $method_id AND path='$file';";
        db_query($sql);
        if (!strcmp($pass, "pXs8hYVUqe")) {
            drupal_set_message(arg(2) . " wurde erfolgreich gelöscht!");
            drupal_goto('/change_protocol_method_information/'.$method_id);
        } else if (!strcmp($pass, "t8HVhPRlPZ")) {
            drupal_goto('/protokoll_uebersicht');

        } else if (!strcmp($pass, "zqjvEz44Qx")) {
            drupal_set_message(arg(2) . " wurde erfolgreich gelöscht!");
            drupal_goto('/change_method_execution/'.$method_id);
        } else if (!strcmp($pass, "zqjvEz44Qxyy")) {
            drupal_set_message(arg(2) . " wurde erfolgreich gelöscht!");
            drupal_goto('protokoll_methode_aendern/'.$method_id.'/pXs8hYVUqe');
        } else {
            drupal_set_message("Ein Weiterleitungsfehler ist aufgetreten. Sie wurden zur Startseite weitergeleitet", "error");
            drupal_goto('');
        }
    } else {
        global $base_url;
        drupal_set_message("Sie besitzen nicht die nötigen Rechte, um die Datei ändern oder löschen zu können! <a href='".$base_url."'>Zurück zum Hauptmenü</a>", "error");
        return "";
    }








}

