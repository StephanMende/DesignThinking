<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 02.11.16
 * Time: 12:34
 */

function task_reminder_show_tasks($form, &$form_submit) {

    $id = arg(1);

    $test = get_table_for_tasks($id);

    $form['form_item'] = array(
        '#type' => 'markup',
        '#title' => t('My Form Item'),
        '#prefix' => $test
    );


    return $form;
}

function get_table_for_tasks($id) {


    $tempString = "<table><th>Kurzbezeichnung</th><th>zugehöriger Benutzer (Wer?)</th> <th> Aufgabe (Was?)</th> <th>Deadline (Wann?)</th><th>Aktionen</th>";


    $sql = "SELECT * FROM {task} WHERE task_id = $id";
    $result = db_query($sql);

    foreach($result as $pro) {

        $user=user_load($pro->uid);
        $username=$user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'];

        $date = new DateTime($pro->deadline);
        $deadline2 = $date->format('d.m.Y');
        $deadline3 = $date->format('d.m.Y H:i:s') . " Uhr";


        $tempString = $tempString . "<tr><td>$pro->short_description</td><td>$username</td><td>$pro->task</td><td>$deadline3</td><td><a href='/design_thinking_tool/delete_task/$pro->task_id'>erledigt</a></td></tr></tr>";

    }

    $tempString .= "</table>";



    return $tempString;

}


