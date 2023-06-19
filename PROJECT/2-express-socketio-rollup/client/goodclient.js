import { io } from "socket.io-client";
import {startGame} from "./system/system.js"

async function main() {
  let socket = io();
  socket.on("connect", () => 
  {
    const draw = () => {

      sys.prim.draw();
      sys.responsecamera();
      sys.responsetimer();
      window.requestAnimationFrame(draw);
    }
    draw();
  });
  socket.on("disconnect", () => {});
}
window.addEventListener("load", (event) => 
{
  startGame();
  window.addEventListener("mousedown", (event) => 
  {
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
