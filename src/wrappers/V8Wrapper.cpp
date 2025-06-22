#include <napi.h>
#include <regex>

#include "V8Predictor.hpp"
#include "V8Wrapper.hpp"

NodeVersion getNodeVersionFromEnv(const Napi::Env &env) {
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
          V8Wrapper::InstanceAccessor("sequence", &V8Wrapper::getSequence, nullptr),
          V8Wrapper::InstanceAccessor("nodeVersion", &V8Wrapper::getNodeVersion, nullptr),
      }
  );

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set(
      "v8",
      Napi::Function::New(
          env,
          [](const Napi::CallbackInfo &info) {
            std::vector<napi_value> args(info.Length());
            for (size_t i = 0; i < info.Length(); ++i) {
              args[i] = info[i];
            }
            return constructor.New(args);
          },
          "v8"
      )
  );

  return exports;
}

Napi::FunctionReference V8Wrapper::constructor;

V8Wrapper::V8Wrapper(const Napi::CallbackInfo &info)
  : Napi::ObjectWrap<V8Wrapper>(info) {
  Napi::Env env = info.Env();
  nodeVersion = getNodeVersionFromEnv(env);
  Napi::Value maybeSequence = info[0];

  if (info.Length() == 0 || maybeSequence.IsUndefined()) {
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

Napi::Value V8Wrapper::getSequence(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Array result = Napi::Array::New(env, sequence.size());
  for (size_t i = 0; i < sequence.size(); ++i) {
    result.Set(i, Napi::Number::New(env, sequence[i]));
  }
  return result;
}

Napi::Value V8Wrapper::getNodeVersion(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Object obj = Napi::Object::New(env);
  obj.Set("major", Napi::Number::New(env, nodeVersion.major));
  obj.Set("minor", Napi::Number::New(env, nodeVersion.minor));
  obj.Set("patch", Napi::Number::New(env, nodeVersion.patch));
  return obj;
}
