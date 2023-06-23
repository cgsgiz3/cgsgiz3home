const express = require("express");
const router = express.Router();

const { register, login, cow } = require("./auth");

router.route("/register").post(register);
router.route("/login").post(login);
router.route("/cow").post(cow);
module.exports = router;
