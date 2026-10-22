// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
#यदि defined(__GNUC__)
#अन्तर्भाव "onnxruntime_config.h"
#pragma GCC diagnostic थप

#ifdef HAS_SHORTEN_64_TO_32
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#endif
#endif

#अन्तर्भाव "flatbuffers/flatbuffers.h"

#यदि defined(__GNUC__)
#pragma GCC diagnostic झिक
#endif