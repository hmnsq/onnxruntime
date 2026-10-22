// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once
#اگر defined(__GNUC__)
#شامل "onnxruntime_config.h"
#ہدایت GCC diagnostic ڈالو

#اگر_تعریف_شدہ HAS_SHORTEN_64_مقصد_32
#ہدایت GCC diagnostic ignored "-Wshorten-64-to-32"
#اگر_ختم
#اگر_ختم

#شامل "flatbuffers/flatbuffers.h"

#اگر defined(__GNUC__)
#ہدایت GCC diagnostic نکالو
#اگر_ختم