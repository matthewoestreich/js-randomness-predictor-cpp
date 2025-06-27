#pragma once

#include <z3++.h>

#include <string>

struct NodeVersion {
  int major;
  int minor;
  int patch;

  std::string toString() const {
    return "v" + std::to_string(major) + "." + std::to_string(minor) + "." +
           std::to_string(patch);
  }
};

class V8Predictor {
 public:
  V8Predictor(const NodeVersion &version, const std::vector<double> &sequence);
  const std::vector<double> &getSequence() const;
  const NodeVersion &getNodeVersion() const;
  // Only use this if you know what you're doing!
  void setNodeVersion(const NodeVersion &v);
  double predictNext();

 private:
  std::vector<double> sequence;
  NodeVersion nodeVersion;
  std::vector<double> internalSequence;
  bool isInitialized = false;
  uint64_t cState0;
  uint64_t cState1;
  z3::context context;
  z3::solver solver;
  z3::expr sState0;
  z3::expr sState1;

  void xorShift128PlusSymbolic();
  bool initialize();
  void recoverMantissaAndAddToSolver(double value);
  uint64_t xorShift128PlusConcrete();
  double toDouble(uint64_t value);
};