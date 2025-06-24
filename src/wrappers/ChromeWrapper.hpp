#pragma once

#include <napi.h>

#include "ChromePredictor.hpp"

class ChromeWrapper : public Napi::ObjectWrap<ChromeWrapper> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::FunctionReference constructor;

  ChromeWrapper(const Napi::CallbackInfo &info);

private:
  Napi::Value predictNext(const Napi::CallbackInfo &info);
  Napi::Value getSequence(const Napi::CallbackInfo &info);
  Napi::Value getNodeVersion(const Napi::CallbackInfo &info);
  std::unique_ptr<ChromePredictor> ChromePredictorInstance;
};