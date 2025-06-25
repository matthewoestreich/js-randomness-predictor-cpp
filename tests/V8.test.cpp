#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "V8Predictor.hpp"

TEST_CASE("V8Predictor : Node.js Version 22.0.0", "[V8Predictor, v22.0.0]") {
  SECTION("[v22.0.0] should correctly predict next random numbers") {
    std::vector<double> sequence_v22_0_0 = {
        0.36280726230126614, 0.32726837947512855, 0.22834780314989023, 0.18295517908119385
    };
    std::vector<double> expected_v22_0_0 = {
        0.8853110028441145,
        0.14326940888839124,
        0.035607792006009165,
        0.6491231376351401,
        0.3345277284146617,
        0.42618019812863417
    };
    NodeVersion v22_0_0 = NodeVersion{22, 0, 0};
    V8Predictor v8{v22_0_0, sequence_v22_0_0};
    std::vector<double> predictions_v22_0_0;

    for (size_t i = 0; i < expected_v22_0_0.size(); ++i) {
      predictions_v22_0_0.push_back(v8.predictNext());
    }

    REQUIRE(expected_v22_0_0 == predictions_v22_0_0);
  }
}

TEST_CASE("V8Predictor : Node.js Version 24.2.0", "[V8Predictor, v24.2.0]") {
  SECTION("[v24.2.0] should correctly predict next random numbers") {
    std::vector<double> sequence_v24_2_0 = {
        0.01800425609760259, 0.19267361208155598, 0.9892770985784053, 0.49553307275603264, 0.7362624704291061
    };
    std::vector<double> expected_v24_2_0 = {
        0.8664993194151147,
        0.5549329443482626,
        0.8879559862322086,
        0.9570142746667122,
        0.7514661363382521,
        0.9348208735728415
    };
    NodeVersion v24_2_0 = NodeVersion{.major = 24, .minor = 2, .patch = 0};
    V8Predictor v8_v24_2_0{v24_2_0, sequence_v24_2_0};
    std::vector<double> predictions_v24_2_0;

    for (size_t i = 0; i < expected_v24_2_0.size(); ++i) {
      predictions_v24_2_0.push_back(v8_v24_2_0.predictNext());
    }

    REQUIRE(expected_v24_2_0 == predictions_v24_2_0);
  }
}

TEST_CASE("V8Predictor : Misc", "[V8Predictor]") {
  std::vector<double> sequence = {
      0.01800425609760259, 0.19267361208155598, 0.9892770985784053, 0.49553307275603264, 0.7362624704291061
  };
  NodeVersion v = NodeVersion{24, 2, 0};
  V8Predictor v8 = V8Predictor{v, sequence};

  SECTION("Validate sequence doesn't change") {
    // Just run a predict next to change internal state.
    v8.predictNext();
    REQUIRE(v8.getSequence() == sequence);
  }
  SECTION("Validate Node version doesn't change") {
    NodeVersion nv = v8.getNodeVersion();
    REQUIRE(nv.toString() == v.toString());
  }
}