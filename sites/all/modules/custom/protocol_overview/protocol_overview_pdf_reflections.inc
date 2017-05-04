<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 29.04.17
 * Time: 16:11
 */

/*
 * Diese Funktion baut die Anzeige der persönlichen Nutzer-Reflexion
 */
function protocol_overview_pdf_user_reflection($user, $reflection)
{

    //Die Überschrift der Seite wird erstellt
    $reflection_string =
        '<h2 style="text-align:center">Reflexion von ' . $user->field_first_name['und'][0]['value'] . " " . $user->field_last_name['und'][0]['value'] . '</h2>';


    //Die übergebene Reflexions-Variable wird hinzugefügt
    $reflection_string .= '
        <p>' . $reflection->reflection . '</p>';

    return $reflection_string;
}

/*
 * Diese Funktion baut die Anzeige der Gruppenreflexion
 */
function protocol_overview_pdf_group_reflection($reflection)
{

    //Die Überschrift der Seite wird erstellt
    $reflection_string = '<h2 style="text-align:center">Reflexion der gesamten Gruppe</h2>';

    //Die übergebene Reflexions-Variable wird hinzugefügt
    $reflection_string .= '<p>' . $reflection->reflection . '</p>';

    return $reflection_string;

}