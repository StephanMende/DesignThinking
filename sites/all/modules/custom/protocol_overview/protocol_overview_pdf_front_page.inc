<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 28.04.17
 * Time: 17:43
 */

/*
 * Diese Funktion baut das Titelblatt der vollständigen Dokumentation
 */
function protocol_overview_pdf_front_page($group_name, $names_of_participants, $date_of_today)
{

    $front_page_string = '

        <style>
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
        <br>
        <br>

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
        <br>
        <br>

        <h1 style="font-size: 3em;" align="center">Dokumentation ' . $group_name . '</h1>
        <br>
        <br>
        <br>

    ' . $names_of_participants . '

    <h3 align="center">' . $date_of_today . '</h3>';

    return $front_page_string;
}