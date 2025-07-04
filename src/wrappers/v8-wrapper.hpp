#pragma once

#include <napi.h>

#include "v8-predictor.hpp"

class V8Wrapper : public Napi::ObjectWrap<V8Wrapper> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::FunctionReference constructor;

  V8Wrapper(const Napi::CallbackInfo &info);

 private:
  Napi::Value predictNext(const Napi::CallbackInfo &info);
  Napi::Value getSequence(const Napi::CallbackInfo &info);
  Napi::Value getNodeVersion(const Napi::CallbackInfo &info);
  void setNodeVersion(const Napi::CallbackInfo &info, const Napi::Value &value);
  std::unique_ptr<V8Predictor> V8PredictorInstance;
};