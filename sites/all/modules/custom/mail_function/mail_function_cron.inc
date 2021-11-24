<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 13.04.17
 * Time: 18:22
 */

/*
 * Diese Funktion wird aufgerufen, sobald das System den üblichen Cron-Job ausführt. Es werden alle Aufgaben geladen,
 * die noch offen sind. Im Anschluss wird die Zei bis zur Deadline berechnet und die entsprechenden Nutzer per Mail
 * darüber informiert.
 */
function mail_function_cron()
{

    //Alle noch offenen Aufgaben werden ermittelt.
    $open_tasks = mail_function_get_all_open_tasks();

    //Es wird über alle Aufgaben iteriert.
    foreach ($open_tasks as $open_task) {

        //Nutzername wird ermittelt
        $user = user_load($open_task->uid);
        $username = $user->name;

        //Zeit bis zur Deadline wird ermittelt
        $today = new DateTime();
        $deadline = new DateTime($open_task->deadline);
        $interval = $today->diff($deadline);
        $days = $interval->format("%r%a");
        $hours = $interval->format("%r%h");
        $time_until_deadline = ($days * 24) + ($hours);

        //Daten für ein Mail-Objekt werden ermittelt
        $from = "noreply@dtt.de";
        $to = "kortumj@uni-hildesheim.de";
        $cc = "";
        $subject = "offene Aufgabe";
        $body = "Hallo " . $username . "!  \r\n Ihre Aufgabe mit der Kurzbeschreibung \r\n\r\n " . $open_task->short_description . "\r\n\r\n läuft in " . $interval->format("%d Tag(en) und %h Stunde(n)") . " ab! \r\n Diese Mail wurde automatisch generiert. \r\n Mit freundlichen Grüßen
        \r\nIhr Design-Thinking-Bot";


        //Sind es noch weniger als 72 Stunden bis zur Deadline...
        if ($time_until_deadline < 72) {
            //Wurde die erste Erinnerung bereits versendet?
            if ($open_task->reminder3 != 1) {
                //Wenn nicht, dann schicke die erste Erinnerung
                $sql = "UPDATE {task} SET reminder3 = 1 WHERE task_id = $open_task->task_id";
                db_query($sql);
                mail_function_send_personal_mail($from, $to, $cc, $subject, $body);

                //Wenn ja, sind es noch weniger als 24 Stunden bis zur Deadline?
            } else if ($time_until_deadline < 24) {
                //Wenn ja, wurde die zweite Erinnerung schon verschickt?
                if ($open_task->reminder1 != 1) {
                    $sql = "UPDATE {task} SET reminder1 = 1 WHERE task_id = $open_task->task_id";
                    db_query($sql);
                    mail_function_send_personal_mail($from, $to, $cc, $subject, $body);
                }
            }
        }
    }
}

/*
 * Diese Funktion liest alle offenen Aufgaben aus der Datenbank aus.
 */
function mail_function_get_all_open_tasks()
{
    $sql = "SELECT * FROM {task} WHERE done = 0";
    $open_task_result = db_query($sql);
    return $open_task_result;
}