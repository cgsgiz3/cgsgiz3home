/* eslint-disable @typescript-eslint/no-var-requires */
/* eslint-disable no-undef */
const glslify = require("rollup-plugin-glslify");
const json = require("@rollup/plugin-json");
const eslint = require("@rollup/plugin-eslint");
const babel = require("@rollup/plugin-babel");
const commonjs = require("@rollup/plugin-commonjs");
const resolve = require("@rollup/plugin-node-resolve");
const terser = require("@rollup/plugin-terser");

module.exports = {
  input: "src/main.js",
  output: {
    dir: "dist",
    format: "iife",
    sourcemap: "inline",
    name: "main.js",
  },
  plugins: [
    json(),
    glslify({ exclude: "node_modules/**" }),
    eslint({ exclude: "src/*.glsl" }),
    babel({
      exclude: "node_modules/**",
    }),
    commonjs(),
    resolve({
      main: true,
      browser: true,
      jsnext: true,
    }),
    terser(),
  ],
};
