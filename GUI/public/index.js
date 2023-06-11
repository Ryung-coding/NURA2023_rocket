var socket = io();
var obj;
var modify_obj;

var points = [];
var airFlowData = [];
var line = null;
const THREE = window.THREE;


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
    .call(d3.svg.axis().scale(xScale).orient("bottom"));

svg.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(yScale).orient("left"));

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
    .call(d3.svg.axis().scale(xScale1).orient("bottom"));

svg1.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(yScale1).orient("left"));

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
    .call(d3.svg.axis().scale(xScale2).orient("bottom"));

svg2.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(yScale2).orient("left"));

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
  //console.log(msg);
  obj = JSON.parse(msg);
  //modify_obj = {x: obj.x, y: obj.y, z: obj.z, u: obj.airx, v: obj.airy, w: obj.airz};
  console.log(obj);



  points.push(new THREE.Vector3(obj.x, obj.z, obj.y));
  airFlowData.push({x: obj.x, y: obj.y, z: obj.z, u: obj.airx, v: obj.airy, w: obj.airz});
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
  lx.innerText = obj.x;
  ly.innerText = obj.y;
  lz.innerText = obj.z;
  pitch.innerText = obj.pitch;
  yaw.innerText = obj.yaw;
  roll.innerText = obj.roll;
  ex.innerText = obj.ex;
  ey.innerText = obj.ey;
  t.innerText = obj.t;
  h.innerText = obj.h;
  at.innerText = obj.at;
 // console.log(airFlowData); // airFlowData를 콘솔에 출력
});

mapboxgl.accessToken = 'pk.eyJ1IjoidGFleW91IiwiYSI6ImNsZHY2ajVkejA4MGszdm5vaWpvdG41Nm0ifQ.05ZauoKkriS9v4sp6ozTAA';
const map = new mapboxgl.Map({
container: 'map',
// Choose from Mapbox's core styles, or make your own style with Mapbox Studio
style: 'mapbox://styles/mapbox/satellite-streets-v11',//light-v11
zoom: 17,
center: [127.20799627897934, 34.610],
pitch: 60,
antialias: true // create the gl context with MSAA antialiasing, so custom layers are antialiased
});

// parameters to ensure the model is georeferenced correctly on the map
modelOrigin = [127.20799627897930, 34.610]; //현장에서 바꿈
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

//const geometry1 = new THREE.BufferGeometry().setFromPoints( points );
//const line = new THREE.Line( geometry1, material1 );
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
  gltf.scene.position.z = Number(obj.x); 
  gltf.scene.position.x = Number(obj.y);
  gltf.scene.position.y = Number(obj.z); //높이
  gltf.scene.rotation.z = Number(obj.pitch);
  gltf.scene.rotation.y = Number(obj.yaw);
  gltf.scene.rotation.x = Number(obj.roll);
  cube.rotation.y += 0.08;
  cube.position.z = Number(obj.ex);
  cube.position.x = Number(obj.ey);

  //그래프1 시작
  var newData = { x: count-2000, y: obj.x };
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
  // increment count
  count++;
  //그래프1 끝
  //그래프2 시작
  var newData1 = { x: count1-2000, y: obj.y };
  data1.push(newData1);
 
  
  // redraw the line
  path1.datum(data1)
    .attr("d", lineGenerator1);
  
  // shift x axis to the left
  xScale1.domain([count1-4000, count1-2000]);
  svg1.select(".x.axis")
    .transition()
    .duration(50)
    .ease("linear")
    .call(d3.svg.axis().scale(xScale1).orient("bottom"));
  
  // pop the old data point off the front
  if (data1.length > 2000) {
    data1.shift();
  }
  // increment count
  count1++;
  //그래프2 끝
  //그래프3 시작
  var newData2 = { x: count2-2000, y: obj.z };
  data2.push(newData2);
 
  
  // redraw the line
  path2.datum(data2)
    .attr("d", lineGenerator2);
  
  // shift x axis to the left
  xScale2.domain([count2-4000, count2-2000]);
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
  count2++;
  //그래프3 끝
  
 
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

//geometry1 = new THREE.BufferGeometry().setFromPoints( points );
//line = new THREE.Line( geometry1, material1 );
//this.scene.add( line );
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

//여기부터 air flow log



		// // 측정 지점의 수
		// var numPoints = 200;

		// // 측정 지점 생성
		// for (var i = 0; i < numPoints; i++) {
		// 	var x = Math.random() * 100 - 50;  // x 좌표
		// 	var y = Math.random() * 100 - 50;  // y 좌표
		// 	var z = Math.random() * 100 - 50;  // z 좌표

		// 	// 공기 흐름 속도 계산
		// 	var u = Math.random() * 20 - 5;    // x 방향 속도
		// 	var v = Math.random() * 20 - 5;    // y 방향 속도
		// 	var w = Math.random() * 20 - 5;    // z 방향 속도

		// 	// 측정 지점 정보 저장
		// 	airFlowData.push({x: x, y: y, z: z, u: u, v: v, w: w});
		// }


     