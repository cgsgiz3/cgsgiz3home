const http = require("http");
const express = require("express");
const morgan = require("morgan");
const { Server } = require("socket.io");
const { MongoClient } = require("mongodb");
const url = "mongodb://127.0.0.1:27017";
const client = new MongoClient(url);
//const collectionMessage = db.collection("MessageClient");

//let router = express.Router();
//router.get("/", (req, res) => {
//  res.sendFile("Z:\\2-express-socketio-rollup\\dist\\beginning\\index.html");
//});
//app.use(
//  "/",
//  express.static("Z:\\2-express-socketio-rollup\\dist\\beginning\\")
//);

//app.use(express.static(file));
//file = "Z:\\cgsgiz3home\\2-express-socketio-rollup\\" + "dist\\bundle.js";
//app.use(express.static(file));
//app.get("/", function (req, res) {
//  res.sendFile("Z:\\2-express-socketio-rollup\\dist\\beginning\\index.html");
//});
//initialize a simple http server
const db = client.db("IZ3_Chat");
const collectionPassword = db.collection("PasswordClient");

const app = express();
app.use(morgan("combined"));
let file = "Z:\\2-express-socketio-rollup\\dist\\beginning";
app.use(express.static(file));
//app.get("/", (req, res) => {
//  res.sendFile(file + "\\index.html");
//});
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
      let person = await collectionPassword.find({ Login: msg[0] }).toArray();
      if (person.length !== 0) {
        socket.emit("ClientRegisterFromServer", false);
      } else {
        await collectionPassword.insertOne({
          Login: msg[0],
          Password: msg[1],
        });
        //        app.use(
        //          `${socket.id}`,
        //          express.static("Z:\\2-express-socketio-rollup\\dist\\chat")
        //        );
        app.get(`/styles.css`, function (req, res) {
          res.sendFile(
            "Z:\\2-express-socketio-rollup\\dist\\chat" + "\\styles.css"
          );
        });
        app.get(`/${socket.id}`, (req, res) => {
          res.sendFile(
            "Z:\\2-express-socketio-rollup\\dist\\chat" + "\\index.html"
          );
        });
        app.get(`${socket.id}/bundle.js`, function (req, res) {
          res.sendFile(
            "Z:\\2-express-socketio-rollup\\dist\\chat" + "\\bundle.js"
          );
        });
        socket.emit("ClientRegisterFromServer", `${socket.id}`);
        for (let client of clients) {
          client.emit("ClientsToMessage", socket.login);
        }
        for (let client of clients) {
          socket.emit("ClientsToMessage", client.login);
        }
        clients.push(socket);
        socket.on("MessageToServer", (msg, client) => {
          const replyMsg = `Message from client: ${socket.id} is ${msg}`;
          console.log(replyMsg);
          client.emit("MessageFromServer", msg);
        });
        socket.on("disconnect", () => {
          console.log(`Client disconnected with id: ${socket.id}`);
          const index = clients.indexOf(socket);
          if (index > -1) {
            clients.splice(index, 1);
          }
        });
      }
    } else {
      socket.emit("ClientRegisterFromServer", false);
    }
  });
  socket.on("disconnect", () => {
    console.log(`Client disconnected with id: ${socket.id}`);
  });
});

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
