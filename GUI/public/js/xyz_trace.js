var count = 2000,
    data = [],
    margin = {top: 10, right: 10, bottom: 10, left: 40},
    width = 400, //- margin.left - margin.right,
    height = 300; //- margin.top - margin.bottom;

var xScale = d3.scale.linear()
    .domain([count-2000, count])
    .range([0, width]);

var yScale = d3.scale.linear()
    .domain([-300, 300])
    .range([height, 0]);

var lineGenerator = d3.svg.line()
    .x(function(d, i) { return xScale(d.x); })
    .y(function(d, i) { return yScale(d.y); });

    var svg = d3.select("#chart").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
  .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

svg.append("defs").append("clipPath")
    .attr("id", "clip")
  .append("rect")
    .attr("width", width)
    .attr("height", height);

svg.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + yScale(0) + ")")
    .call(d3.svg.axis().scale(xScale).orient("bottom"));

svg.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(yScale).orient("left"));

var path = svg.append("g")
    .attr("clip-path", "url(#clip)")
  .append("path")
    .attr("class", "line");

tick();

function tick() {
  
  // add new data point to the back
  var newData = { x: count-2000, y: 200 * Math.sin((count-2000)/500) };
  data.push(newData);
  
  // redraw the line
  path.datum(data)
    .attr("d", lineGenerator);
  
  // shift x axis to the left
  xScale.domain([count-4000, count-2000]);
  svg.select(".x.axis")
    .transition()
    .duration(50)
    .ease("linear")
    .call(d3.svg.axis().scale(xScale).orient("bottom"));
  
  // pop the old data point off the front
  if (data.length > 2000) {
    data.shift();
  }
  
  // schedule the next tick
  setTimeout(tick, 10);
  
  // increment count
  count++;
  
}
