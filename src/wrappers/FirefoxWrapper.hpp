#include <napi.h>

#include "FirefoxPredictor.hpp"

class FirefoxWrapper : public Napi::ObjectWrap<FirefoxWrapper> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::FunctionReference constructor;

  FirefoxWrapper(const Napi::CallbackInfo &info);

private:
  Napi::Value predictNext(const Napi::CallbackInfo &info);
  Napi::Value getSequence(const Napi::CallbackInfo &info);
  Napi::Value getNodeVersion(const Napi::CallbackInfo &info);
  std::unique_ptr<FirefoxPredictor> FirefoxPredictorInstance;
};