#include "v8-wrapper.hpp"

#include <napi.h>

#include <cstddef>
#include <regex>

/*
  Gets current Node.js version.
*/
NodeVersion getCurrentNodeVersion(const Napi::Env &env) {
  Napi::Object process = env.Global().Get("process").As<Napi::Object>();
  std::string version = process.Get("version").As<Napi::String>().Utf8Value();
  std::regex versionRegex(R"(^v(\d+)\.(\d+)\.(\d+))");
  std::smatch match;
  NodeVersion nodeVersion{};
  if (std::regex_search(version, match, versionRegex) && match.size() == 4) {
    nodeVersion.major = std::stoi(match[1].str());
    nodeVersion.minor = std::stoi(match[2].str());
    nodeVersion.patch = std::stoi(match[3].str());
  }
  return nodeVersion;
}

/*
  Calls `Math.random()` in native code.
*/
double MathRandom(const Napi::Env &env) {
  Napi::Object global = env.Global();
  Napi::Object math = global.Get("Math").As<Napi::Object>();
  Napi::Function randomFn = math.Get("random").As<Napi::Function>();
  return randomFn.Call(math, {}).As<Napi::Number>().DoubleValue();
}

/*
  Reference to V8Wrapper constructor.
*/
Napi::FunctionReference V8Wrapper::constructor;

/*
  Initialize our V8Wrapper.
*/
Napi::Object V8Wrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = V8Wrapper::DefineClass(
      env, "v8",
      {
          V8Wrapper::InstanceMethod("predictNext", &V8Wrapper::predictNext),
          V8Wrapper::InstanceAccessor("sequence", &V8Wrapper::getSequence,
                                      nullptr),
          V8Wrapper::InstanceAccessor<&V8Wrapper::getNodeVersion,
                                      &V8Wrapper::setNodeVersion>(
              "nodeVersion"),
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

  exports.Set("v8", Napi::Function::New(env, callCtor, "v8"));
  return exports;
}

/*
  Create V8Wrapper.
*/
V8Wrapper::V8Wrapper(const Napi::CallbackInfo &info)
    : Napi::ObjectWrap<V8Wrapper>(info) {
  Napi::Env env = info.Env();
  Napi::Value maybeSequence = info[0];

  // If we were given more than one param.
  if (info.Length() > 1) {
    Napi::Error::New(env, "Expected single parameter of type number[]")
        .ThrowAsJavaScriptException();
    return;
  }
  // If we were given a param but it isn't an array.
  if (info.Length() > 0 && !maybeSequence.IsUndefined() &&
      !maybeSequence.IsArray()) {
    Napi::TypeError::New(env, "Expected number[]").ThrowAsJavaScriptException();
    return;
  }

  std::vector<double> _sequence;

  // If no param, fall back to Math.random.
  if (info.Length() == 0) {
    for (size_t i = 0; i < 4; ++i) {
      _sequence.push_back(MathRandom(env));
    }
  } else {
    // Otherwise construct from provided array.
    Napi::Array maybeSequenceArray = maybeSequence.As<Napi::Array>();

    for (size_t i = 0; i < maybeSequenceArray.Length(); ++i) {
      Napi::Value val = maybeSequenceArray.Get(i);
      if (!val.IsNumber()) {
        Napi::TypeError::New(env, "Expected number")
            .ThrowAsJavaScriptException();
        return;
      }
      _sequence.push_back(val.As<Napi::Number>().DoubleValue());
    }
  }

  V8PredictorInstance = std::make_unique<V8Predictor>(
      getCurrentNodeVersion(env), std::move(_sequence));
}

/*
   Predict next random number.
*/
Napi::Value V8Wrapper::predictNext(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), V8PredictorInstance->predictNext());
}

/*
  Get sequence as expected type.
*/
Napi::Value V8Wrapper::getSequence(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  std::vector<double> instanceSequence = V8PredictorInstance->getSequence();
  Napi::Array result = Napi::Array::New(env, instanceSequence.size());

  for (size_t i = 0; i < instanceSequence.size(); ++i) {
    result.Set(i, Napi::Number::New(env, instanceSequence[i]));
  }

  return result;
}

/*
  Gets Node.js version from our class as expected type.
*/
Napi::Value V8Wrapper::getNodeVersion(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Object obj = Napi::Object::New(env);
  NodeVersion nv = V8PredictorInstance->getNodeVersion();

  obj.Set("major", Napi::Number::New(env, nv.major));
  obj.Set("minor", Napi::Number::New(env, nv.minor));
  obj.Set("patch", Napi::Number::New(env, nv.patch));

  return obj;
}

/*
  Set Node.js version. This allows you to change the predictor so it is able to
  predict from sequences generated in different versions of Node.js.
*/
void V8Wrapper::setNodeVersion(const Napi::CallbackInfo &info,
                               const Napi::Value &value) {
  Napi::Env env = info.Env();

  if (!value.IsObject()) {
    Napi::TypeError::New(env, "Expected an object with { major, minor, patch }")
        .ThrowAsJavaScriptException();
    return;
  }

  Napi::Object input = value.As<Napi::Object>();

  NodeVersion version;
  version.major = input.Get("major").As<Napi::Number>().Int32Value();
  version.minor = input.Get("minor").As<Napi::Number>().Int32Value();
  version.patch = input.Get("patch").As<Napi::Number>().Int32Value();

  V8PredictorInstance->setNodeVersion(version);
}