import fs from "node:fs";
import path from "node:path";

const ROOT_FOLDER = path.resolve(import.meta.dirname, "../src");
const PROJECT_ROOT = path.resolve(import.meta.dirname, "..");
const cppFiles = getAllFilesWithExtensionRecursively(ROOT_FOLDER, ".cpp");

// Important: Use forward slashes so Windows GYP doesn't choke
console.log(cppFiles.map((p) => p.replace(/\\/g, "/")).join(" "));

function getAllFilesWithExtensionRecursively(dir, ext) {
  const results = [];

  for (const entry of fs.readdirSync(dir, { withFileTypes: true })) {
    const fullPath = path.join(dir, entry.name);

    if (entry.isDirectory()) {
      results.push(...getAllFilesWithExtensionRecursively(fullPath, ext));
    } else if (entry.isFile() && fullPath.endsWith(ext)) {
      // Return relative path from project root (for binding.gyp)
      results.push(path.relative(PROJECT_ROOT, fullPath));
    }
  }

  return results;
}
