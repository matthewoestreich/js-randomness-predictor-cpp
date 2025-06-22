#include <napi.h>
#include <regex>

#include "V8Predictor.hpp"
#include "V8Wrapper.hpp"

NodeVersion getNodeVersion(const Napi::Env &env) {
  // For finding the current node version
  Napi::Object process = env.Global().Get("process").As<Napi::Object>();
  std::string version = process.Get("version").As<Napi::String>().Utf8Value();
  std::regex versionRegex(R"(^v(\d+)\.(\d+)\.(\d+))");
  std::smatch match;
  NodeVersion nodeVersion;
  if (std::regex_search(version, match, versionRegex) && match.size() == 4) {
    nodeVersion.major = std::stoi(match[1].str());
    nodeVersion.minor = std::stoi(match[2].str());
    nodeVersion.patch = std::stoi(match[3].str());
  }
  return nodeVersion;
}

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
  : Napi::ObjectWrap<V8Wrapper>(info) {
  Napi::Env env = info.Env();
  nodeVersion = getNodeVersion(env);
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
    V8PredictorInstance = std::make_unique<V8Predictor>(nodeVersion, sequence);
    return;
  }

  if (maybeSequence.IsArray()) {
    Napi::Array _sequence = maybeSequence.As<Napi::Array>();
    for (uint32_t i = 0; i < _sequence.Length(); ++i) {
      double val = _sequence.Get(i).As<Napi::Number>().DoubleValue();
      sequence.push_back(val);
    }
    V8PredictorInstance = std::make_unique<V8Predictor>(nodeVersion, sequence);
    return;
  }

  Napi::TypeError::New(env, "Expected number[]").ThrowAsJavaScriptException();
}

Napi::Value V8Wrapper::predictNext(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), V8PredictorInstance->predictNext());
}