// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once
#यदि defined(__GNUC__)
#अन्तर्भाव "onnxruntime_config.h"
#निर्देशः GCC diagnostic प्रक्षिप

#यदि_परिभाषितम् HAS_SHORTEN_64_गन्तव्यम्_32
#निर्देशः GCC diagnostic ignored "-Wshorten-64-to-32"
#यदि_अन्तः
#यदि_अन्तः

#अन्तर्भाव "flatbuffers/flatbuffers.h"

#यदि defined(__GNUC__)
#निर्देशः GCC diagnostic निष्कासय
#यदि_अन्तः