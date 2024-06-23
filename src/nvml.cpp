#include "napi.h"
#include "nvml.h"
#include <string>
#include <memory>

#define NVMLcheck(status_code)                                                   \
  if (static_cast<nvmlReturn_t>(status_code) != NVML_SUCCESS)                    \
  {                                                                              \
    Napi::TypeError::New(env, "NVML Failed with " + std::to_string(status_code)) \
        .ThrowAsJavaScriptException();                                           \
  }

Napi::Value InitializeNVML(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  NVMLcheck(nvmlInit_v2());
  return Napi::Number::New(env, 0);
}

Napi::Value DeviceGetCount(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  std::unique_ptr<uint32_t> count = std::make_unique<uint32_t>();
  NVMLcheck(nvmlDeviceGetCount_v2(count.get()));
  return Napi::Number::New(env, *count.get());
}
Napi::External<nvmlDevice_t> DeviceGetHandleByIndex(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
  }

  if (!info[0].IsNumber())
  {
    Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
  }
  uint32_t index = info[0].As<Napi::Number>().Int32Value();
  nvmlDevice_t *handle = new nvmlDevice_t;
  NVMLcheck(nvmlDeviceGetHandleByIndex_v2(index, handle));

  return Napi::External<nvmlDevice_t>::New(env, handle);
}

Napi::Value DeviceGetTotalEnergyConsumption(const Napi::CallbackInfo &info)
{

  Napi::Env env = info.Env();

  std::unique_ptr<unsigned long long> energy_value = std::make_unique<unsigned long long>();
  if (info.Length() < 1 || !info[0].IsExternal())
  {
    Napi::TypeError::New(env, "Wrong arguments")
        .ThrowAsJavaScriptException();
  }
  NVMLcheck(nvmlDeviceGetTotalEnergyConsumption(*info[0].As<Napi::External<nvmlDevice_t>>().Data(), energy_value.get()));
  return Napi::Number::New(env, *energy_value.get());
}
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "nvmlInit"), Napi::Function::New(env, InitializeNVML));
  exports.Set(Napi::String::New(env, "nvmlDeviceGetCount"), Napi::Function::New(env, DeviceGetCount));
  exports.Set(Napi::String::New(env, "nvmlDeviceGetHandleByIndex"), Napi::Function::New(env, DeviceGetHandleByIndex));
  exports.Set(Napi::String::New(env, "nvmlDeviceGetTotalEnergyConsumption"), Napi::Function::New(env, DeviceGetTotalEnergyConsumption));
  return exports;
}

NODE_API_MODULE(addon, Init)