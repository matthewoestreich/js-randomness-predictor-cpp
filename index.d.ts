declare module "js-randomness-predictor-cpp" {
  interface NodeJsVersion {
    major: number;
    minor: number;
    patch: number;
  }

  interface V8Predictor {
    predictNext(): number;
    get sequence(): number[];
    get nodeVersion(): NodeJsVersion;
    // Only use this if you know what you're doing! This allows you to change
    // the predictor so it is able to predict usingg sequences generated in 
    // different versions of Node.js.
    set nodeVersion(version: NodeJsVersion);
  }

  interface ChromePredictor {
    predictNext(): number;
    get sequence(): number[];
  }

  interface FirefoxPredictor {
    predictNext(): number;
    get sequence(): number[];
  }

  function v8(sequence?: number[]): V8Predictor;
  function chrome(sequence: number[]): ChromePredictor;
  function firefox(sequence: number[]): FirefoxPredictor;

  const JSRandomnessPredictorCpp: {
    v8: typeof v8;
    chrome: typeof chrome;
    firefox: typeof firefox;
  };

  export = JSRandomnessPredictorCpp;
}
