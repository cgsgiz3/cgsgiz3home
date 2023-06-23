import { io } from "socket.io-client";
import { startGame } from "./system/system.js";

let otherprims = {};
async function main() {
  let socket = io();
  let divContainerElement = document.getElementById("divcontainer");
  let autoCow = document.getElementById("autoCow");
  socket.on("connect", () => {
    sys.sizeField = 500.0;
    sys.primcow = sys.objLoadSync("cow");
    sys.primtree = sys.objLoadSync("tree");
    sys.primcloth = sys.primitiveCreateCloth(3, 3);
    sys.primgrid = sys.primitiveCreateGrid(20, 20, sys.sizeField);
    sys.primsky = sys.primitiveCreateSky("./skybox/IceMountains");
    sys.resize(canvas.width, canvas.height);
    autoCow.onchange = () => {
      if (autoCow.checked) {
        let startTime = sys.localTime;
        let drawAutoCow = () => {
          let t = sys.localTime - startTime;
          startTime = sys.localTime;
          let vector = sys.vec3(
            sys.at.x,
            sys.at.y - sys.primcow.maxBB.y,
            sys.at.z
          );
          let cowRight = sys.addVec(
            sys.vec3(sys.right).normalize() * 8.125,
            vector
          );
          let finalvector = sys.addVec(
            sys.addVec(
              sys.vec3(sys.autoCow.pos).mul(1 - t * t),
              cowRight.mul(2 * (1 - t))
            ),
            sys.vec3(vector).mul(t * t)
          );
          let angle = Math.acos(
            sys
              .vec3(1, 0, 0)
              .dot(sys.subVec(finalvector, sys.autoCow.pos).normalize())
          );
          sys.autoCow.pos = finalvector;
          let world = sys.mulMatr(
            sys.matrRotateY(angle),
            sys.vec3(finalvector).translete()
          );
          if (sys.subVec(finalvector, vector).length() < 1) {
            let losediv = document.createElement("div");
            losediv.className = "lose-div";
            losediv.innerText = "You lose";
            divContainerElement.appendChild(losediv);
            sys = {};
          }
          sys.primcow.draw(world);

          window.requestAnimationFrame(drawAutoCow);
        };
        drawAutoCow();
      } else {
      }
    };
    socket.on("deleteOtherPrim", (name) => {
      delete otherprims[name];
      sys.primcow.instance--;
      sys.primcow.updateShader(sys.primcow.instance);
    });
    socket.on("clietnLoseFromServer", () => {
      let losediv = document.createElement("div");
      losediv.className = "lose-div";
      losediv.innerText = "You lose";
      divContainerElement.appendChild(losediv);
      sys = {};
    });
    socket.on("getOtherPrim", ([world, name, legend]) => {
      if (otherprims[name]) {
        otherprims[name] = [world, otherprims[name][1], legend];
      } else {
        sys.primcow.instance++;
        sys.primcow.updateShader(sys.primcow.instance);
        otherprims[name] = document.createElement("div");
        otherprims[name].className = "floating-div";
        otherprims[name].innerText = name;
        divContainerElement.appendChild(otherprims[name]);
        otherprims[name] = [world, otherprims[name]];
      }
    });
    const draw = () => {
      gl.disable(gl.DEPTH_TEST);
      sys.primsky.draw();
      gl.enable(gl.DEPTH_TEST);
      sys.primgrid.draw();
      sys.primtree.draw(sys.matrScale(sys.vec3(15, 15, 15)));
      sys.primcloth.apply();
      sys.primcloth.draw();
      let world = [];
      world[0] = sys.mulMatr(
        sys.matrRotateY(sys.azimuth - sys.startazimuth),
        sys.vec3(sys.at.x, sys.at.y - sys.primcow.maxBB.y, sys.at.z).translete()
      );
      let i = 1;
      for (let property in otherprims) {
        world[i++] = otherprims[property][0];
        let v = sys.vecMulMatr(
          sys.locationName,
          sys.mulMatr(otherprims[property][0], sys.vp)
        );
        let pixelX = (v.x * 0.5 + 0.5) * gl.canvas.width;
        let pixelY = (v.y * -0.5 + 0.5) * gl.canvas.height;
        if (
          pixelX > 0.0 &&
          pixelX < gl.canvas.width &&
          pixelY > 0.0 &&
          pixelY < gl.canvas.height
        ) {
          otherprims[property][1].style.left = Math.floor(pixelX) + "px";
          otherprims[property][1].style.top = Math.floor(pixelY) + "px";
        }
      }
      sys.primcow.draw(world);
      socket.emit("takeSocketPrim", world[0]);
      sys.responsetimer();
      sys.responsecamera();
      window.requestAnimationFrame(draw);
    };
    draw();
  });
  socket.on("disconnect", () => {});
}
window.addEventListener("load", (event) => {
  startGame();
  window.addEventListener("mousedown", (event) => {
    sys.responsemouseup(event);
  });
  window.addEventListener("mouseup", (event) => {
    sys.responsemousedown(event);
  });
  document.getElementById("glCanvas").addEventListener("mousemove", (event) => {
    sys.responsemouse(event);
  });
  window.addEventListener("keyup", (event) => {
    sys.responsekeyup(event);
  });
  window.addEventListener("keydown", (event) => {
    sys.responsekeydown(event);
  });
  window.addEventListener("wheel", (event) => {
    sys.responsemousewheel(event);
    event.preventDefault();
  });
  main();
});
