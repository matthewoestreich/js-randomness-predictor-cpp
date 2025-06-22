#include "V8Wrapper.hpp"
#include "NodeJsVersionNapi.hpp"
#include "V8Predictor.hpp"
#include "napi.h"

Napi::Object V8Wrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = V8Wrapper::DefineClass(
      env,
      "v8",
      {
          V8Wrapper::InstanceMethod("predictNext", &V8Wrapper::predictNext),
      }
  );

  Napi::FunctionReference *ctor = new Napi::FunctionReference();
  *ctor = Napi::Persistent(func);
  exports.Set("v8", func);
  return exports;
}

V8Wrapper::V8Wrapper(const Napi::CallbackInfo &info)
  : Napi::ObjectWrap<V8Wrapper>(info),
    nodeVersion(info.Env()) {
  Napi::Env env = info.Env();
  Napi::Value maybeSequence = info[0];

  if (info.Length() == 0 || maybeSequence.IsUndefined() || !maybeSequence.IsArray()) {
    // Call Math.random() 4 times
    Napi::Object global = env.Global();
    Napi::Object math = global.Get("Math").As<Napi::Object>();
    Napi::Function randomFn = math.Get("random").As<Napi::Function>();
    for (int i = 0; i < 4; ++i) {
      double value = randomFn.Call(math, {}).As<Napi::Number>().DoubleValue();
      sequence.push_back(value);
    }
    V8PredictorInstance = std::make_unique<V8Predictor>(sequence);
    V8PredictorInstance->setNodeVersion(nodeVersion.major, nodeVersion.minor, nodeVersion.patch);
    return;
  }

  if (maybeSequence.IsArray()) {
    Napi::Array _sequence = maybeSequence.As<Napi::Array>();
    for (uint32_t i = 0; i < _sequence.Length(); ++i) {
      double val = _sequence.Get(i).As<Napi::Number>().DoubleValue();
      sequence.push_back(val);
    }
    V8PredictorInstance = std::make_unique<V8Predictor>(sequence);
    V8PredictorInstance->setNodeVersion(nodeVersion.major, nodeVersion.minor, nodeVersion.patch);
    return;
  }

  Napi::TypeError::New(env, "Expected number[]").ThrowAsJavaScriptException();
}

Napi::Value V8Wrapper::predictNext(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), V8PredictorInstance->predictNext());
}