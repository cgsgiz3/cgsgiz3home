const http = require("http");
const express = require("express");
const morgan = require("morgan");
const { Server } = require("socket.io");
const cookieParser = require("cookie-parser");
const fs = require("fs");
const { userAuth, getClientName } = require("../middleware/auth.js");
const {
  getNewMessageClient,
  getUsers,
  getMessageClient,
  saveMessageClient,
  takeNewMessageClient,
} = require("../Auth/auth.js");

const app = express();
const server = http.createServer(app);
const io = new Server(server);
app.use(morgan("combined"));
app.use(express.json());
app.use(cookieParser());
//app.use("/basic", userAuth, express.static("../views/chat"));
//app.use("/", express.static("../views/beginning"));
app.use("/", express.static("../views/chat"));
app.use("/basic", express.static("../views/chat"));

app.use("/api/auth", require("../Auth/route.js"));
app.get("/logout", (req, res) => {
  res.cookie("jwt", "", { maxAge: "1" });
  res.redirect("/");
});
let clientsonline = {};
//let clientscross = [];
console.primcow = {};
console.primtree = {};
console.primgrid = {};
console.primsky = {};
//21.763290405273438,
//21.763290405273438
let vectorEnd = {
  x: 13.18665599822998,
  y: 6.5551551170647135,
  z: -0.08041954040527366,
};
let vectorStart = {
  x: -8.576634407043457,
  y: 6.5551551170647135,
  z: -0.08041954040527366,
};

let numOfInstance = 1;
let numOfClientOnline = 0;
let vecMulMatr = function (mtr) {
  let v = [];
  let w =
    vectorEnd.x * mtr.massiv[0][3] +
    vectorEnd.y * mtr.massiv[1][3] +
    vectorEnd.z * mtr.massiv[2][3] +
    mtr.massiv[3][3];

  v[0] = {};
  v[0].x =
    (vectorEnd.x * mtr.massiv[0][0] +
      vectorEnd.y * mtr.massiv[1][0] +
      vectorEnd.z * mtr.massiv[2][0] +
      mtr.massiv[3][0]) /
    w;
  v[0].y =
    (vectorEnd.x * mtr.massiv[0][1] +
      vectorEnd.y * mtr.massiv[1][1] +
      vectorEnd.z * mtr.massiv[2][1] +
      mtr.massiv[3][1]) /
    w;
  v[0].z =
    (vectorEnd.x * mtr.massiv[0][2] +
      vectorEnd.y * mtr.massiv[1][2] +
      vectorEnd.z * mtr.massiv[2][2] +
      mtr.massiv[3][2]) /
    w;
  w =
    vectorStart.x * mtr.massiv[0][3] +
    vectorStart.y * mtr.massiv[1][3] +
    vectorStart.z * mtr.massiv[2][3] +
    mtr.massiv[3][3];

  v[1] = {};
  v[1].x =
    (vectorStart.x * mtr.massiv[0][0] +
      vectorStart.y * mtr.massiv[1][0] +
      vectorStart.z * mtr.massiv[2][0] +
      mtr.massiv[3][0]) /
    w;

  v[1].y =
    (vectorStart.x * mtr.massiv[0][1] +
      vectorStart.y * mtr.massiv[1][1] +
      vectorStart.z * mtr.massiv[2][1] +
      mtr.massiv[3][1]) /
    w;
  v[1].z =
    (vectorStart.x * mtr.massiv[0][2] +
      vectorStart.y * mtr.massiv[1][2] +
      vectorStart.z * mtr.massiv[2][2] +
      mtr.massiv[3][2]) /
    w;
  return v;
};
function crossCow(socketvector, clientvector) {
  let X10 = socketvector[0].x,
    Y10 = socketvector[0].z,
    X11 = socketvector[1].x,
    Y11 = socketvector[1].z;
  let X20 = clientvector[0].x,
    Y20 = clientvector[0].z,
    X21 = clientvector[1].x,
    Y21 = clientvector[1].z,
    T1,
    T2,
    A = (X20 - X10) / (X11 - X10),
    B = (X21 - X20) / (X11 - X10),
    C = (Y21 - Y20) / (Y11 - Y10),
    D = (Y20 - Y10) / (Y11 - Y10);
  T2 = (A - D) / (C - B);
  T1 = (X20 + (X21 - X20) * T2 - X10) / (X11 - X10);
  if (X11 - X10 == 0) {
    if (X21 - X20 == 0) return false;
    T2 = (X10 - X20) / (X21 - X20);
    T1 = 0;
  }
  if (Y11 - Y10 == 0) {
    if (Y21 - Y20 == 0) return false;
    T2 = (Y10 - Y20) / (Y21 - Y20);
  }
  if (Y21 - Y20 == 0) T1 = (Y20 - Y10) / (Y11 - Y10);
  if (T1 < 1 && T1 > 0 && T2 < 1 && T2 > 0) {
    return true;
  }
}
io.on("connection", async (socket) => {
  if (socket.handshake.headers.cookie) {
    socket.login = await getClientName(
      socket.handshake.headers.cookie.split("=")[1]
    );
  }
  socket.login = socket.id;
  if (socket.login) {
    socket.login = socket.id;
    numOfClientOnline++;
    for (let client in clientsonline) {
      client = clientsonline[client];
      socket.emit("getOtherPrim", [client.world, client.login]);
    }
    socket.on("takeSocketPrim", (world) => {
      socket.world = world;
      clientsonline[socket.login] = socket;
      let socketvector = vecMulMatr(world);
      for (let client in clientsonline) {
        client = clientsonline[client];
        if (client === socket) {
          continue;
        }
        let clientvector = vecMulMatr(client.world);
        if (clientvector[0].y.toFixed(2) === socketvector[0].y.toFixed(2)) {
          if (crossCow(socketvector, clientvector)) {
            delete clientsonline[client.login];
            numOfClientOnline--;
            for (let liveclient in clientsonline) {
              liveclient = clientsonline[liveclient];
              liveclient.emit("deleteOtherPrim", client.login);
            }
            client.emit("clietnLoseFromServer");
          }
        }
        client.emit("getOtherPrim", [socket.world, socket.login]);
      }
    });
    socket.on("disconnect", () => {
      delete clientsonline[socket.login];
      numOfClientOnline--;
      for (let client in clientsonline) {
        client = clientsonline[client];
        client.emit("deleteOtherPrim", socket.login);
      }
    });
  }
});

//app.get("/chat", (req, res) => {
//  res.sendFile(
//    "Z:\\2-express-socketio-rollup\\views\\chat" + "\\index.html"
//  );
//});
//app.get("/chat/styles.css", function (req, res) {
//  res.sendFile(
//    "Z:\\2-express-socketio-rollup\\views\\chat" + "\\styles.css"
//  );
//});
//app.get("/chat/bundle.js", function (req, res) {
//  res.sendFile(
//    "Z:\\2-express-socketio-rollup\\views\\chat" + "\\bundle.js"
//  );
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
