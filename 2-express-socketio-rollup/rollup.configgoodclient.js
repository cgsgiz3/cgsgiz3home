const resolve = require("@rollup/plugin-node-resolve"); // иметь возможность работать с модулямииз nodejs
const commonjs = require("@rollup/plugin-commonjs"); // иметь возможность работать с модулями любого типа

module.exports = {
  input: "client/client.js",
  output: {
    file: "dist/chat/bundle.js",
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
  ],
};
