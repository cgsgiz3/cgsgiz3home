const resolve = require("@rollup/plugin-node-resolve"); // иметь возможность работать с модулямииз nodejs
const commonjs = require("@rollup/plugin-commonjs"); // иметь возможность работать с модулями любого типа
const glslify = require("rollup-plugin-glslify");

module.exports = {
  input: "client/goodclient.js",
  output: {
    file: "views/chat/bundle.js",
    format: "iife",
    sourcemap: "inline",
  },
  plugins: [
    resolve({
      jsnext: true,
      main: true,
      browser: true,
    }),
    commonjs(),
    glslify({ exclude: "node_modules/**" }),
  ],
};
