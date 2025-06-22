#include <catch2/catch_test_macros.hpp>

#include "../src/ChromePredictor.hpp"

TEST_CASE("ChromePredictor", "[ChromePredictor]") {
  std::vector<double> sequence{
      0.32096095967729477,
      0.3940071672626849,
      0.3363374923027722,
      0.7518761096243554,
      0.44201420586496387,
  };
  std::vector<double> expected{
      0.8199006769436774,   0.6250240806313154,  0.9101975676132608, 0.5889203398264599,  0.5571161440436232,
      0.9619184649129092,   0.8385620929536599,  0.3822042053588621, 0.5040552869863579,  0.12014019399083042,
      0.44332968383610927,  0.37830079319230936, 0.542449069899975,  0.0659240460476268,  0.9589494984837686,
      0.007621633090565627, 0.14119301022498787, 0.9964718645470699, 0.14527130036353442, 0.6260597083849548,
      0.86354903522581,     0.7245123107811886,  0.6565323828155891, 0.3636039851663503,  0.5799453712253447,
  };

  SECTION("should correctly predict next random numbers") {
    ChromePredictor chrome(sequence);
    std::vector<double> predictions;

    for (size_t i = 0; i < expected.size(); ++i) {
      predictions.push_back(chrome.predictNext());
    }

    REQUIRE(expected == predictions);
  }
}