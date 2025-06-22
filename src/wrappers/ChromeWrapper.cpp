#include <napi.h>

#include "ChromePredictor.hpp"
#include "ChromeWrapper.hpp"

Napi::Object ChromeWrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = ChromeWrapper::DefineClass(
      env,
      "chrome",
      {
          ChromeWrapper::InstanceMethod("predictNext", &ChromeWrapper::predictNext),
          ChromeWrapper::InstanceAccessor("sequence", &ChromeWrapper::getSequence, nullptr),
      }
  );

  Napi::FunctionReference *ctor = new Napi::FunctionReference();
  *ctor = Napi::Persistent(func);
  exports.Set("chrome", func);
  return exports;
}

ChromeWrapper::ChromeWrapper(const Napi::CallbackInfo &info)
  : Napi::ObjectWrap<ChromeWrapper>(info) {
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
  ChromePredictorInstance = std::make_unique<ChromePredictor>(sequence);
}

Napi::Value ChromeWrapper::predictNext(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), ChromePredictorInstance->predictNext());
}

Napi::Value ChromeWrapper::getSequence(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Array result = Napi::Array::New(env, sequence.size());
  for (size_t i = 0; i < sequence.size(); ++i) {
    result.Set(i, Napi::Number::New(env, sequence[i]));
  }
  return result;
}