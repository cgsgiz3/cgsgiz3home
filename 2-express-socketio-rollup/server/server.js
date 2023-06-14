const http = require("http");
const express = require("express");
const morgan = require("morgan");
const { Server } = require("socket.io");

const app = express();
app.use(morgan("combined"));
app.use(express.static("."));

//initialize a simple http server
const server = http.createServer(app);
const io = new Server(server);

const clients = [];

io.on("connection", (socket) => {
  clients.push(socket);
  console.log(`Client connected with id: ${socket.id}`);
  socket.on("MessageToServer", (msg) => {
    const replyMsg = `Message from client: ${socket.id} is ${msg}`;
    console.log(replyMsg);
    for (client of clients) {
      if (client === socket) {
        continue;
      }
      client.emit("MessageFromServer", msg);
    }
  });
  socket.on("disconnect", () => {
    console.log(`Client disconnected with id: ${socket.id}`);
    const index = clients.indexOf(socket);
    if (index > -1) {
      clients.splice(index, 1);
    }
  });
});

server.listen(process.env.PORT || 8080, () => {
  console.log(`Server started on port ${server.address().port} :)`);
});

//<style>
//textarea {
//    resize: none;
//    height: 200px;
//    width: 200px;
//}
//</style>
//<p style="border:3px #00B344  ridge; width:200px;">
//</p>

//<img src="http://placekitten.com/40/50" alt="photo" class="image"></img>
//<div>
//<textarea cols="30" rows="30" name="Ivan" placeholder="Go to chat" id="id1" ></textarea>
//</div>

//<div class="serveroutput"> Текстовая область</div>
//<div class="clientoutput"> Всем привет</div>
