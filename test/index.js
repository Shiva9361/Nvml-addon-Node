const nvml = require("../build/Release/nvml");
console.log(nvml.nvmlInit());
console.log(nvml.nvmlDeviceGetCount());
const device = nvml.nvmlDeviceGetHandleByIndex(0);
console.log(device);
console.log(nvml.nvmlDeviceGetTotalEnergyConsumption(device));
