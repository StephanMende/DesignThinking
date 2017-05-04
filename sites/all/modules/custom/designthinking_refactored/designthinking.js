/**
 * Created by hofer on 13.10.2015.
 */



(function ($) {
    Drupal.behaviors.designthinking = {
        attach: function (context) {
            window.onload = function () {

                //Get block
                //var block = document.getElementById('block-designthinking-refactored-methods');
                //block.style.marginLeft = "0px";

                //var content = document.getElementById('content');
                //content.style.width = "40%";


                //Get the object by ID
                var a = document.getElementById('methodengrafik_test');


                //Get the svg document inside the object tag
                var svgDoc = a.contentDocument;


                //Get one of the SVG items by ID
                var svgItem = svgDoc.getElementById('svg_2');
                var svgIcon = svgDoc.getElementById('taschenlampe');

                //Get one of the SVG items by ID
                var svgItem_2 = svgDoc.getElementById('svg_3');
                var svgIcon_2 = svgDoc.getElementById('fragezeichen');

                //Get one of the SVG items by ID
                var svgItem_3 = svgDoc.getElementById('svg_4');
                var svgIcon_3 = svgDoc.getElementById('glühbirne');

                //Get one of the SVG items by ID
                var svgItem_4 = svgDoc.getElementById('svg_5');
                var svgIcon_4 = svgDoc.getElementById('ausrufezeichen');

                //Get one of the SVG items by ID
                var svgItem_5 = svgDoc.getElementById('svg_6');
                var svgIcon_5 = svgDoc.getElementById('baustein');

                //Get one of the SVG items by ID
                var svgItem_6 = svgDoc.getElementById('svg_7');
                var svgIcon_6 = svgDoc.getElementById('haken');

                //Get one oht the SVG items by ID
                var svgItem_7 = svgDoc.getElementById('svg_9');
                var svgIcon_7 = svgDoc.getElementById('vorbereitung');
                //Get one oht the SVG items by ID
                var svgItem_8 = svgDoc.getElementById('svg_10');
                var svgIcon_8 = svgDoc.getElementById('projektmanagement');

                // Set the color to something else

                svgItem.onclick = function () {
                    //alert('Phase1 wurde angeklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_1',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert(data);
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 1. Phase</h2>');
                            //alert(data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "yellow");

                    var a = document.getElementById('methodengrafik_test');

                    var svgIcon = svgDoc.getElementById('taschenlampe');

                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");

                }

                svgIcon.onclick = function () {
                    //alert('Phase1 wurde angeklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_1',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 1. Phase</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "yellow");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");

                }

                //svgItem.onmouseout = function() {
                //    svgItem.setAttribute("fill","#58ACFA");
                //}
                //
                //svgIcon.onmouseout = function() {
                //    svgItem.setAttribute("fill","#58ACFA");
                //}
                //-------------------------------------------


                // Set the color to something else

                svgItem_2.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_2',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 2. Phase</h2>' + data);

                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "yellow");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                    //window.location.reload();


                }

                svgIcon_2.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_2',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 2. Phase</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "yellow");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }



                //svgItem_2.onmouseout = function() {
                //    svgItem_2.setAttribute("fill","#58ACFA");
                //}
                //
                //svgIcon_2.onmouseout = function() {
                //    svgItem_2.setAttribute("fill","#58ACFA");
                //}

                //------------------------------------------------

                // Set the color to something else

                svgItem_3.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_3',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 3. Phase</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "yellow");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }

                svgIcon_3.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_3',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 3. Phase</h2>' + data);
                            //location.reload();
                        }
                    });

                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "yellow");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");

                }

                //svgItem_3.onmouseout = function() {
                //    svgItem_3.setAttribute("fill","#58ACFA");
                //}
                //
                //svgIcon_3.onmouseout = function() {
                //    svgItem_3.setAttribute("fill","#58ACFA");
                //}

                //------------------------------------------------

                // Set the color to something else

                svgIcon_4.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_4',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 4. Phase</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "yellow");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }

                svgItem_4.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_4',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 4. Phase</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "yellow");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }
                //svgItem_4.onmouseout = function() {
                //    svgItem_4.setAttribute("fill","#58ACFA");
                //}
                //
                //svgIcon_4.onmouseout = function() {
                //    svgItem_4.setAttribute("fill","#58ACFA");
                //}

                //------------------------------------------------

                // Set the color to something else

                svgItem_5.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_5',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 5. Phase</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "yellow");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }

                svgIcon_5.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_5',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 5. Phase</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "yellow");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }

                //svgItem_5.onmouseout = function() {
                //    svgItem_5.setAttribute("fill","#58ACFA");
                //}
                //
                //svgIcon_5.onmouseout = function() {
                //    svgItem_5.setAttribute("fill","#58ACFA");
                //}

                //------------------------------------------------

                // Set the color to something else

                svgItem_6.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_6',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der 6. Phase</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "yellow");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }

                svgIcon_6.onclick = function () {
                    //alert('Phase2 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/phase_6',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden  der 6. Phase</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "yellow");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }
                //svgItem_6.onmouseout = function() {
                //    svgItem_6.setAttribute("fill","#58ACFA");
                //}
                //
                //svgIcon_6.onmouseout = function() {
                //    svgItem_6.setAttribute("fill","#58ACFA");
                //}

                //------------------------------------------------



                svgItem_7.onclick = function () {
                    //alert('Phase7 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/vorbereitung',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der Vorbereitung</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "yellow");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }

                svgIcon_7.onclick = function () {
                    //alert('Phase7 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/vorbereitung',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden der Vorbereitung</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "yellow");
                    svgItem_8.setAttribute("fill", "#58ACFA");
                }



                //svgItem_7.onmouseout = function() {
                //    svgIcon_7.style.opacity = 1.0;
                //    svgItem_7.setAttribute("fill","#58ACFA");
                //}
                //
                //svgIcon_7.onmouseout = function() {
                //    svgItem_7.setAttribute("fill","#58ACFA");
                //}

                //------------------------------------------------



                svgItem_8.onclick = function () {
                    //alert('Phase7 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/projektmanagement',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden des Projektmanagements</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "yellow");
                }

                svgIcon_8.onclick = function () {
                    //alert('Phase7 wurde angecklickt!');
                    $.ajax({
                        //Request ajax/username for page callback
                        url: 'ajax/projektmanagement',
                        success: function (data) {
                            //alert('Test success!!!');
                            //alert('Phase1 wurde angecklickt!');
                            //Change site name to a current user name
                            //$('#site-name a span').html(data + '.huhu');
                            $('#block-designthinking-refactored-methods').html('<h2>Methoden des Projektmanagements</h2>' + data);
                            //location.reload();
                        }
                    });
                    svgItem.setAttribute("fill", "#58ACFA");
                    svgItem_2.setAttribute("fill", "#58ACFA");
                    svgItem_3.setAttribute("fill", "#58ACFA");
                    svgItem_4.setAttribute("fill", "#58ACFA");
                    svgItem_5.setAttribute("fill", "#58ACFA");
                    svgItem_6.setAttribute("fill", "#58ACFA");
                    svgItem_7.setAttribute("fill", "#58ACFA");
                    svgItem_8.setAttribute("fill", "yellow");
                }

                //svgItem_8.onmouseout = function() {
                //    svgItem_8.setAttribute("fill","#58ACFA");
                //}
                //
                //svgIcon_8.onmouseout = function() {
                //    svgItem_8.setAttribute("fill","#58ACFA");
                //}

            }
        }
    }
})(jQuery);

