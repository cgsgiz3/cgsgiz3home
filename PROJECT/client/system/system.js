import { mth } from "./mth/mth.js";
import { camera } from "./camera/camera.js";
import { objLoadSync } from "./prim/3DObj.js";
import { timer } from "./timer/timer.js";
import { material } from "./prim/material/material.js";
import {
  createprimitive,
  primitiveCreateGrid,
  primitiveCreateSky,
  primitiveCreateCloth,
} from "./prim/prim.js";
class system {
  constructor() {
    Object.assign(this, new material());
    Object.assign(this, new timer());
    Object.assign(this, new mth());
    Object.assign(this, new camera());
    this.primitiveCreateGrid = primitiveCreateGrid;
    this.createprimitive = createprimitive;
    this.primitiveCreateSky = primitiveCreateSky;
    this.objLoadSync = objLoadSync;
    this.primitiveCreateCloth = primitiveCreateCloth;
  }
}
window.sys = new system();
export function startGame() {
  window.canvas = document.getElementById("glCanvas");
  window.gl = window.canvas.getContext("webgl2");
  gl.clearColor(0, 1, 0, 1);
  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.clear(gl.DEPTH_BUFFER_BIT);
  gl.enable(gl.DEPTH_TEST);
}
