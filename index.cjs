const path = require("path");
const nodeGypBuild = require("node-gyp-build");
module.exports = nodeGypBuild(path.resolve(__dirname));
