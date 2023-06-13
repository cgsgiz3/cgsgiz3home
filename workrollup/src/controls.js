/* eslint-disable no-constant-condition */
/* eslint-disable no-undef */
import { units } from "./units.js";
import { Camera } from "./camera.js";
import { Timer } from "./timer.js";
import { mtlCreate } from "./material.js";

let controls = units();
controls.init = () => {
  controls.primitive.a1 = {};
  controls.primitive.a1.file = "./background";
  controls.primitive.a1.idCanvas = "glCanvas";
  controls.primitive.a1.type = "tristrip";
  const pos = [-1, -1, 0, -1, 1, 0, 1, -1, 0, 1, 1, 0];
  const normal = [1, 0.5, 0.1, 0.1, 0.5, 0.6, 0.2, 0.4, 0.5, 0.3, 0.9, 1];
  const mtl = mtlCreate("Obsidian");
  controls.primitive.a1.buffer = {
    in_pos: pos,
    in_normal: normal,
  };
  controls.primitive.a1.uniform = {
    frameBuffer: () => {
      return Camera.vp.massiv;
    },
    materialBuffer: mtl,
  };
  //controls.create(controls.primitive.a1);
};
controls.render = () => {
  Camera.responsecamera();
  Camera.responsemousewheel();
  Camera.responsemouse();
  Timer.response("fps");
  //controls.primitive.a1.draw();
};
export { controls };
