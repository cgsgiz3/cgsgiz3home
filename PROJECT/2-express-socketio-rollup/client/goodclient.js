import { io } from "socket.io-client";
import { startGame } from "./system/system.js";

let otherprims = {};
async function main() {
  let socket = io();
  socket.on("connect", () => {
    socket.on("getOtherPrim", ([azimuth, elevator, at, name]) => {
      otherprim[name] = [azimuth, elevator, at];
    });
    const draw = () => {
      sys.primgrid.draw();
      sys.primcow.draw(
        sys.vec3(sys.at.x - sys.primcow.location, 1, sys.at.z).translete()
      );
      for (let property in otherprims) {
        sys.primcow.draw(
          sys.matrRotateX(otherprims[property].elevator),
          sys.matrRotateY(otherprims[property].azimuth),
          sys.mulMatr3(
            sys
              .vec3(
                otherprims[property].at.x - sys.primcow.location,
                0,
                otherprims[property].at.z
              )
              .translete()
          )
        );
      }
      sys.responsetimer();
      sys.responsecamera();
      socket.emit("takeSocketPrim", [
        sys.azimuth,
        sys.elevator,
        sys.loc,
        sys.at,
      ]);
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
