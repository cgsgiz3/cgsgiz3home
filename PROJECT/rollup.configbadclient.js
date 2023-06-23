const resolve = require("@rollup/plugin-node-resolve"); // иметь возможность работать с модулямииз nodejs
const commonjs = require("@rollup/plugin-commonjs"); // иметь возможность работать с модулями любого типа

module.exports = {
  input: "client/badclient.js",
  output: {
    file: "views/beginning/bundle.js",
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
