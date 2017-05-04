<?php
/**
 * Created by PhpStorm.
 * User: jonaskortum
 * Date: 29.04.17
 * Time: 17:11
 */

function statistiken_create_pie_chart($title, $value)
{

    $testarray = array_keys($value);

    $testString = "[";

    $id = 0;
    foreach ($value as $item) {

        $color = "black";
        if ($item == 0) {
            $color = "white";
        }


        $testString .= '{"label":"' . $testarray[$id] . '", "value":' . $item . ', "color":"' . $color . '"},';
        $id++;
    }

    $testString = substr($testString, 0, -1);
    $testString .= "];";


    $html = '<script src="http://d3js.org/d3.v3.min.js"></script>';
    $html .= '

    <style>
        svg {
	        width: 960px;
            height: 500px;
            display: block;
            margin: 0 auto;
        }

        .svg text {
	        fill: black;
	        font: 10px sans-serif;
	        font-weight: bold;
	        text-anchor: start;
        }

        path.slice{
	        stroke-width:2px;
        }

        polyline{
	        opacity: .3;
	        stroke: black;
	        stroke-width: 2px;
	        fill: none;
        }

        .labelValue{
	        font-size: 60%;
	        opacity: .5;
        }

        .toolTip {
            font-family: "Helvetica Neue", Helvetica, Arial, sans-serif;
            position: absolute;
            display: none;
            width: auto;
            height: auto;
            background: none repeat scroll 0 0 white;
            border: 0 none;
            border-radius: 8px 8px 8px 8px;
            box-shadow: -3px 3px 15px #888888;
            color: black;
            font: 12px sans-serif;
            padding: 5px;
            text-align: center;
        }

        text {
            font: 12px sans-serif;
        }
    </style>';


    $html .= '

    <script>


        d3.select("input[value=\"total\"]").property("checked", true);

        var svg = d3.select("body")
	        .append("svg")
	        .append("g")

        svg.append("g")
	        .attr("class", "slices");
        svg.append("g")
	        .attr("class", "labelName");
        svg.append("g")
	        .attr("class", "labelValue");
        svg.append("g")
	        .attr("class", "lines");

        var width = 960,
            height = 450,
	        radius = Math.min(width, height) / 2;

        var pie = d3.layout.pie()
	        .sort(null)
	        .value(function(d) {
		    return d.value;
	    });

        var arc = d3.svg.arc()
	        .outerRadius(radius * 0.8)
	        .innerRadius(radius * 0.4);

        var outerArc = d3.svg.arc()
	        .innerRadius(radius * 0.9)
	        .outerRadius(radius * 0.9);

        var legendRectSize = (radius * 0.05);
        var legendSpacing = radius * 0.02;


        var div = d3.select("body").append("div").attr("class", "toolTip");

        svg.attr("transform", "translate(" + width / 2 + "," + height / 2 + ")");

        var colorRange = d3.scale.category20();
        var color = d3.scale.ordinal()
	        .range(colorRange.range());


        datasetTotal = ' . $testString . '


        change(datasetTotal);


        function change(data) {

	        var slice = svg.select(".slices").selectAll("path.slice")
                .data(pie(data), function(d){ return d.data.label });

            slice.enter()
                .insert("path")
                .style("fill", function(d) { return color(d.data.label); })
                .attr("class", "slice");

            slice
                .transition().duration(1000)
                .attrTween("d", function(d) {
                    this._current = this._current || d;
                    var interpolate = d3.interpolate(this._current, d);
                    this._current = interpolate(0);
                    return function(t) {
                        return arc(interpolate(t));
                    };
            })

            slice
                .on("mousemove", function(d){
                    div.style("left", d3.event.pageX+10+"px");
                    div.style("top", d3.event.pageY-25+"px");
                    div.style("display", "inline-block");
                    div.html((d.data.label)+"<br>"+(d.data.value));
            });

            slice
                .on("mouseout", function(d){
                    div.style("display", "none");
            });

            slice.exit()
                .remove();

            var legend = svg.selectAll(\'.legend\')
                .data(color.domain())
                .enter()
                .append(\'g\')
                .attr(\'class\', \'legend\')
                .attr(\'transform\', function(d, i) {
                    var height = legendRectSize + legendSpacing;
                    var offset =  height * color.domain().length / 2;
                    var horz = -3 * legendRectSize;
                    var vert = i * height - offset;
                    return \'translate(\' + horz + \',\' + vert + \')\';
            });

            legend.append(\'rect\')
                .attr(\'width\', legendRectSize)
                .attr(\'height\', legendRectSize)
                .style(\'fill\', color)
                .style(\'stroke\', color);

            legend.append(\'text\')
                .attr(\'x\', legendRectSize + legendSpacing)
                .attr(\'y\', legendRectSize - legendSpacing)
                .text(function(d) { return d; });

            var text = svg.select(".labelName").selectAll("text")
                .data(pie(data), function(d){ return d.data.label });

            text.enter()
                .append("text")
                .attr("dy", ".35em")
                .style("fill", function(d) { return "red"; })
                .text(function(d) {
                    return (d.data.label+": "+d.value);
            });

            function midAngle(d){
                return d.startAngle + (d.endAngle - d.startAngle)/2;
            }

            text
                .transition().duration(1000)
                .attrTween("transform", function(d) {
                    this._current = this._current || d;
                    var interpolate = d3.interpolate(this._current, d);
                    this._current = interpolate(0);
                    return function(t) {
                        var d2 = interpolate(t);
                        var pos = outerArc.centroid(d2);
                        pos[0] = radius * (midAngle(d2) < Math.PI ? 1 : -1);
                        return "translate("+ pos +")";
                    };
            })
                .styleTween("text-anchor", function(d){
                    this._current = this._current || d;
                    var interpolate = d3.interpolate(this._current, d);
                    this._current = interpolate(0);
                    return function(t) {
                        var d2 = interpolate(t);
                        return midAngle(d2) < Math.PI ? "start":"end";
                    };
                })
                .style("fill", function(d) {
                    return (d.data.color);
                })
                .text(function(d) {
                    return (d.data.label+": "+d.value);
                });


            text.exit()
                .remove();

            var polyline = svg.select(".lines").selectAll("polyline")
                .data(pie(data), function(d){ return d.data.label });

            polyline.enter()
                .append("polyline");

            polyline.transition().duration(1000)
                .attrTween("points", function(d){
                    this._current = this._current || d;
                    var interpolate = d3.interpolate(this._current, d);
                    this._current = interpolate(0);
                    return function(t) {
                        var d2 = interpolate(t);
                        var pos = outerArc.centroid(d2);
                        pos[0] = radius * 0.95 * (midAngle(d2) < Math.PI ? 1 : -1);
                        return [arc.centroid(d2), outerArc.centroid(d2), pos];
                    };
                });

            polyline.exit()
                .remove();
        };

        svg.append("text")
                .attr("x", 0)
                .attr("y", 0-height/2+15)
                .attr("text-anchor", "middle")
                .style("font-size", "16px")
                .style("text-decoration", "underline")
                .text("' . $title . '");

    </script>';

    return $html;
}