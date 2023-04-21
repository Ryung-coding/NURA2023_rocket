var n = 40,
    random = d3.random.normal(0, 100),
    data = d3.range(n).map(random);

var margin = {top: 20, right: 20, bottom: 50, left: 50},
    width = 300 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

var p = d3.scale.log()
    .domain([1000, 100])
    .range([0, height]);

var T = d3.scale.linear()
    .domain([-100, 40])
    .range([height, 0]);

var x = d3.scale.linear()
    .domain([-40, 60])
    .range([0, width]);

var y = d3.scale.log()
    .domain([1000, 100])
    .range([0, height]);


var line = d3.svg.line()
    .x(function(d) { return x(d[0]); })
    .y(function(d) { return y(d[1]); });

var svg = d3.select("body").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
  .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

svg.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(p).orient("left"))
  .append("text")
    .attr("transform", "rotate(-90)")
    .attr("y", 6)
    .attr("dy", ".71em")
    .style("text-anchor", "end")
    .text("Pressure (hPa)");

svg.append("g")
    .attr("class", "y axis")
    .attr("transform", "translate(" + width + ",0)")
    .call(d3.svg.axis().scale(T).orient("right"))
  .append("text")
    .attr("transform", "rotate(90)")
    .attr("y", -6)
    .attr("dy", ".71em")
    .style("text-anchor", "end")
    .text("Temperature (°C)");

svg.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + y(1000) + ")")
    .call(d3.svg.axis().scale(x).orient("bottom"))
  .append("text")
    .attr("x", width / 2)
    .attr("y", 40)
    .attr("dy", ".71em")
    .style("text-anchor", "middle")
    .text("Dewpoint (°C)");

var path = svg.append("path")
    .datum(data)
    .attr("class", "line")
    .attr("d", line);

tick();

function tick() {
    data.push([Math.random() * 100 - 40, Math.random() * 900 + 100]);
    path.attr("d", line)
        .attr("transform", null)
      .transition()
        .duration(500)
        .ease("linear")
        .attr("transform", "translate(" + x(-1) + ",0)")
        .each("end", tick);
    data.shift();
}
