var socket = io();

var raw_obj;
var modify_obj;

var points = [];
var airFlowData = [];
var line = null;
const THREE = window.THREE;
var origin_lonlat = [127.07755612, 37.63300324]; //[127.207996, 34.610]; 고흥쪽 좌표
var altitude_diff = 125;
let frameCounter = 0;


var lx = document.getElementById('lx');
var ly = document.getElementById('ly');
var lz = document.getElementById('lz');
var pitch = document.getElementById('pitch');
var yaw = document.getElementById('yaw');
var roll = document.getElementById('roll');
var ex = document.getElementById('ex');
var ey = document.getElementById('ey');
var t = document.getElementById('t');
var h = document.getElementById('h');
var at = document.getElementById('at');
var form = document.getElementById('form');
var input = document.getElementById('input');

//그래프 시작
var count = 2000,
    data = [],
    margin = {top: 10, right: 10, bottom: 10, left: 40},
    width = 400, //- margin.left - margin.right,
    height = 180; //- margin.top - margin.bottom;

var xScale = d3.scale.linear()
    .domain([count-2000, count])
    .range([0, width]);

var yScale = d3.scale.linear()
    .domain([-400, 400])
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
    .call(d3.svg.axis().scale(xScale).orient("bottom"))
    .append("text") // x축 이름 추가
    .attr("class", "axis-label")
    .attr("x", width/1.15)
    .attr("y", 30)
    .text("(1/10 Second)");

svg.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(yScale).orient("left"))
    .append("text") // y축 이름 추가
    .attr("class", "axis-label")
    .attr("transform", "rotate(-90)")
    .attr("x", -height / 1.8)
    .attr("y", -30)
    .text("(meter)");

var path = svg.append("g")
    .attr("clip-path", "url(#clip)")
  .append("path")
    .attr("class", "line");
//그래프1 끝
//그래프2 시작
var count1 = 2000,
    data1 = [],
    margin1 = {top: 10, right: 10, bottom: 10, left: 40},
    width1 = 400, //- margin.left - margin.right,
    height1 = 180; //- margin.top - margin.bottom;

var xScale1 = d3.scale.linear()
    .domain([count1-2000, count1])
    .range([0, width1]);

var yScale1 = d3.scale.linear()
    .domain([-400, 400])
    .range([height1, 0]);

var lineGenerator1 = d3.svg.line()
    .x(function(d, i) { return xScale1(d.x); })
    .y(function(d, i) { return yScale1(d.y); });

    var svg1 = d3.select("#chart1").append("svg")
    .attr("width", width1 + margin1.left + margin1.right)
    .attr("height", height1 + margin1.top + margin1.bottom)
  .append("g")
    .attr("transform", "translate(" + margin1.left + "," + margin1.top + ")");

svg1.append("defs").append("clipPath")
    .attr("id", "clip1")
  .append("rect")
    .attr("width", width1)
    .attr("height", height1);

svg1.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + yScale(0) + ")")
    .call(d3.svg.axis().scale(xScale1).orient("bottom"))
    .append("text") // x축 이름 추가
    .attr("class", "axis-label")
    .attr("x", width/1.15)
    .attr("y", 30)
    .text("(1/10 Second)");

svg1.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(yScale1).orient("left"))
    .append("text") // y축 이름 추가
    .attr("class", "axis-label")
    .attr("transform", "rotate(-90)")
    .attr("x", -height / 1.8)
    .attr("y", -30)
    .text("(meter)");

var path1 = svg1.append("g")
    .attr("clip-path", "url(#clip1)")
  .append("path")
    .attr("class", "line");
//그래프2 끝
//그래프3 시작
var count2 = 2000,
    data2 = [],
    margin2 = {top: 10, right: 10, bottom: 10, left: 40},
    width2 = 400, //- margin.left - margin.right,
    height2 = 180; //- margin.top - margin.bottom;

var xScale2 = d3.scale.linear()
    .domain([count2-2000, count2])
    .range([0, width2]);

var yScale2 = d3.scale.linear()
    .domain([-400, 400])
    .range([height2, 0]);

var lineGenerator2 = d3.svg.line()
    .x(function(d, i) { return xScale2(d.x); })
    .y(function(d, i) { return yScale2(d.y); });

    var svg2 = d3.select("#chart2").append("svg")
    .attr("width", width2 + margin2.left + margin2.right)
    .attr("height", height2 + margin2.top + margin2.bottom)
  .append("g")
    .attr("transform", "translate(" + margin2.left + "," + margin2.top + ")");

svg2.append("defs").append("clipPath")
    .attr("id", "clip2")
  .append("rect")
    .attr("width", width2)
    .attr("height", height2);

svg2.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + yScale(0) + ")")
    .call(d3.svg.axis().scale(xScale2).orient("bottom"))
    .append("text") // x축 이름 추가
    .attr("class", "axis-label")
    .attr("x", width/1.15)
    .attr("y", 30)
    .text("(1/10 Second)");

svg2.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(yScale2).orient("left"))
    .append("text") // y축 이름 추가
    .attr("class", "axis-label")
    .attr("transform", "rotate(-90)")
    .attr("x", -height / 1.8)
    .attr("y", -30)
    .text("(meter)");

var path2 = svg2.append("g")
    .attr("clip-path", "url(#clip2)")
  .append("path")
    .attr("class", "line");
//그래프3 끝

//air flow log 시작
var scene1 = new THREE.Scene();
var camera1 = new THREE.PerspectiveCamera( 75, 1.7/*window.innerWidth / window.innerHeight*/, 0.1, 1000 );
var renderer1 = new THREE.WebGLRenderer();
renderer1.setSize( 520,300);//window.innerWidth, window.innerHeight );
document.getElementById('airflow').appendChild(renderer1.domElement); // 변경된 부분: 그래프를 airflow div에 추가

var controls1 = new THREE.OrbitControls( camera1, renderer1.domElement );
controls1.enableDamping = true;
controls1.dampingFactor = 0.25;
controls1.screenSpacePanning = false;
controls1.minDistance = 1; // 카메라 최대줌
controls1.maxDistance = 1000;//카메라 최소줌
controls1.maxPolarAngle = Math.PI / 2;

var material1 = new THREE.LineBasicMaterial( { color: 0x00ffff } );
var geometry1 = new THREE.BufferGeometry();
var vertices = [];
var indices = [];



camera1.position.z = 100;

// xyz 눈금자 그리기
for (var i = -500; i <= 500; i++) {
var geometry1 = new THREE.BoxGeometry(2, 2, 2);
var material1 = new THREE.MeshBasicMaterial({ color: 0x0000ff });
var cube = new THREE.Mesh(geometry1, material1);
cube.position.set(i, 0, 0);
scene1.add(cube);
}

// y축 눈금 생성
for (var i = -500; i <= 500; i++) {
var geometry1 = new THREE.BoxGeometry(2, 2, 2);
var material1 = new THREE.MeshBasicMaterial({ color: 0x00ff00 });
var cube = new THREE.Mesh(geometry1, material1);
cube.position.set(0, i, 0);
scene1.add(cube);
}

// z축 눈금 생성
for (var i = -500; i <= 500; i++) {
var geometry1 = new THREE.BoxGeometry(2, 2, 2);
var material1 = new THREE.MeshBasicMaterial({ color: 0xff0000 });
var cube = new THREE.Mesh(geometry1, material1);
cube.position.set(0, 0, i);
scene1.add(cube);
}
//air flow log 끝

form.addEventListener('submit', function(e) {
  e.preventDefault();
  if (input.value) {
    socket.emit('message', input.value);
    input.value = '';
    console.log("sending OK");
  }
 });
 //points.push(new THREE.Vector3(0,0,0));
 socket.on('data', (msg) => {
  raw_obj = msg.split('|') 
  modify_obj = {
    x: Math.round(111220*(raw_obj[9] - origin_lonlat[1])),
    y: Math.round(91560*(raw_obj[10] - origin_lonlat[0])), 
    z: Math.round(raw_obj[12]) - altitude_diff,
    u: (111220*(raw_obj[9] - origin_lonlat[1])) + raw_obj[6], 
    v: (91560*(raw_obj[10] - origin_lonlat[0])) + raw_obj[7], 
    w: raw_obj[12] + Math.sqrt(raw_obj[8] * raw_obj[8] - raw_obj[6] * raw_obj[6] - raw_obj[7] * raw_obj[7]), 
    roll : Math.atan(raw_obj[0] / Math.sqrt(raw_obj[2]*raw_obj[2] + raw_obj[1]*raw_obj[1])),
    pitch : Math.atan(raw_obj[2] / Math.sqrt(raw_obj[0]*raw_obj[0] + raw_obj[1]*raw_obj[1])),
    yaw : Math.atan(Math.sqrt(raw_obj[0]*raw_obj[0] + raw_obj[2]*raw_obj[2]) / (-raw_obj[1]) ), 
    t : raw_obj[13], 
    h : raw_obj[14], 
    at : raw_obj[11] 
  };
  //console.log(obj);



  points.push(new THREE.Vector3(modify_obj['y'], modify_obj['z'], -modify_obj['x']));
  airFlowData.push({x: -modify_obj['x'], y: modify_obj['y'], z: modify_obj['z'], u: modify_obj['u'], v: modify_obj['v'], w: modify_obj['w']});
  console.log(modify_obj);
  // line 객체가 생성되어 있지 않으면 생성합니다.
  if (!line) {
    const material1 = new THREE.LineBasicMaterial({ color: 0x0000ff });
    const geometry1 = new THREE.BufferGeometry().setFromPoints(points);
    line = new THREE.Line(geometry1, material1);
    this.scene.add(line);
  }

  // line 객체가 이미 생성되어 있으면 위치를 변경합니다.
  else {
    const geometry1 = new THREE.BufferGeometry().setFromPoints(points);
    line.geometry.dispose();
    line.geometry = geometry1;
  }

  // 기존의 코드에서 points 배열을 사용하는 부분을 모두 제거합니다.
  lx.innerText = raw_obj[9];
  ly.innerText = raw_obj[10];
  lz.innerText = Math.round(modify_obj['z']);
  pitch.innerText = modify_obj['pitch'];
  yaw.innerText = modify_obj['yaw'];
  roll.innerText = modify_obj['roll'];
  ex.innerText = 200;
  ey.innerText = 200;
  t.innerText = Number(modify_obj['t']);
  h.innerText = Number(modify_obj['h']);
  at.innerText = Number(modify_obj['at']);
 // console.log(airFlowData); // airFlowData를 콘솔에 출력
});

mapboxgl.accessToken = 'pk.eyJ1IjoidGFleW91IiwiYSI6ImNsZHY2ajVkejA4MGszdm5vaWpvdG41Nm0ifQ.05ZauoKkriS9v4sp6ozTAA';
const map = new mapboxgl.Map({
container: 'map',
// Choose from Mapbox's core styles, or make your own style with Mapbox Studio
style: 'mapbox://styles/mapbox/satellite-streets-v11',//light-v11
zoom: 17,
center: [origin_lonlat[0], origin_lonlat[1]],//시선 좌표
pitch: 60,
antialias: true // create the gl context with MSAA antialiasing, so custom layers are antialiased
});

// parameters to ensure the model is georeferenced correctly on the map
modelOrigin = [origin_lonlat[0], origin_lonlat[1]];
modelAltitude = 0;
modelRotate = [Math.PI / 2, 0, 0];

const modelAsMercatorCoordinate = mapboxgl.MercatorCoordinate.fromLngLat(
modelOrigin,
modelAltitude
);

// transformation parameters to position, rotate and scale the 3D model onto the map
const modelTransform = {
translateX: modelAsMercatorCoordinate.x,
translateY: modelAsMercatorCoordinate.y,
translateZ: modelAsMercatorCoordinate.z,
rotateX: modelRotate[0],
rotateY: modelRotate[1],
rotateZ: modelRotate[2],
/* Since the 3D model is in real world meters, a scale transform needs to be
* applied since the CustomLayerInterface expects units in MercatorCoordinates.
*/
scale: modelAsMercatorCoordinate.meterInMercatorCoordinateUnits()
};



// configuration of the custom layer for a 3D model per the CustomLayerInterface
const customLayer = {
id: '3d-model',
type: 'custom',
renderingMode: '3d',
onAdd: function (map, gl) {
this.camera = new THREE.Camera();
this.scene = new THREE.Scene();

// create two three.js lights to illuminate the model
const directionalLight = new THREE.DirectionalLight(0xffffff);
directionalLight.position.set(0, -70, 100).normalize();
this.scene.add(directionalLight);

const directionalLight2 = new THREE.DirectionalLight(0xffffff);
directionalLight2.position.set(0, 70, 100).normalize();
this.scene.add(directionalLight2);

//비행 경로 표시
const material1 = new THREE.LineBasicMaterial( { color: 0xff0000 } );

var geometry1;
 geometry1 = new THREE.BufferGeometry().setFromPoints( points );
         line = new THREE.Line( geometry1, material1 );


//낙하지점



const geometry = new THREE.BoxGeometry( 10, 3, 10 );
      const material = new THREE.MeshBasicMaterial( { color: 0xff00ff } );
      const cube = new THREE.Mesh( geometry, material );


// use the three.js GLTF loader to add the 3D model to the three.js scene
const loader = new THREE.GLTFLoader();
loader.load(
//'/rocket',
'/sci-fi_rocket/scene.gltf',
(gltf) => {
//this.scene.add(gltf.scene);
this.scene.add(gltf.scene);
this.scene.add( cube );
gltf.scene.scale.set(2,3,2) //로켓 크기 설정
this.scene.add( line ); //선 긋기

function animate(){
//this.scene.add( cube );
  requestAnimationFrame(animate) //1초에 60번 실행됨.

  //회전
  gltf.scene.position.z = -Number(modify_obj['x']); 
  gltf.scene.position.x = Number(modify_obj['y']); //xy 바꿔봄
  gltf.scene.position.y = Number(modify_obj['z']); //높이
  gltf.scene.rotation.z = Number(modify_obj['pitch']);
  gltf.scene.rotation.y = Number(modify_obj['yaw']);
  gltf.scene.rotation.x = Number(modify_obj['roll']);
  cube.rotation.y += 0.08;
  cube.position.z = 200; //예상 낙하지점
  cube.position.x = 200;

  //그래프1 시작
  var newData = { x: count-2000, y: modify_obj['x'] };
  data.push(newData);
  
  // redraw the line
  path.datum(data)
    .attr("d", lineGenerator);
  
  // shift x axis to the left
  xScale.domain([count-2400, count-2000]);
  svg.select(".x.axis")
    .transition()
    .duration(50)
    .ease("linear")
    .call(d3.svg.axis().scale(xScale).orient("bottom"));
  
  // pop the old data point off the front
  if (data.length > 2000) {
    data.shift();
  }
  
  //그래프1 끝
  //그래프2 시작
  var newData1 = { x: count1-2000, y: modify_obj['y'] };
  data1.push(newData1);
 
  
  // redraw the line
  path1.datum(data1)
    .attr("d", lineGenerator1);
  
  // shift x axis to the left
  xScale1.domain([count1-2400, count1-2000]);
  svg1.select(".x.axis")
    .transition()
    .duration(50)
    .ease("linear")
    .call(d3.svg.axis().scale(xScale1).orient("bottom"));
  
  // pop the old data point off the front
  if (data1.length > 2000) {
    data1.shift();
  }

  //그래프2 끝
  //그래프3 시작
  var newData2 = { x: count2-2000, y: modify_obj['z']};
  data2.push(newData2);
 
  
  // redraw the line
  path2.datum(data2)
    .attr("d", lineGenerator2);
  
  // shift x axis to the left
  xScale2.domain([count2-2400, count2-2000]);
  svg2.select(".x.axis")
    .transition()
    .duration(50)
    .ease("linear")
    .call(d3.svg.axis().scale(xScale2).orient("bottom"));
  
  // pop the old data point off the front
  if (data2.length > 2000) {
    data2.shift();
  }
  // increment count
  if (frameCounter++ % 4 === 0){
    count++;
    count1++;
    count2++;
  }
  
 
  renderer1.render(scene1, camera1);

  

}
animate();


var material1 = new THREE.MeshBasicMaterial({ color: 0xffff00 });

function airflowdraw(){
//air flow log print
  //console.log(airFlowData)
  airFlowData.forEach(function(d, i) {
    vertices.push(d.x, d.y, d.z);
    vertices.push(d.x + d.u, d.y + d.v, d.z + d.w);
    indices.push(i*2, i*2+1);
  });
  geometry1.setAttribute( 'position', new THREE.Float32BufferAttribute(vertices, 3));
geometry1.setIndex(indices);
var lines1 = new THREE.LineSegments(geometry1, material1);
scene1.add(lines1);
vertices = [];
indices = [];
}
setInterval(airflowdraw, 1000);

}
//,
//undefined, function(error) {
//  console.error(error);
//}
);

this.map = map;

// use the Mapbox GL JS map canvas for three.js
this.renderer = new THREE.WebGLRenderer({
canvas: map.getCanvas(),
context: gl,
antialias: true
});

this.renderer.autoClear = false;
},
render: function (gl, matrix) {
const rotationX = new THREE.Matrix4().makeRotationAxis(
new THREE.Vector3(1, 0, 0),
modelTransform.rotateX
);
const rotationY = new THREE.Matrix4().makeRotationAxis(
new THREE.Vector3(0, 1, 0),
modelTransform.rotateY
);
const rotationZ = new THREE.Matrix4().makeRotationAxis(
new THREE.Vector3(0, 0, 1),
modelTransform.rotateZ
);

const m = new THREE.Matrix4().fromArray(matrix);
const l = new THREE.Matrix4()
.makeTranslation(
modelTransform.translateX,
modelTransform.translateY,
modelTransform.translateZ
)
.scale(
new THREE.Vector3(
modelTransform.scale,
-modelTransform.scale,
modelTransform.scale
)
)
.multiply(rotationX)
.multiply(rotationY)
.multiply(rotationZ);

this.camera.projectionMatrix = m.multiply(l);
this.renderer.resetState();
this.renderer.render(this.scene, this.camera);
this.map.triggerRepaint();
}
};

map.on('style.load', () => {
map.addLayer(customLayer, 'waterway-label');
// Insert the layer beneath any symbol layer.
const layers = map.getStyle().layers;
const labelLayerId = layers.find(
(layer) => layer.type === 'symbol' && layer.layout['text-field']
).id;

// The 'building' layer in the Mapbox Streets
// vector tileset contains building height data
// from OpenStreetMap.
map.addLayer(
{
'id': 'add-3d-buildings',
'source': 'composite',
'source-layer': 'building',
'filter': ['==', 'extrude', 'true'],
'type': 'fill-extrusion',
'minzoom': 15,
'paint': {
'fill-extrusion-color': '#aaa',

// Use an 'interpolate' expression to
// add a smooth transition effect to
// the buildings as the user zooms in.
'fill-extrusion-height': [
'interpolate',
['linear'],
['zoom'],
15,
0,
15.05,
['get', 'height']
],
'fill-extrusion-base': [
'interpolate',
['linear'],
['zoom'],
15,
0,
15.05,
['get', 'min_height']
],
'fill-extrusion-opacity': 0.6
}
},
labelLayerId
);
});