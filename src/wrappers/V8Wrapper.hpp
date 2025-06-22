#include <napi.h>

#include "V8Predictor.hpp"

class V8Wrapper : public Napi::ObjectWrap<V8Wrapper> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  V8Wrapper(const Napi::CallbackInfo &info);
  std::vector<double> sequence;
  Napi::Value predictNext();

private:
  NodeVersion nodeVersion;
  Napi::Value predictNext(const Napi::CallbackInfo &info);
  std::unique_ptr<V8Predictor> V8PredictorInstance;
};