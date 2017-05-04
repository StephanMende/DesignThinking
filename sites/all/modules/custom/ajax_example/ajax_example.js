/**
 * Created by hofer on 09.10.2015.
 */
/**
(function ($){
 Drupal.behaviors.ajax_example = {
     attach:function (context) {

         //If the site name is present set it to the username
         if($('#site-name', context).length) {
             $.ajax({
                url: 'ajax/username',
                 success: function(data){
                     //alert('Test success!!!');
                     //Change site name to a current user name
                     $('#site-name a span').html(data + '.com');
                 }
             });
         }
     }
 }
})(jQuery);
**/
(function ($) {
    Drupal.behaviors.ajax_example2 = {
        attach: function (context) {
            window.onload = function () {
                //Get the object by ID
                var a = document.getElementById('methodengrafik_test');
                //Get the svg document inside the object tag
                var svgDoc = a.contentDocument;
                //Get one of the SVG items by ID
                var svgItem = svgDoc.getElementById('svg_2');
                // Set the color to something else
                svgItem.setAttribute("fill", "yellow");
                svgItem.onmouseover = function () {
                    //alert('Phase1 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/username',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-ajax-example-0').html('<h2>Methoden</h2>' + data);
                            //location.reload();
                        }
                    });
                }
            }
        }
    }
})(jQuery);
