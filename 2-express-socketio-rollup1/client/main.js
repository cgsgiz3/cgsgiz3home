import { io } from "socket.io-client";

async function main() {
  const socket = io();
  socket.on("connect", () => {
    console.log(socket.id); // x8WIv7-mJelg7on_ALbx
    socket.on("MessageFromServer", function (msg) {
      let server = document.getElementById("cleint-server-chat");
      server.innerHTML =
        server.innerHTML + `<div class="serveroutput">${msg}</div>`;
    });
    socket.on("ClientRegisterFromServer", function (exit) {
      if (exit) {
        //window.location.href = window.location.href + "/chat";
        //document.getElementById("id1").onkeyup = (ev) => {
        //  if (ev.code === "Enter") {
        //    const value = document.getElementById("id1").value;
        //    document.getElementById("id1").value = "";
        //    if (value === "\n") {
        //      return;
        //    }
        //    //console.log(value);
        //    let today = new Date();
        //    let server = document.getElementById("cleint-server-chat");
        //    //${today.getFullYear()}/${today.getMonth()}/${today.getDate()}
        //    server.innerHTML =
        //      server.innerHTML +
        //      `<div class="clientoutput" style="text-align: right; width:500px;">${value}</div>`;
        //    //server.innerHTML =
        //    //  server.innerHTML +
        //    //  `<p class="clientoutput" style="border: 1px #00b344 solid;  position: relative; left: ${
        //    //    500 + 500 - width
        //    //  }px; width: ${width}px; height: ${height}px">${value}</p>`;
        //    socket.emit("MessageToServer", value);
        //  }
        //};
      } else {
        document.getElementById("id1").value =
          "Login or password is not correct";
      }
    });
  });

  socket.on("disconnect", () => {
    console.log(socket.id); // undefined
  });
  document.getElementById("register").onclick = function () {
    socket.emit("ClientRegisterToServer", [
      document.getElementById("login").value,
      document.getElementById("password").value,
    ]);
  };
}
window.addEventListener("load", (event) => {
  main();
});
