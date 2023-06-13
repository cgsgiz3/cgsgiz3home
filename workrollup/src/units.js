/* eslint-disable @typescript-eslint/no-empty-function */
/* eslint-disable @typescript-eslint/no-this-alias */
//import { createshader } from "./shader.js";
//import { createprimitive } from "./prim.js";
//import { mat4 } from "./math.js";
//import { mtlCreate } from "./material.js";
//import { Timer } from "./timer.js";
//import { primcreate, primdraw } from "./prim.js";

let majorUnit;
export function units() {
  return new newUnits();
}
export function unitHandle(event) {
  majorUnit.shaderPromise.push(event);
}
function newUnits() {
  this.init = () => {};
  this.response = () => {};
  this.render = () => {};
  this.primitive = {};
  this.shaderPromise = [];
  /*
  this.uniform = {};
  this.material = {};
  this.idCanvas = "glCanvas";
  this.inctance = 1;
  */
  this.units = async () => {
    majorUnit = this;
    this.init();
    return Promise.all([...this.shaderPromise])
      .then(() => {
        let draw = () => {
          try {
            this.response();
            this.render();
          } catch {
            console.error("Error render");
            return;
          }
          window.requestAnimationFrame(draw);
          /*
          window.requestAnimationFrame(draw);
          window.requestAnimationFrame(draw);
          window.requestAnimationFrame(draw);
          window.requestAnimationFrame(draw);
          */
          //setTimeout(draw(), Infinity);
        };
        window.requestAnimationFrame(draw);

        /*
        let promise;
        function draw() {
          promise = unitRender();
          //window.requestAnimationFrame(draw);
          //setTimeout(draw(), 1);
          window.requestAnimationFrame(draw);
          return promise;
        }
        */
        /*
        draw();
        promise
          .then(() => {
            window.requestAnimationFrame(draw);
            //draw();
            //window.requestAnimationFrame(draw);
            //promise = unitRender();
          })
          .catch(() => {
            console.log("Error render");
          });
        //draw();
        //window.requestAnimationFrame(draw);
        return promise;
        */
      })
      .catch(() => {
        console.log("Error initialization");
      });
  };
}
