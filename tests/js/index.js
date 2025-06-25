/**
 *
 * This code transitively tests the native C++ wrappers by importing the built .node addon files and using the code within them.
 *
 * This code is designed to be called from GitHub Actions - ideally, a matrix job - so we can test on all platforms.
 *
 */

const { describe, it } = require("node:test");
const assert = require("node:assert");
const os = require("os");
const JSRandomnessPredictor = require(`../../prebuilds/${os.platform()}-${os.arch()}/js-randomness-predictor-cpp.node`);

describe("V8", () => {
  it("should generate a random sequence and predict the next numbers", () => {
    const v8 = JSRandomnessPredictor.v8();
    const V8_PREDICTIONS = Array.from({ length: 10 }, () => v8.predictNext());
    const V8_EXPECTED = Array.from({ length: 10 }, Math.random);
    assert.deepStrictEqual(V8_EXPECTED, V8_PREDICTIONS);
  });
  describe("V8 : Node.js v22.0.0", () => {
    it("should predict the next random numbers", () => {
      const sequence_v22_0_0 = [0.36280726230126614, 0.32726837947512855, 0.22834780314989023, 0.18295517908119385];
      const expected_v22_0_0 = [
        0.8853110028441145, 0.14326940888839124, 0.035607792006009165, 0.6491231376351401, 0.3345277284146617, 0.42618019812863417,
      ];
      const v8 = JSRandomnessPredictor.v8(sequence_v22_0_0);
      v8.nodeVersion = { major: 22, minor: 0, patch: 0 };
      const predictions = Array.from({ length: expected_v22_0_0.length }, () => v8.predictNext());
      assert.deepStrictEqual(predictions, expected_v22_0_0);
    });
  });
  describe("V8 : Node.js v24.2.0", () => {
    it("should predict the next random numbers", () => {
      const sequence_v24_2_0 = [0.01800425609760259, 0.19267361208155598, 0.9892770985784053, 0.49553307275603264, 0.7362624704291061];
      const expected_v24_2_0 = [
        0.8664993194151147, 0.5549329443482626, 0.8879559862322086, 0.9570142746667122, 0.7514661363382521, 0.9348208735728415,
      ];
      const v8 = JSRandomnessPredictor.v8(sequence_v24_2_0);
      v8.nodeVersion = { major: 24, minor: 2, patch: 0 };
      const predictions = Array.from({ length: expected_v24_2_0.length }, () => v8.predictNext());
      assert.deepStrictEqual(predictions, expected_v24_2_0);
    });
  });
});

describe("Firefox", () => {
  it("should predict the next random numbers", () => {
    const FIREFOX_SEQUENCE = [0.1321263101773572, 0.03366887439746058, 0.032596957696410134, 0.9986575482138969, 0.8479779907956815];
    const FIREFOX_EXPECTED = [
      0.13963871472821332, 0.25068024611907636, 0.6656237481612675, 0.7381091878692425, 0.8709382509549467, 0.49171337524788294, 0.6991749430716799,
      0.9530887478758369, 0.781511163650037, 0.699311162730038,
    ];
    const firefox = JSRandomnessPredictor.firefox(FIREFOX_SEQUENCE);
    const FIREFOX_PREDICTIONS = Array.from({ length: FIREFOX_EXPECTED.length }, () => firefox.predictNext());
    assert.deepStrictEqual(FIREFOX_EXPECTED, FIREFOX_PREDICTIONS);
  });
});

describe("Chrome", () => {
  it("should predict the next random numbers", () => {
    const CHROME_SEQUENCE = [0.32096095967729477, 0.3940071672626849, 0.3363374923027722, 0.7518761096243554, 0.44201420586496387];
    const CHROME_EXPECTED = [
      0.8199006769436774, 0.6250240806313154, 0.9101975676132608, 0.5889203398264599, 0.5571161440436232, 0.9619184649129092, 0.8385620929536599,
      0.3822042053588621, 0.5040552869863579, 0.12014019399083042, 0.44332968383610927, 0.37830079319230936, 0.542449069899975, 0.0659240460476268,
      0.9589494984837686, 0.007621633090565627, 0.14119301022498787, 0.9964718645470699, 0.14527130036353442, 0.6260597083849548, 0.86354903522581,
      0.7245123107811886, 0.6565323828155891, 0.3636039851663503, 0.5799453712253447,
    ];

    const chrome = JSRandomnessPredictor.chrome(CHROME_SEQUENCE);
    const CHROME_PREDICTIONS = Array.from({ length: CHROME_EXPECTED.length }, () => chrome.predictNext());
    assert.deepStrictEqual(CHROME_EXPECTED, CHROME_PREDICTIONS);
  });
});
