<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 03.04.17
 * Time: 12:17
 */

function reflection_group_get_form($form, &$form_submit) {
    global $user;

    $group = end($user->roles);
    $role_id = 0;


    $sql = "SELECT * FROM {role} WHERE name = '".$group."'";
    $result = db_query($sql);

    foreach ($result as $item) {
        $role_id = $item->rid;
    }


    $reflection = get_current_group_reflection($role_id);

    $form['test'] = array(
        '#title' => "Mail versenden",
        '#type' => 'fieldset',
        '#description' => "Am Ende der Projektphase soll jede Gruppe ein persönliches Fazit ziehen. Dort können sowohl positive als auch negative Aspekte der Projektphase genannt werden."
    );
    $form['test']['task'] = array(
        '#title' => t('Fazit'),
        '#resizable' => FALSE,
        '#default_value' => $reflection,
        '#type' => 'textarea',
    );

    $form['save_reflection'] = array(
        '#type' => 'submit',
        '#value' => 'Fazit speichern',
        '#submit' => array('group_reflection_save_reflection')

    );

    return $form;
}

function group_reflection_save_reflection($form, &$form_state) {

    global $user;

    $group = end($user->roles);
    $role_id = 0;


    $sql = "SELECT * FROM {role} WHERE name = '".$group."'";
    $result = db_query($sql);

    foreach ($result as $item) {
        $role_id = $item->rid;
    }

    $task = $form_state['values']['task'];

    if (check_current_group_reflection($role_id)) {
        $sql = "UPDATE {group_reflection} SET reflection = '$task' WHERE role_id = ".$role_id;
    } else {
        $sql = "INSERT INTO {group_reflection} (role_id,reflection) VALUES(".$role_id.",'".$task."');";
    }

    db_query($sql);
    drupal_set_message("Reflexion wurde erfolgreich gespeichert!");
    drupal_goto("");
}

function check_current_group_reflection($role_id) {
    $reflection = false;
    $sql = "SELECT * FROM {group_reflection} WHERE role_id = ".$role_id;
    $result = db_query($sql);
    foreach ($result as $r) {
        $reflection = true;
    }

    return $reflection;
}

function get_current_group_reflection($role_id) {
    $reflection = "";
    $sql = "SELECT * FROM {group_reflection} WHERE role_id = ".$role_id;
    $result = db_query($sql);
    foreach ($result as $r) {
        $reflection = $r->reflection;
    }

    return $reflection;
}