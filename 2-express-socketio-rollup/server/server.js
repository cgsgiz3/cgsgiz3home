const http = require("http");
const express = require("express");
const morgan = require("morgan");
const { Server } = require("socket.io");
//const collectionMessage = db.collection("MessageClient");

//let router = express.Router();
//router.get("/", (req, res) => {
//  res.sendFile("Z:\\2-express-socketio-rollup\\dist\\beginning\\index.html");
//});

//app.use(express.static(file));
//file = "Z:\\cgsgiz3home\\2-express-socketio-rollup\\" + "dist\\bundle.js";
//app.use(express.static(file));
//app.get("/", function (req, res) {
//  res.sendFile("Z:\\2-express-socketio-rollup\\dist\\beginning\\index.html");
//});
//initialize a simple http server

const app = express();
app.use(morgan("combined"));
let file = __dirname.split("\\");
file.pop();
file = file.join("\\") + "\\dist\\";

app.use(express.static(file + "beginning"));
app.get(`/styles.css`, function (req, res) {
  res.sendFile(file + "chat" + "\\styles.css");
});
app.get("/bundle.js", function (req, res) {
  res.sendFile(file + "chat" + "\\bundle.js");
});

//app.get("/styles.css", function (req, res) {
//  res.sendFile(file + "\\styles.css");
//});
//app.get("/bundle.js", function (req, res) {
//  res.sendFile(file + "\\bundle.js");
//});
const server = http.createServer(app);
const io = new Server(server);
const clients = [];
io.on("connection", (socket) => {
  console.log(`Client connected with id: ${socket.id}`);
  socket.on("ClientRegisterToServer", async (msg) => {
    if (msg[1] !== "" && msg[0] !== "") {
      clients.push(msg[0]);
      app.use(`/${msg[0]}`, function (req, res) {
        //res.redirect(file + "chat");
        //express.static(file + "chat");
        res.sendFile(file + "chat" + "\\index.html");
      });
      //app.get(`/${msg[0]}`, (req, res) => {
      //  //res.sendFile(
      //  //  "Z:\\2-express-socketio-rollup\\dist\\chat" + "\\index.html");
      //});
      socket.emit("ClientRegisterFromServer", `${msg[0]}`);
    } else {
      socket.emit("ClientRegisterFromServer", false);
    }
  });
  socket.on("disconnect", () => {
    console.log(`Client disconnected with id: ${socket.id}`);
  });
  socket.on("GoodClientConnectedToServer", (client) => {
    console.log("connected");
  });
});

//      for (let client of clients)
//      {
//        client.emit("ClientsToMessage", socket.login);
//      }
//      clients.push(socket);
//      socket.on("MessageToServer", (msg, client) => {
//        const replyMsg = `Message from client: ${socket.id} is ${msg}`;
//        console.log(replyMsg);
//        client.emit("MessageFromServer", msg);
//      });
//      socket.on("disconnect", () => {
//        console.log(`Client disconnected with id: ${socket.id}`);
//        const index = clients.indexOf(socket);
//        if (index > -1) {
//          clients.splice(index, 1);
//        }
//      });

//app.get("/chat", (req, res) => {
//  res.sendFile(
//    "Z:\\2-express-socketio-rollup\\dist\\chat" + "\\index.html"
//  );
//});
//app.get("/chat/styles.css", function (req, res) {
//  res.sendFile(
//    "Z:\\2-express-socketio-rollup\\dist\\chat" + "\\styles.css"
//  );
//});
//app.get("/chat/bundle.js", function (req, res) {
//  res.sendFile(
//    "Z:\\2-express-socketio-rollup\\dist\\chat" + "\\bundle.js"
//  );
//});

//for (let client of clients) {
//  client.emit("ClientsToMessage", socket.login);
//}
//for (let client of clients) {
//  socket.emit("ClientsToMessage", client.login);
//}
//clients.push(socket);
//socket.on("MessageToServer", (msg, client) => {
//  const replyMsg = `Message from client: ${socket.id} is ${msg}`;
//  console.log(replyMsg);
//  client.emit("MessageFromServer", msg);
//});

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
