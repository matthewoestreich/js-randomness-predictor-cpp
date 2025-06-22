#pragma once

#include "napi.h"
#include <string>
#include <regex>

struct NodeJsVersionNapi {
  std::string version;  // full version string, e.g. "v24.2.0"
  int major = 0;
  int minor = 0;
  int patch = 0;

  NodeJsVersionNapi() = default;

  explicit NodeJsVersionNapi(const Napi::Env &env) {
    Napi::Object process = env.Global().Get("process").As<Napi::Object>();
    version = process.Get("version").As<Napi::String>().Utf8Value();

    // version string format: v<major>.<minor>.<patch>
    // example: "v24.2.0"
    std::regex versionRegex(R"(^v(\d+)\.(\d+)\.(\d+))");
    std::smatch match;

    if (std::regex_search(version, match, versionRegex) && match.size() == 4) {
      major = std::stoi(match[1].str());
      minor = std::stoi(match[2].str());
      patch = std::stoi(match[3].str());
    }
  }
};
