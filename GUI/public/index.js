var socket = io();
var obj;

var points = [];
var line = null;



var x = document.getElementById('x');
var y = document.getElementById('y');
var z = document.getElementById('z');
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
  console.log(obj);



  points.push(new THREE.Vector3(obj.x, obj.z, obj.y));

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
  x.innerText = obj.x;
  y.innerText = obj.y;
  z.innerText = obj.z;
  pitch.innerText = obj.pitch;
  yaw.innerText = obj.yaw;
  roll.innerText = obj.roll;
  ex.innerText = obj.ex;
  ey.innerText = obj.ey;
  t.innerText = obj.t;
  h.innerText = obj.h;
  at.innerText = obj.at;
});

mapboxgl.accessToken = 'pk.eyJ1IjoidGFleW91IiwiYSI6ImNsZHY2ajVkejA4MGszdm5vaWpvdG41Nm0ifQ.05ZauoKkriS9v4sp6ozTAA';
const map = new mapboxgl.Map({
container: 'map',
// Choose from Mapbox's core styles, or make your own style with Mapbox Studio
style: 'mapbox://styles/mapbox/light-v11',
zoom: 17,
center: [127.20799627897934, 34.610],
pitch: 60,
antialias: true // create the gl context with MSAA antialiasing, so custom layers are antialiased
});

// parameters to ensure the model is georeferenced correctly on the map
modelOrigin = [127.20799627897930, 34.610];
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

const THREE = window.THREE;

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

  renderer.render(scene,camera);  
}
animate();
}//,
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
