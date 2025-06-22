#include <napi.h>

#include "ChromePredictor.hpp"

class ChromeWrapper : public Napi::ObjectWrap<ChromeWrapper> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  ChromeWrapper(const Napi::CallbackInfo &info);
  std::vector<double> sequence;
  Napi::Value predictNext();

private:
  Napi::Value predictNext(const Napi::CallbackInfo &info);
  Napi::Value getSequence(const Napi::CallbackInfo &iinfo);
  std::unique_ptr<ChromePredictor> ChromePredictorInstance;
};