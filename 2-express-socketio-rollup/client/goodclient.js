import { io, Manager } from "socket.io-client";

async function main() {
  let socket = io();
  socket.on("connect", () => {
    socket.on("MessageFromServer", function (msg) {
      let server = document.getElementById("cleint-server-chat");
      server.innerHTML =
        server.innerHTML + `<div class="serveroutput">${msg}</div>`;
    });
    socket.on("ClientsToMessage", function (clientlogin) {
      let clients = document.getElementById("add-clients");
      clients.innerHTML = clients.innerHTML + `<option>${clientlogin}</option>`;
    });
    socket.emit("GoodClientConnectedToServer", socket);
  });
  //document.getElementById("input").onkeyup = (ev) => {
  //  if (ev.code === "Enter") {
  //    const value = document.getElementById("input").value;
  //    document.getElementById("input").value = "";
  //    if (value === "\n") {
  //      return;
  //    }
  //    let today = new Date();
  //    let server = document.getElementById("cleint-server-chat");
  //    server.innerHTML =
  //      server.innerHTML +
  //      `<div class="clientoutput" style="text-align: right; width:500px;">${value}</div>`;
  //    socket.emit("MessageToServer", value);
  //  }
  //};
}
window.addEventListener("load", (event) => {
  main();
});
