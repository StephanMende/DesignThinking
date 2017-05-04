/**
 * Created by jonaskortum on 15.03.16.
 */
function name ($) {
    Drupal.behaviors.url_parameter = {
        attach: function (context) {
            window.onload = function () {
                alert('Hallo Welt');

            }
            }
        }
    }