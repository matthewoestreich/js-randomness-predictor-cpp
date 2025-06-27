#include <napi.h>

#include "chrome-wrapper.hpp"
#include "firefox-wrapper.hpp"
#include "v8-wrapper.hpp"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  V8Wrapper::Init(env, exports);
  ChromeWrapper::Init(env, exports);
  FirefoxWrapper::Init(env, exports);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll)