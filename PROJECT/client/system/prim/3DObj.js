//import {motobike} from "./models/motobike.js"
import { cow } from "./models/cow.js";
import { tree } from "./models/tree.js";
export function objLoadSync(file) {
  let primitive = {};
  let db;
  if (file === "cow") {
    db = cow;
    primitive.file = "cow";
    primitive.type = "trimesh";
    primitive.instance = 1;
  } else if (file === "tree") {
    db = tree;
    primitive.file = "tree";
    primitive.instance = 400;
    primitive.type = "tristrip";
  }

  primitive.buffer = {};
  primitive.uniform = {
    materialBuffer: sys.mtlGetDef(),
    cameraBuffer: () => {
      return [
        [...sys.loc.array(), 1],
        [...sys.at.array(), 1],
        [...sys.dir.array(), 1],
        [...sys.right.array(), sys.startelevator],
        [...sys.up.array(), sys.startazimuth],
        [sys.localTime, sys.globalDeltaTime, sys.azimuth, sys.elevator],
      ];
    },
  };
  let text = db.split("\n");
  let nv = 0,
    nf = 0;
  let I = [],
    V = [];
  for (let i = 0; i < text.length; i++) {
    let massiv = text[i].split(" ");
    if (massiv[0] === "v") {
      for (let j = 1; j < massiv.length; j++) {
        let number = parseFloat(massiv[j]);
        V.push(number);
      }
      nv++;
    } else if (massiv[0] === "f") {
      let n = 0,
        c0 = 0,
        c = 0,
        c1 = 0;
      for (let j = 1; j < massiv.length; j++) {
        c = massiv[j].split("//")[0];
        if (c < 0) {
          c = nv + c;
        } else {
          c--;
        }
        if (n == 0) {
          c0 = c;
        } else if (n == 1) {
          c1 = c;
        } else {
          I[nf++] = c0;
          I[nf++] = c1;
          I[nf++] = c;
          c1 = c;
        }
        n++;
      }
    }
  }
  //primitive.type = "tristrip";
  primitive.buffer.in_pos = V;
  primitive.buffer.index = I;
  primitive.buffer.in_normal = sys.autoNormals(V, I, primitive.type);

  new sys.createprimitive(primitive);
  return primitive;
}
