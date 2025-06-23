#!/usr/bin/env node

/**
 *
 * Used by `npm run format` to run `clang-format` on all cpp/hpp files located in specified directories.
 *
 */

import { execSync, spawnSync } from "child_process";
import { readdir } from "fs/promises";
import path from "path";

const DIRECTORIES_TO_FORMAT_FILES_IN = ["src", "tests"];

const CLANG_FORMAT = "clang-format";
const CLANG_FORMAT_CONFIG = path.resolve(import.meta.dirname, "../.clang-format");

async function checkClangFormat() {
  try {
    execSync(`${CLANG_FORMAT} --version`, { stdio: "ignore" });
  } catch {
    console.error(`Error: ${CLANG_FORMAT} is not installed or not in PATH.`);
    process.exit(1);
  }
}

async function* walk(dir) {
  // Skip any build dirs
  if (dir.endsWith("/build")) {
    return;
  }
  for await (const d of await readdir(dir, { withFileTypes: true })) {
    const entry = path.resolve(dir, d.name);
    if (d.isDirectory()) {
      yield* walk(entry);
    } else if (entry.endsWith(".cpp") || entry.endsWith(".hpp")) {
      yield entry;
    }
  }
}

async function formatFilesInDir(directory) {
  const absDir = path.resolve(directory);
  for await (const filePath of walk(absDir)) {
    console.log(`Formatting ${filePath}`);
    const result = spawnSync(CLANG_FORMAT, [`-style=file:${CLANG_FORMAT_CONFIG}`, "-i", filePath], { stdio: "inherit" });
    if (result.error) {
      console.error(`Failed to format ${filePath}:`, result.error);
    }
  }
}

async function main() {
  await checkClangFormat();
  for (const dir of DIRECTORIES_TO_FORMAT_FILES_IN) {
    await formatFilesInDir(dir);
  }
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
