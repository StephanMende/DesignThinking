<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 03.04.17
 * Time: 12:17
 */

/*
 * Diese Funktion erstellt ein Formular, in das die Gruppe ihre Gruppenreflexion eintragen kann. Dieses Formular
 * enthält eine Textarea und einen Submit-Button
 */
function reflection_get_group_reflection_form($form, &$form_submit)
{

    //Der aktuelle angemeldete Benutzer wird ermittelt
    global $user;

    //die Rollenbezeichnung (Gruppen-ID) wird ermittelt
    $group = end($user->roles);

    //In diese Variable wird nach der Datenbanksuche die Rollen-ID der Gruppe eingetragen
    $role_id = 0;

    $sql = "SELECT * FROM {role} WHERE name = '" . $group . "'";
    $role_result = db_query($sql);

    foreach ($role_result as $role) {
        $role_id = $role->rid;
    }

    //Falls es eine gespeicherte Reflexion gibt, wird diese geladen
    $group_reflection = reflection_get_saved_group_reflection($role_id);

    $form['group_reflection'] = array(
        '#title' => "Gruppenreflexion",
        '#type' => 'fieldset',
        '#description' => "Am Ende der Projektphase soll jede Gruppe ein persönliches Fazit ziehen. Dort können sowohl positive als auch negative Aspekte der Projektphase genannt werden."
    );
    $form['group_reflection']['reflection'] = array(
        '#title' => t('reflection'),
        '#resizable' => FALSE,
        '#default_value' => $group_reflection,
        '#type' => 'textarea',
    );

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Fazit speichern',
        '#submit' => array('reflection_save_group_reflection')

    );

    return $form;
}

/*
 * Diese Funktion liest alle nötigen Felder des Formulares aus und speichert die Reflexion in der Datenbank ab
 */
function reflection_save_group_reflection($form, &$form_state)
{

    //Der aktuelle angemeldete Benutzer wird ermittelt
    global $user;

    //die Rollenbezeichnung (Gruppen-ID) wird ermittelt
    $group = end($user->roles);

    //In diese Variable wird nach der Datenbanksuche die Rollen-ID der Gruppe eingetragen
    $role_id = 0;

    $sql = "SELECT * FROM {role} WHERE name = '" . $group . "'";
    $role_result = db_query($sql);

    foreach ($role_result as $role) {
        $role_id = $role->rid;
    }

    //Die Reflexion wird aus der Textarea geladen
    $reflection = $form_state['values']['reflection'];

    //Falls schon eine gespeicherte Reflexion in der Datenbank vorliegt, wird diese aktualsiert. Falls nicht, wird ein
    //neuer Eintrag erstellt
    if (reflection_check_if_there_is_already_a_group_reflection($role_id)) {
        $sql = "UPDATE {group_reflection} SET reflection = '$reflection' WHERE role_id = " . $role_id;
    } else {
        $sql = "INSERT INTO {group_reflection} (role_id,reflection) VALUES(" . $role_id . ",'" . $reflection . "');";
    }

    db_query($sql);
    drupal_set_message("Reflexion wurde erfolgreich gespeichert!");
    drupal_goto("");
}

/*
 * Diese Funktion überprüft, ob bereits eine gespeicherte Reflexion der Nutzers in der Datenbank vorhanden ist
 */
function reflection_check_if_there_is_already_a_group_reflection($role_id)
{
    $reflection = false;
    $sql = "SELECT * FROM {group_reflection} WHERE role_id = " . $role_id;
    $group_reflection_result = db_query($sql);
    foreach ($group_reflection_result as $group_reflection) {
        $reflection = true;
    }

    return $reflection;
}

/*
 * Diese Funktion lädt die gespeicherte Reflexion der Gruppe aus der Datenbank. Sollte noch keine
 * vorhanden sein, wird ein leerer String zurückgegeben
 */
function reflection_get_saved_group_reflection($role_id)
{
    $reflection = "";
    $sql = "SELECT * FROM {group_reflection} WHERE role_id = " . $role_id;
    $group_reflection_result = db_query($sql);
    foreach ($group_reflection_result as $group_reflectionr) {
        $reflection = $group_reflectionr->reflection;
    }

    return $reflection;
}