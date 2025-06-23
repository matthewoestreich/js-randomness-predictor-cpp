#include <cstddef>
#include <napi.h>

#include "ChromePredictor.hpp"
#include "ChromeWrapper.hpp"

/*
  Initialize our ChromeWrapper.
*/
Napi::Object ChromeWrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = ChromeWrapper::DefineClass(
      env,
      "chrome",
      {
          ChromeWrapper::InstanceMethod("predictNext", &ChromeWrapper::predictNext),
          ChromeWrapper::InstanceAccessor("sequence", &ChromeWrapper::getSequence, nullptr),
      }
  );

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set(
      "chrome",
      Napi::Function::New(
          env,
          [](const Napi::CallbackInfo &info) {
            std::vector<napi_value> args(info.Length());
            for (size_t i = 0; i < info.Length(); ++i) {
              args[i] = info[i];
            }
            return constructor.New(args);
          },
          "chrome"
      )
  );

  return exports;
}

/*
  Reference to ChromeWrapper constructor.
*/
Napi::FunctionReference ChromeWrapper::constructor;

/*
  Create new ChromeWrapper.
*/
ChromeWrapper::ChromeWrapper(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<ChromeWrapper>(info) {
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

  ChromePredictorInstance = std::make_unique<ChromePredictor>(std::move(_sequence));
}

/*
  Predict next random value.
*/
Napi::Value ChromeWrapper::predictNext(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), ChromePredictorInstance->predictNext());
}

/*
  Get sequence as expected type.
*/
Napi::Value ChromeWrapper::getSequence(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  std::vector<double> instanceSequence = ChromePredictorInstance->getSequence();
  Napi::Array result = Napi::Array::New(env, instanceSequence.size());

  for (size_t i = 0; i < instanceSequence.size(); ++i) {
    result.Set(i, Napi::Number::New(env, instanceSequence[i]));
  }

  return result;
}