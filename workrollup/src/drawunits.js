/* eslint-disable no-undef */
import { units } from "./units.js";
import { background } from "./background.js";
import { objLoadSync } from "./3DObj.js";

let drawunits = units();
drawunits.init = () => {
  background.init();
  drawunits.primitive = objLoadSync("./cow.obj");
};
drawunits.render = () => {
  gl.disable(gl.DEPTH_TEST);
  background.render();
  gl.enable(gl.DEPTH_TEST);
  drawunits.primitive.draw();
  //window.requestAnimationFrame(drawunits.render);
  //window.requestAnimationFrame(drawunits.render);
  //window.requestAnimationFrame(drawunits.render);
};
export { drawunits };
