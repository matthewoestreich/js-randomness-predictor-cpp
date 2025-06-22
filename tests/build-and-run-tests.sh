# MAKE SURE YOU ARE IN DIRECTORY "tests/"
CURRENT_DIR=$(pwd)
ROOT_DIR="/js-randomness-predictor-cpp"
BASENAME="/$(basename $CURRENT_DIR)"
if [ "$BASENAME" = "$ROOT_DIR" ]; then
  cd tests
else
  echo "Error: Please go to root of project and run 'npm test'"
  exit 1
fi
# Configure build
cmake -B build
# Build files
cmake --build build
# Run tests
./build/JSRandomnessPredictorTests --success --colour-mode ansi