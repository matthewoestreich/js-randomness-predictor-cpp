declare module "js-randomness-predictor-cpp" {
  interface V8Predictor {
    predictNext(): number;
    sequence: number[];
  }

  interface ChromePredictor {
    predictNext(): number;
    sequence: number[];
  }

  interface FirefoxPredictor {
    predictNext(): number;
    sequence: number[];
  }

  function v8(sequence?: number[]): V8Predictor;
  function chrome(sequence: number[]): ChromePredictor;
  function firefox(sequence: number[]): FirefoxPredictor;

  const JSRandomnessPredictorCpp: {
    v8: typeof v8;
    chrome: typeof chrome;
    firefox: typeof firefox;
  }

  export = JSRandomnessPredictorCpp;
}