# js-randomness-predictor-cpp

[![npm version](https://img.shields.io/npm/v/js-randomness-predictor-cpp.svg?logo=npm&color=cb0000)](https://www.npmjs.com/package/js-randomness-predictor-cpp)

Node.js native addon (C++) for high-performance Math.random() prediction in V8 (Node), Chrome, and Firefox

# Installation

Use your favorite package manager.

```bash
npm i js-randomness-predictor-cpp
yarn add js-randomness-predictor-cpp
pnpm add js-randomness-predictor-cpp
# etc...
```

# Usage

**IMPORTANT**:

- You must use the appropriate predictor for the environment used to generate the initial sequence. **Meaning, if you generated the sequence in Chrome, you must use the Chrome predictor, etc..**
- We recommend at least 4 numbers in the initial sequence.

```js
// ESM
import JSRandomnessPredictor from "js-randomness-predictor-cpp";
// CJS
const JSRandomnessPredictor = require("js-randomness-predictor-cpp");
```

## Chrome Predictor

```js
const chromePredictor = JSRandomnessPredictor.chrome([...]);
const nextPrediction = await chromePredictor.predictNext();
// You'll need to manually verify accuracy.
```

## Firefox Predictor

```js
const firefoxPredictor = JSRandomnessPredictor.firefox([...]);
const nextPrediction = await firefoxPredictor.predictNext();
// You'll need to manually verify accuracy.
```

#### FIREFOX ISSUE WHEN GENERATING NUMBERS IN CONSOLE

You must disable "Instant Evaluation", otherwise your predictions may show incorrectly. Especially if you use more than one call to generate the initial sequence + expected values.

<img width="1920" alt="Firefox_DisableConsoleInstantEvaluation" src="/.github/Firefox_DisableConsoleInstantEvaluation.png" />

**If you do not want to disable "Instant Evaluation"**, you'll need to generate initial sequence + expected values in one command.

So instead of using two (or more) calls to `Math.random`:

```js
/** Pretend this is the console */
// Output used as initial sequence.
Array.from({ length: 4 }, Math.random);
// Output used for validating predictions.
Array.from({ length: 10 }, Math.random);
```

You'll need to do:

```js
/** Pretend this is the console */
// Only use one call! Manually separate numbers!
Array.from({ length: 6 }, Math.random);
[
  // --------------------|
  0.5654163987207667, // |
  0.7409356182179403, // | --> Use "these" numbers as initial sequence
  0.46136469064448193, //|
  0.18124646315195891, //|
  // --------------------|
  0.25678544986069995, // --> Use the rest of the numbers for validation
  0.5543550504255771,
];
```

## Node/V8 Predictor

Since we are running in V8, we can produce the initial sequence dynamically by not providing any parameters to the `v8()` method. This will automatically generate a sequence behind the scenes.

Keep in mind, you can manually provide a sequence as well.

### Node/V8 : Provide Your Own Sequence

```js
const manualSequence = Array.from({ length: 4 }, Math.random);
// You could also generate your sequence via Node REPL and provide it that way.
const manualSequence = [
  /* copy/paste numbers generated via REPL */
];
const v8Predictor = JSRandomnessPredictorCpp.v8(manualSequence);
const nextPrediction = await v8Predictor.predictNext();
// We can programmatically verify since we are running in Node.
const isAccurate = nextPrediction === Math.random();
```

### Node/V8 : Automatically Generate Sequence

```js
// Automatically create sequence behind the scenes because
// parameter not provided to 'v8' method.
const v8Predictor = JSRandomnessPredictor.v8();
const nextPrediction = await v8Predictor.predictNext();
// We can programmatically verify since we are running in Node.
const isAccurate = nextPrediction === Math.random();
```