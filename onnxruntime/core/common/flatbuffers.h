// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#اگر defined(__GNUC__)
#شامل "onnxruntime_config.h"
#pragma GCC diagnostic افزودن

#ifdef HAS_SHORTEN_64_TO_32
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#endif
#endif

#شامل "flatbuffers/flatbuffers.h"

#اگر defined(__GNUC__)
#pragma GCC diagnostic برداشت
#endif