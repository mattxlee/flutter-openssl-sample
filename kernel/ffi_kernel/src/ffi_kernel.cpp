#include "ffi_kernel.h"

#include <cstdint>

#include <array>

#ifdef __APPLE__
#include "sha256_apple.hpp"
#else
#include "sha256_openssl.hpp"
#endif

// A very short-lived native function.
//
// For very short-lived functions, it is fine to call them on the main isolate.
// They will block the Dart execution while running the native function, so
// only do this for native functions which are guaranteed to be short-lived.
FFI_PLUGIN_EXPORT intptr_t sum(intptr_t a, intptr_t b) { return a + b; }

// A longer-lived native function, which occupies the thread calling it.
//
// Do not call these kind of native functions in the main isolate. They will
// block Dart execution. This will cause dropped frames in Flutter applications.
// Instead, call these native functions on a separate isolate.
FFI_PLUGIN_EXPORT intptr_t sum_long_running(intptr_t a, intptr_t b) {
  // Simulate work.
#if _WIN32
  Sleep(5000);
#else
  usleep(5000 * 1000);
#endif
  return a + b;
}

static crypto::SHA256Result g_result;

FFI_PLUGIN_EXPORT unsigned char const* hash_number(int number)
{
    crypto::SHA256 sha256;
    sha256.Write(&number, sizeof(number));
    g_result = sha256.Finalize();
    return g_result.data();
}