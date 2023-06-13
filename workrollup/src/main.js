/* eslint-disable no-undef */
/* eslint-disable no-constant-condition */
//import { icosahedron } from "./icosahedron.js";
import { controls } from "./controls.js";
import { drawunits } from "./drawunits.js";
//import * as shader from "./shader.js";
//import { Timer } from "/timer.js";
import { Camera, cameraResize } from "./camera.js";
Object.defineProperty(window, "gl", {
  get: () => {
    if (window._gl == null || window._gl == undefined) {
      window._gl = canvas.getContext("webgl2");
    }
    return window._gl;
  },
  set: (idCanvas) => {
    canvas = idCanvas;
    window._gl = canvas.getContext("webgl2");
  },
});
Object.defineProperty(window, "canvas", {
  get: () => {
    if (window._canvas == null || window._canvas == undefined) {
      window._canvas = BaseCanvas;
    }
    return window._canvas;
  },
  set: (idCanvas) => {
    if (window.OldidCanvas === idCanvas) {
      return;
    }
    window._canvas = document.getElementById(`${idCanvas}`);
    if (window._canvas === null) {
      window._canvas = document.getElementById("glCanvas");
    }
    window.OldidCanvas = idCanvas;
  },
});
/*
window.get = (idCanvas) => {
  let canvas = document.getElementById(`${idCanvas}`);
  if (canvas === null) {
    return BaseCanvas;
  }
  return canvas.getContext("webgl2");
};
*/
//const canvas = document.getElementById("glCanvas");
let BaseCanvas;
let DrawUnits = [];
function Init() {
  let i = 1;
  let name = "glCanvas";
  gl = name;
  while (true) {
    gl.clearColor(0, 1, 0, 1);
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.clear(gl.DEPTH_BUFFER_BIT);
    gl.enable(gl.DEPTH_TEST);
    name = `glCanvas${i}`;
    gl = name;
    if (BaseCanvas === gl) {
      break;
    }
    i++;
  }
  DrawUnits.push(drawunits, controls);
}
export function initGL() {
  Init();
  DrawUnits.forEach((res) => {
    res.units();
  });

  /*
  let UnitsPromise = [];
  DrawUnits.forEach((res) => {
    res.init();
  });

  gl.clearColor(0, 1, 1, 1);
  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.clear(gl.DEPTH_BUFFER_BIT);
  gl.enable(gl.DEPTH_TEST);
  
  /*
  DrawUnits.forEach((res) => {
    UnitsPromise.push(shader.createshader(res.file));
  });
  */
  /*
  const AllShader = Promise.([...UnitsPromise]);
  AllShader.then((res) => {
  };
  * /
  const draw = () => {
    if (!Timer.isPause) {
      Timer.response("fps");
      Camera.responsecamera();
      Camera.responsemousewheel();
      Camera.responsemouse();
      DrawUnits.forEach((draw) => {
        draw.render(res[0]);
      });
    }
  }
  window.requestAnimationFrame(draw);
  draw();
  */
}
window.addEventListener("load", () => {
  BaseCanvas = document.getElementById("glCanvas").getContext("webgl2");
  cameraResize();
  initGL();
  window.onscroll = () => window.scroll(0, 0);
  window.addEventListener("mousedown", (event) => {
    Camera.responsemouseclick(event);
  });
  window.addEventListener("mouseup", (event) => {
    Camera.responsemouseclick(event);
  });
  window.addEventListener("mousemove", (event) => {
    Camera.responsemouse(event);
  });
  window.addEventListener("keyup", (event) => {
    Camera.responsekeys(event);
  });
  window.addEventListener("keydown", (event) => {
    Camera.responsekeys(event);
  });
  window.addEventListener("wheel", (event) => {
    Camera.responsemousewheel(event);
    event.preventDefault();
  });
  /*
  type="text/javascript"
  window.setInterval(() => {
    Render();
  }, 300);
  */
});

//export function MouseMove(event, key) {
//  if (event === 0 || !key) {
//    Mouseobj.x1 = Mouseobj.x;
//    Mouseobj.y1 = Mouseobj.y;
//    return;
//  }
//  if (key) {
//    Mouseobj.x =
//      Mouseobj.x1 -
//      ((event.clientX / gl.canvas.width) * 2.0 - 1.0) * Mouseobj.wheel;
//    Mouseobj.y =
//      Mouseobj.y1 -
//      ((event.clientY / gl.canvas.height) * 2.0 - 1.0) * Mouseobj.wheel;
//  }
//}
//export function MouseWheel(event, key) {
//  ///*
//
//  if (key) {
//    Mouseobj.wheel += 0.125;
//  } else {
//    if (Mouseobj.wheel > 0.125) {
//      Mouseobj.wheel -= 0.025;
//    } else {
//      let k = 0.025;
//
//      // eslint-disable-next-line no-constant-condition
//      while (true) {
//        if (Mouseobj.wheel < k + k / 10.0) {
//          k = k / 1.5;
//        } else {
//          break;
//        }
//        if (k < 0.000000025) {
//          k = k / 5.0;
//        }
//      }
//      Mouseobj.wheel -= k;
//    }
//  }
//  Mouseobj.x +=
//    ((event.clientX / gl.canvas.width) * 2.0 - 1.0) * Mouseobj.wheel;
//  Mouseobj.y +=
//    ((event.clientY / gl.canvas.height) * 2.0 - 1.0) * Mouseobj.wheel;
//
//  event.preventDefault();
//}
//if (window.gl === undefined) {
//  window.gl = document.getElementById("glCanvas").getContext("webgl2");
//}
