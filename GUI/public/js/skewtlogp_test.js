// 배열 예시입니다. 실제 데이터에 맞게 변경해주세요.
let tempArray = [10, 12, 14, 16, 18, 20, 22, 24, 26, 28];
let pressureArray = [1000, 900, 800, 700, 600, 500, 400, 300, 200, 100];

// tempArray와 pressureArray에서 데이터를 가져와서 객체로 만듭니다.
let SLdata = tempArray.map((temp, i) => ({ temp, pressure: pressureArray[i] }));

// SVG 설정
var margin3 = { top: 50, right: 50, bottom: 50, left: 50 };
var width3 = 800 - margin3.left - margin3.right;
var height3 = 600 - margin3.top - margin3.bottom;
var svg3 = d3.select("#skewt-logp").append("svg")
            .attr("width", width3 + margin3.left + margin3.right)
            .attr("height", height3 + margin3.top + margin3.bottom)
            .append("g")
            .attr("transform", "translate(" + margin3.left + "," + margin3.top + ")");

// x와 y의 scale 설정
var xScale3 = d3.scaleLinear().domain([-50, 50]).range([0, width3]);
var yScale3 = d3.scaleLog().domain([1050, 100]).range([height3, 0]);

// x축과 y축 설정
var xAxis3 = d3.axisBottom(xScale3);
var yAxis3 = d3.axisLeft(yScale3);

// x축 그리기
svg3.append("g")
    .attr("transform", "translate(0," + height3 + ")")
    .call(xAxis3);

// y축 그리기
svg3.append("g")
    .call(yAxis3);

// 점 그리기
svg3.selectAll("dot")
    .data(SLdata)
    .enter()
    .append("circle")
    .attr("r", 5) // 점의 크기를 설정합니다.
    .attr("cx", function(d) { return xScale3(d.temp); })
    .attr("cy", function(d) { return yScale3(d.pressure); })
    .style("fill", "#ff0000"); // 점의 색깔을 설정합니다.
