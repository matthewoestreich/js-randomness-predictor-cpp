/**
 *
 * Used by `npm test` to build and run test files located in "tests/" dir
 *
 */

import { execSync } from "child_process";
import { cwd, chdir } from "process";
import path from "path";
import fs from "fs";

const CURRENT_DIR = cwd();

// Change into the `tests` directory
const testsDir = path.join(CURRENT_DIR, "tests");
chdir(testsDir);

// Create `build` directory if it doesn't exist
if (!fs.existsSync("build")) {
  fs.mkdirSync("build");
}

// Run cmake configure
execSync("cmake -B build", { stdio: "inherit" });
// Build test binary
execSync("cmake --build build", { stdio: "inherit" });
// Run the test binary
execSync("./build/JSRandomnessPredictorTests --success --colour-mode ansi", { stdio: "inherit" });
