#ifdef _WIN32

#include <Windows.h>
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

#include "driver_log.h"
#include "file_path.h"

static std::string GetLastErrorAsString() {
  const DWORD errorMessageId = ::GetLastError();
  if (errorMessageId == 0) return std::string();

  LPSTR messageBuffer = nullptr;
  const size_t size = FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      errorMessageId,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      reinterpret_cast<LPSTR>(&messageBuffer),
      0,
      nullptr);

  std::string message(messageBuffer, size);

  LocalFree(messageBuffer);

  return message;
}

std::string GetDriverPath() {
  HMODULE hm = nullptr;
  if (GetModuleHandleExA(
          GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, reinterpret_cast<LPCSTR>(&__ImageBase), &hm) == 0) {
    DriverLog("GetModuleHandle failed, error: %s", GetLastErrorAsString().c_str());
    return "";
  }

  char path[1024];
  if (GetModuleFileNameA(hm, path, sizeof path) == 0) {
    DriverLog("GetModuleFileName failed, error: %s", GetLastErrorAsString().c_str());
    return "";
  }

  std::string path_string = path;
  const std::string unwanted = R"(\bin\win64\)";
  return path_string.substr(0, path_string.find_last_of("\\/")).erase(path_string.find(unwanted), unwanted.length());
}

#endif