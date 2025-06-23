/**
 *
 * This file is used to find all .cpp files in src/ recursively. We interpolate that data into
 * 'binding.gyp' via this line (inside binding.gyp) `"<!@(node scripts/get-cpp-sources-for-binding-gyp.mjs)"`
 *
 * Must return path relative to root of project!
 *
 */

import fs from "node:fs";
import path from "node:path";

// Relative to this files current location
const RELATIVE_PROJECT_ROOT = "..";
// Relative to this files current location
const ROOT_FOLDER = path.resolve(import.meta.dirname, "../src");
const cppFiles = getAllFilesWithExtensionRecursively(ROOT_FOLDER, ".cpp");

// Must log to console so the .gyp file can read it.
console.log(cppFiles.join(" "));

/**
 * @param {string} rootDirectory root path to recursively search
 * @param {string} fileExtension file extension to find
 * @returns {void}
 */
function getAllFilesWithExtensionRecursively(rootDirectory, fileExtension) {
  const results = [];
  const allFilesAndDirectories = fs.readdirSync(rootDirectory, { withFileTypes: true });

  for (const fileOrDirectory of allFilesAndDirectories) {
    const fullPath = path.join(fileOrDirectory.parentPath, fileOrDirectory.name);

    if (fileOrDirectory.isDirectory()) {
      results.push(...getAllFilesWithExtensionRecursively(fullPath, fileExtension));
    } else if (fileOrDirectory.isFile() && fullPath.endsWith(fileExtension)) {
      results.push(path.relative(path.resolve(import.meta.dirname, RELATIVE_PROJECT_ROOT), fullPath).replace("/\\/g", "/"));
    }
  }

  return results;
}
