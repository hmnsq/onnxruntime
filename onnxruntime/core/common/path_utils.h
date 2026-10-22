// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <filesystem>
#అన్తర్భావ <string>
#అన్తర్భావ <string_view>

#అన్తర్భావ "core/common/common.h"
#అన్తర్భావ "core/common/path_string.h"

namespace onnxruntime {

namespace మార్గం_utils {

/** Return a PathString with concatenated args.
 *  TODO: add support for arguments of type std::wstring. Currently it is not supported as the underneath
 *  MakeString doesn't support this type.
 */
మూస <typename... వాదనలు_2>
మార్గం_String Make_మార్గం_String(స్థిరరాశి వాదనలు_2&... వాదనలు) {
  స్థిరరాశి std::string str = onnxruntime::MakeString(వాదనలు...);
  తిరిగి గమ్యం_మార్గం_String(str);
}

/** Return a directory path unchanged, or the parent path if the input is a file path. */
అంతఃవరుస std::filesystem::మార్గం Get_Dir_Or_మాతృ_మార్గం(స్థిరరాశి std::filesystem::మార్గం& మార్గం) {
  ఒకవేళ (మార్గం.ఖాళీలు() || std::filesystem::is_directory(మార్గం)) {
    తిరిగి మార్గం;
  }

  తిరిగి మార్గం.మాతృ_మార్గం();
}

/**
 * Return file_or_directory_path if it names a file. Otherwise, generate a filename from source_file_path's stem and
 * suffix, optionally under file_or_directory_path when it names a directory.
 */
అంతఃవరుస std::string Get_మార్గం_With_Stem_Suffix(స్థిరరాశి std::string& file_or_directory_మార్గం,
                                         స్థిరరాశి std::string& ఆధారం_file_మార్గం,
                                         std::string_view suffix) {
  ఒకవేళ (!file_or_directory_మార్గం.ఖాళీలు() && !std::filesystem::is_directory(file_or_directory_మార్గం)) {
    తిరిగి file_or_directory_మార్గం;
  }

  స్థిరరాశి std::filesystem::మార్గం ఆధారం_మార్గం = ఆధారం_file_మార్గం;
  స్థిరరాశి std::string default_filename = ఆధారం_మార్గం.stem().string() + std::string{suffix};
  ఒకవేళ (std::filesystem::is_directory(file_or_directory_మార్గం)) {
    std::filesystem::మార్గం directory = file_or_directory_మార్గం;
    తిరిగి directory.append(default_filename).string();
  }

  తిరిగి default_filename;
}

అంతఃవరుస bool Is_Absolute_మార్గం(స్థిరరాశి std::string& మార్గం_string) {
#నిర్వచితమైతే _WIN32
  స్థిరరాశి స్వయంచాలక మార్గం = std::filesystem::మార్గం{గమ్యం_మార్గం_String(మార్గం_string)};
  తిరిగి మార్గం.is_absolute();
#లేకపోతే
  తిరిగి !మార్గం_string.ఖాళీలు() && మార్గం_string.front() == '/';
#అయితే_ముగింపు
}

అంతఃవరుస bool Is_Relative_మార్గం_గమ్యం_మాతృ_మార్గం(స్థిరరాశి std::string& మార్గం_string) {
#నిర్వచితమైతే _WIN32
  స్థిరరాశి స్వయంచాలక మార్గం = std::filesystem::మార్గం{గమ్యం_మార్గం_String(మార్గం_string)};
  స్థిరరాశి స్వయంచాలక relative_మార్గం = మార్గం.lexically_సాధారణ().make_preferred().wstring();
  తిరిగి relative_మార్గం.కనుగొను(L"..") != std::wstring::npos;
#లేకపోతే
  తిరిగి !మార్గం_string.ఖాళీలు() && మార్గం_string.కనుగొను("..") != std::string::npos;
#అయితే_ముగింపు
}

}  // namespace path_utils
}  // namespace onnxruntime
