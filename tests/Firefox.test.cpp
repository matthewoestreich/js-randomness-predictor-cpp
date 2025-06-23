#include <catch2/catch_test_macros.hpp>

#include "FirefoxPredictor.hpp"

TEST_CASE("FirefoxPredictor", "[FirefoxPredictor]") {
  std::vector<double> sequence{
      0.1321263101773572,
      0.03366887439746058,
      0.032596957696410134,
      0.9986575482138969,
      0.8479779907956815,
  };
  std::vector<double> expected{
      0.13963871472821332,
      0.25068024611907636,
      0.6656237481612675,
      0.7381091878692425,
      0.8709382509549467,
      0.49171337524788294,
      0.6991749430716799,
      0.9530887478758369,
      0.781511163650037,
      0.699311162730038,
  };

  SECTION("should correctly predict next random numbers") {
    FirefoxPredictor firefox{sequence};
    std::vector<double> predictions;

    for (size_t i = 0; i < expected.size(); ++i) {
      predictions.push_back(firefox.predictNext());
    }

    REQUIRE(expected == predictions);
  }
}