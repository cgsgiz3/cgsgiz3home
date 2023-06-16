import { io, Manager } from "socket.io-client";

async function main() {
  let socket = io();
  socket.on("connect", () => {
    socket.on("ClientRegisterFromServer", function (entrance) {
      if (entrance) {
        window.location.href = window.location.href + entrance;
      } else {
        document.getElementById("entrance").innerHTML =
          "Login or password is incorrect";
      }
    });
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
