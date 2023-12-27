#ifndef FFI_KERNEL_SHA256_H
#define FFI_KERNEL_SHA256_H

#include <array>

namespace crypto
{

const int SHA256ResultLength = 256 / 8;
using SHA256Result = std::array<std::uint8_t, SHA256ResultLength>;

}

#endif