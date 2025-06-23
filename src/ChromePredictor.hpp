#pragma once

#include <z3++.h>

class ChromePredictor {
public:
  ChromePredictor(const std::vector<double> &sequence);
  const std::vector<double> &getSequence() const;
  double predictNext();

private:
  std::vector<double> sequence;
  std::vector<double> internalSequence;
  uint64_t cState0;
  uint64_t cState1;
  z3::context context;
  z3::solver solver;
  z3::expr sState0;
  z3::expr sState1;

  void xorShift128PlusSymbolic();
  uint64_t xorShift128PlusConcrete();
  uint64_t recoverMantissa(double value);
  double toDouble(uint64_t value);
};