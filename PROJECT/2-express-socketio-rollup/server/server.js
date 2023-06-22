const http = require("http");
const express = require("express");
const morgan = require("morgan");
const { Server } = require("socket.io");

const app = express();
app.use(morgan("combined"));
let file = __dirname.split("\\");
file.pop();
file = file.join("\\") + "\\dist";
app.use(express.static(file));
const server = http.createServer(app);
const io = new Server(server);
const clients = [];
io.on("connection", (socket) => {});

server.listen(process.env.PORT || 8080, () => {
  console.log(`Server started on port ${server.address().port} :)`);
});
