const bcrypt = require("bcryptjs");
const jwt = require("jsonwebtoken");
const { MongoClient } = require("mongodb");
const url = "mongodb://127.0.0.1:27017";
const client = new MongoClient(url);
const db = client.db("IZ3_Chat");
const collectionPassword = db.collection("PasswordClient");

const jwtSecret =
  "4715aed3c946f7b0a38e6b534a9583628d84e96d10fbc04700770d572af3dce43625dd";
exports.register = async (req, res, next) => {
  const { Login, Password } = req.body;
  let person = await collectionPassword.find({ Login: Login }).toArray();
  if (Password.length < 6) {
    return res.status(400).json({ message: "Password less than 6 characters" });
  }
  if (person.length !== 0) {
    return res
      .status(400)
      .json({ message: "This login already is have existence" });
  }
  bcrypt.hash(Password, 10).then(async (hash) => {
    await collectionPassword
      .insertOne({
        Login: Login,
        Password: hash,
        Message: { "-1": "" },
      })
      .then((user) => {
        const maxAge = 3 * 60 * 60;
        let id = user.insertedId;
        const token = jwt.sign({ id: id, Login }, jwtSecret, {
          expiresIn: maxAge, // 3hrs
        });
        res.cookie("jwt", token, {
          httpOnly: true,
          maxAge: maxAge * 1000,
        });
        res.status(201).json({
          message: "User successfully created",
          user: user.insertedId,
        });
      })
      .catch((error) =>
        res.status(400).json({
          message: "User not successful created",
          error: error.message,
        })
      );
  });
};
exports.cow = async (req, res, next) => {
  const token = req.cookies.jwt;
  if (token) {
    jwt.verify(token, jwtSecret, (err, decodedToken) => {
      if (err) {
        return false;
      } else {
        console.log("asdas");
      }
    });
  }
};
exports.login = async (req, res, next) => {
  const { Login, Password } = req.body;

  // Check if username and password is provided
  if (!Login || !Password) {
    return res.status(400).json({
      message: "Username or Password not present",
    });
  }

  try {
    let person = await collectionPassword.find({ Login: Login }).toArray();

    if (person.length !== 1) {
      res.status(400).json({
        message: "Login not successful",
        error: "User not found",
      });
    } else {
      // comparing given password with hashed password
      bcrypt.compare(Password, person[0].Password).then(function (result) {
        if (result) {
          const maxAge = 3 * 60 * 60;
          const token = jwt.sign({ id: person[0]._id, Login }, jwtSecret, {
            expiresIn: maxAge, // 3hrs in sec
          });
          res.cookie("jwt", token, {
            httpOnly: true,
            maxAge: maxAge * 1000, // 3hrs in ms
          });
          res.status(201).json({
            message: "User successfully Logged in",
            user: person[0]._id,
          });
        } else {
          res.status(400).json({ message: "Login not succesful" });
        }
      });
    }
  } catch (error) {
    res.status(400).json({
      message: "An error occurred",
      error: error.message,
    });
  }
};
exports.getUsers = async () => {
  let clients = [];
  clients = await collectionPassword.find().toArray();
  for (let i = 0; i < clients.length; i++) {
    clients[i] = clients[i].Login;
  }
  return clients;
};
exports.saveMessageClient = async (login, message) => {
  await collectionPassword.updateOne(
    { Login: login },
    { $set: { Message: message } }
  );
};
exports.getMessageClient = async (login) => {
  let client = await collectionPassword.findOne({ Login: login });
  return client.Message;
};
exports.takeNewMessageClient = async (message, login, name) => {
  let person = await collectionPassword.find({ Login: name }).toArray();
  person[0].Message[login] += `<div class="serveroutput">${message}</div>`;
  await collectionPassword.updateOne(
    { Login: name },
    { $set: { Message: person[0].Message } }
  );
};
