import { mth } from "./mth/mth.js";
import { camera } from "./camera/camera.js";
import { objLoadSync } from "./prim/3DObj.js";
import { timer } from "./timer/timer.js";
import { material } from "./prim/material/material.js";
import { createprimitive, primitiveCreateGrid } from "./prim/prim.js";
class system {
  constructor() {
    Object.assign(this, new material());
    Object.assign(this, new timer());
    Object.assign(this, new mth());
    Object.assign(this, new camera());
    this.primitiveCreateGrid = primitiveCreateGrid;
    this.createprimitive = createprimitive;
  }
}
window.sys = new system();
export function startGame() {
  window.canvas = document.getElementById("glCanvas");
  window.gl = window.canvas.getContext("webgl2");
  sys.primcow = objLoadSync();
  sys.primcow.location = 11;
  sys.primgrid = sys.primitiveCreateGrid(2, 2);
  sys.resize(canvas.width, canvas.height);
  //sys.set(sys.vec3(8, 8, 8), sys.vec3(0, 8, 0));
  //.type = "trimesh";
  //let VertexPositions = [
  //  vec3(1, 1, 1),
  //  vec3(1, 1, -1),
  //  vec3(1, -1, -1),
  //  vec3(1, -1, 1),
  //  vec3(-1, 1, 1),
  //  vec3(-1, -1, -1),
  //  vec3(-1, -1, 1),
  //  vec3(-1, 1, -1),
  //];
  //let VertexIndices = [
  //  0, 1, 2, 0, 2, 3, 2, 3, 5, 3, 5, 6, 6, 5, 7, 6, 7, 4, 7, 4, 1, 4, 1, 0, 0,
  //  4, 6, 0, 6, 3, 1, 7, 5, 1, 5, 2,
  //];
  //plato.cube.buffer = {
  //  in_pos: VertexPositions,
  //  index: VertexIndices,
  //  in_normal: autonormals(VertexPositions, VertexIndices, "trimesh"),
  //};
  //plato.cube.uniform = {
  //  frameBuffer: () => {
  //    return Camera.vp.massiv;
  //  },
  //  materialBuffer: mtl,
  //  cameraBuffer: () => {
  //    return [
  //      [...Camera.loc.array(), 1],
  //      [...Camera.dir.array(), 1],
  //      [...Camera.right.array(), 1],
  //      [...Camera.up.array(), 1],
  //      [Timer.localTime, Timer.globalDeltaTime, 1, 1],
  //    ];
  //  },
  //};
  //createprimitive(plato.cube);
  gl.clearColor(0, 1, 0, 1);
  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.clear(gl.DEPTH_BUFFER_BIT);
  gl.enable(gl.DEPTH_TEST);
}
