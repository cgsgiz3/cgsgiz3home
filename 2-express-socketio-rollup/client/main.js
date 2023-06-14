import { io } from "socket.io-client";

async function main() {
  const socket = io();

  // client-side
  socket.on("connect", () => {
    console.log(socket.id); // x8WIv7-mJelg7on_ALbx
    socket.on("MessageFromServer", function (msg) {
      let server = document.getElementById("cleint-server-chat");
      server.innerHTML =
        server.innerHTML + `<div class="serveroutput">${msg}</div>`;
    });
  });

  socket.on("disconnect", () => {
    console.log(socket.id); // undefined
  });

  document.getElementById("id1").onkeyup = (ev) => {
    if (ev.code === "Enter") {
      const value = document.getElementById("id1").value;
      console.log(value);
      let server = document.getElementById("cleint-server-chat");
      server.innerHTML =
        server.innerHTML + `<div class="clientoutput">${value}</div>`;
      document.getElementById("id1").value = "";
      socket.emit("MessageToServer", value);
    }
  };
}

window.addEventListener("load", (event) => {
  main();
});
