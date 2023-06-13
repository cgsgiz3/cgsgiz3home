/* eslint-disable no-unused-vars */
import { matrrotateX, matrrotateY, mulmatr } from "./math.js";
import { mtlCreate } from "./material.js";
import { Timer } from "./timer.js";
import { vec3, autonormals } from "./math.js";
//import * as mth from "./math.js";
//import * as prim from "./prim.js";
import { Camera } from "./camera.js";
import { createprimitive } from "./prim.js";
let plato = {};
plato.init = () => {
  /*cube*/
  const PH = 1.618033988749894;
  plato.cube = {};
  plato.cube.file = "./icosahedron";
  plato.cube.type = "trimesh";
  //plato.cube.material = mtlCreate("Ruby");
  plato.cube.idCanvas = "glCanvas1";
  //plato.cube.texture = {Tex1: "plato.png"}
  let VertexPositions = [
    vec3(1, 1, 1),
    vec3(1, 1, -1),
    vec3(1, -1, -1),
    vec3(1, -1, 1),
    vec3(-1, 1, 1),
    vec3(-1, -1, -1),
    vec3(-1, -1, 1),
    vec3(-1, 1, -1),
  ];
  let VertexIndices = [
    0, 1, 2, 0, 2, 3, 2, 3, 5, 3, 5, 6, 6, 5, 7, 6, 7, 4, 7, 4, 1, 4, 1, 0, 0,
    4, 6, 0, 6, 3, 1, 7, 5, 1, 5, 2,
  ];
  let mtl = mtlCreate("Bronze");
  plato.cube.buffer = {
    in_pos: VertexPositions,
    index: VertexIndices,
    in_normal: autonormals(VertexPositions, VertexIndices, "trimesh"),
  };
  plato.cube.uniform = {
    frameBuffer: () => {
      return Camera.vp.massiv;
    },
    materialBuffer: mtl,
    cameraBuffer: () => {
      return [
        [...Camera.loc.array(), 1],
        [...Camera.dir.array(), 1],
        [...Camera.right.array(), 1],
        [...Camera.up.array(), 1],
        [Timer.localTime, Timer.globalDeltaTime, 1, 1],
      ];
    },
  };
  createprimitive(plato.cube);
  /*icosahedron*/
  plato.icosahedron = {};
  plato.icosahedron.file = "./icosahedron";
  plato.icosahedron.type = "trimesh";
  plato.icosahedron.material = mtlCreate("Ruby");
  plato.icosahedron.idCanvas = "glCanvas2";
  VertexPositions = [
    PH,
    0,
    1, //0
    0,
    -1,
    PH, //1
    0,
    1,
    PH, //2
    -PH,
    0,
    1, //3
    1,
    -PH,
    0, //4
    1,
    PH,
    0, //5
    -1,
    -PH,
    0, //6
    -1,
    PH,
    0, //7
    0,
    -1,
    -PH, //8
    0,
    1,
    -PH, //9
    PH,
    0,
    -1, //10
    -PH,
    0,
    -1, //11
  ];
  VertexIndices = [
    0, 1, 2, 1, 2, 3, 3, 1, 6, 1, 4, 0, 1, 6, 4, 6, 4, 8, 8, 4, 10, 4, 0, 10, 6,
    8, 11, 6, 11, 3, 8, 10, 9, 8, 11, 9, 10, 9, 5, 0, 5, 10, 0, 2, 5, 7, 5, 9,
    7, 9, 11, 3, 11, 7, 3, 2, 7, 7, 2, 5,
  ];
  mtl = mtlCreate("Bronze");
  plato.icosahedron.buffer = {
    in_pos: VertexPositions,
    index: VertexIndices,
    in_normal: autonormals(VertexPositions, VertexIndices, "trimesh"),
  };
  plato.icosahedron.uniform = {
    frameBuffer: () => {
      return mulmatr(Camera.vp, matrrotateY(Timer.localTime));
    },
    materialBuffer: mtl,
    cameraBuffer: () => {
      return [
        [...Camera.loc.array(), 1],
        [...Camera.dir.array(), 1],
        [...Camera.right.array(), 1],
        [...Camera.up.array(), 1],
        [Timer.localTime, Timer.globalDeltaTime, 1, 1],
      ];
    },
  };
  createprimitive(plato.icosahedron);
  /* plato */
  plato.tetrahedron = {};
  plato.tetrahedron.file = "./icosahedron";
  plato.tetrahedron.type = "trimesh";
  plato.tetrahedron.material = mtlCreate("Ruby");
  plato.tetrahedron.idCanvas = "glCanvas3";
  VertexPositions = [
    0,
    0,
    Math.sqrt(2 / 3), //0
    Math.sqrt(1 / 3),
    0,
    0, //1
    -Math.sqrt(1 / 12),
    0.5,
    0, //1
    -Math.sqrt(1 / 12),
    -0.5,
    0, //1
  ];
  VertexIndices = [0, 2, 3, 0, 1, 2, 0, 1, 3, 1, 2, 3];
  mtl = mtlCreate("Bronze");
  plato.tetrahedron.buffer = {
    in_pos: VertexPositions,
    index: VertexIndices,
    in_normal: autonormals(VertexPositions, VertexIndices, "trimesh"),
  };
  plato.tetrahedron.uniform = {
    frameBuffer: () => {
      return mulmatr(Camera.vp, matrrotateX(Timer.localTime));
    },
    materialBuffer: mtl,
    cameraBuffer: () => {
      return [
        [...Camera.loc.array(), 1],
        [...Camera.dir.array(), 1],
        [...Camera.right.array(), 1],
        [...Camera.up.array(), 1],
        [Timer.localTime, Timer.globalDeltaTime, 1, 1],
      ];
    },
  };
  createprimitive(plato.tetrahedron);
};
plato.render = () => {
  plato.cube.draw();
  plato.icosahedron.draw();
  plato.tetrahedron.draw();
};
export { plato };
