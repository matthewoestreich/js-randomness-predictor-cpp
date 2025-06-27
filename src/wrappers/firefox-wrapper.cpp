#include "firefox-wrapper.hpp"

#include <napi.h>

#include <cstddef>

/*
  Initialize our FirefoxWrapper.
*/
Napi::Object FirefoxWrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = FirefoxWrapper::DefineClass(
      env, "firefox",
      {
          FirefoxWrapper::InstanceMethod("predictNext",
                                         &FirefoxWrapper::predictNext),
          FirefoxWrapper::InstanceAccessor(
              "sequence", &FirefoxWrapper::getSequence, nullptr),
      });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  auto callCtor = [](const Napi::CallbackInfo &info) {
    std::vector<napi_value> args(info.Length());
    for (size_t i = 0; i < info.Length(); ++i) {
      args[i] = info[i];
    }
    return constructor.New(args);
  };

  exports.Set("firefox", Napi::Function::New(env, callCtor, "firefox"));
  return exports;
}

/*
  Reference to FirefoxWrapper constructor.
*/
Napi::FunctionReference FirefoxWrapper::constructor;

/*
  Create new FirefoxWrapper.
*/
FirefoxWrapper::FirefoxWrapper(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<FirefoxWrapper>(info) {
  Napi::Env env = info.Env();
  Napi::Value maybeSequence = info[0];

  if (!maybeSequence.IsArray()) {
    Napi::TypeError::New(env, "Expected number[]").ThrowAsJavaScriptException();
    return;
  }

  Napi::Array sequence = maybeSequence.As<Napi::Array>();
  std::vector<double> _sequence;

  for (size_t i = 0; i < sequence.Length(); ++i) {
    Napi::MaybeOrValue<Napi::Value> val = sequence.Get(i);
    if (val.IsUndefined() || val.IsNull() || !val.IsNumber()) {
      Napi::TypeError::New(env, "Expected number").ThrowAsJavaScriptException();
      return;
    }
    _sequence.push_back(val.As<Napi::Number>().DoubleValue());
  }

  FirefoxPredictorInstance =
      std::make_unique<FirefoxPredictor>(std::move(_sequence));
}

/*
  Predict next random number.
*/
Napi::Value FirefoxWrapper::predictNext(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), FirefoxPredictorInstance->predictNext());
}

/*
  Native method to get our sequence as expected type.
*/
Napi::Value FirefoxWrapper::getSequence(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  std::vector<double> instanceSequence =
      FirefoxPredictorInstance->getSequence();
  Napi::Array result = Napi::Array::New(env, instanceSequence.size());

  for (size_t i = 0; i < instanceSequence.size(); ++i) {
    result.Set(i, Napi::Number::New(env, instanceSequence[i]));
  }

  return result;
}