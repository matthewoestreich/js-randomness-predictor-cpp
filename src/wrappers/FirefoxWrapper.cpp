#include <napi.h>

#include "FirefoxPredictor.hpp"
#include "FirefoxWrapper.hpp"

Napi::Object FirefoxWrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = FirefoxWrapper::DefineClass(
      env,
      "firefox",
      {
          FirefoxWrapper::InstanceMethod("predictNext", &FirefoxWrapper::predictNext),
          FirefoxWrapper::InstanceAccessor("sequence", &FirefoxWrapper::getSequence, nullptr),
      }
  );

  Napi::FunctionReference *ctor = new Napi::FunctionReference();
  *ctor = Napi::Persistent(func);
  exports.Set("firefox", func);
  return exports;
}

FirefoxWrapper::FirefoxWrapper(const Napi::CallbackInfo &info)
  : Napi::ObjectWrap<FirefoxWrapper>(info) {
  Napi::Env env = info.Env();
  Napi::Value maybeSequence = info[0];

  if (!maybeSequence.IsArray()) {
    Napi::TypeError::New(env, "Expected number[]").ThrowAsJavaScriptException();
    return;
  }

  Napi::Array _sequence = maybeSequence.As<Napi::Array>();
  for (uint32_t i = 0; i < _sequence.Length(); ++i) {
    double val = _sequence.Get(i).As<Napi::Number>().DoubleValue();
    sequence.push_back(val);
  }
  FirefoxPredictorInstance = std::make_unique<FirefoxPredictor>(sequence);
}

Napi::Value FirefoxWrapper::predictNext(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), FirefoxPredictorInstance->predictNext());
}

Napi::Value FirefoxWrapper::getSequence(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Array result = Napi::Array::New(env, sequence.size());
  for (size_t i = 0; i < sequence.size(); ++i) {
    result.Set(i, Napi::Number::New(env, sequence[i]));
  }
  return result;
}