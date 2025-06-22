#!/bin/bash

# Path to your clang-format binary
CLANG_FORMAT=clang-format
# Root directory to recursively run clang-format on
ROOT_DIRECTORY=src/

# Exit if clang-format is not installed
if ! command -v $CLANG_FORMAT &> /dev/null; then
  echo "Error: $CLANG_FORMAT is not installed or not in PATH."
  exit 1
fi

# Format all .cpp and .hpp files under ROOT_DIRECTORY
find "$ROOT_DIRECTORY" -type f \( -name "*.cpp" -o -name "*.hpp" \) -print0 | while IFS= read -r -d '' file; do
  echo "Formatting $file"
  $CLANG_FORMAT -style=file -i "$file"
done