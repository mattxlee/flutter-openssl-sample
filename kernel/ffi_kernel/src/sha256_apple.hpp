#ifndef SHA256_APPLE_HPP
#define SHA256_APPLE_HPP

#include <CommonCrypto/CommonDigest.h>

#include <cstdint>

#include <array>

#include "sha256.h"

namespace crypto {

class SHA256
{
public:
    SHA256()
    {
        CC_SHA256_Init(&m_ctx);
    }

    void Write(void const* ptr, int size)
    {
        CC_SHA256_Update(&m_ctx, ptr, size);
    }

    SHA256Result Finalize()
    {
        SHA256Result result;
        CC_SHA256_Final(result.data(), &m_ctx);
        return result;
    }

private:
    CC_SHA256_CTX m_ctx;
};

} // namespace crypto

#endif