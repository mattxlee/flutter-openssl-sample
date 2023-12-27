#ifndef SHA256_OPENSSL_HPP
#define SHA256_OPENSSL_HPP

#include <openssl/evp.h>
#include <openssl/sha.h>

#include <cstdint>

#include <stdexcept>

#include "sha256.h"

namespace crypto
{

class SHA256
{
    EVP_MD_CTX* m_ctx;

    void ThrowOnError(int ret)
    {
        if (ret != 1) {
            throw std::runtime_error("SHA256 error");
        }
    }

public:
    SHA256()
        : m_ctx(EVP_MD_CTX_new())
    {
        ThrowOnError(EVP_DigestInit_ex(m_ctx, EVP_sha256(), nullptr));
    }

    SHA256(SHA256 const&) = delete;

    SHA256& operator=(SHA256 const&) = delete;

    SHA256(SHA256&& rhs)
        : m_ctx(rhs.m_ctx)
    {
        rhs.m_ctx = nullptr;
    }

    SHA256& operator=(SHA256&& rhs)
    {
        if (&rhs != this) {
            m_ctx = rhs.m_ctx;
            rhs.m_ctx = nullptr;
        }
        return *this;
    }

    ~SHA256()
    {
        EVP_MD_CTX_free(m_ctx);
    }

    void Write(void const* ptr, std::size_t size)
    {
        ThrowOnError(EVP_DigestUpdate(m_ctx, ptr, size));
    }

    SHA256Result Finalize()
    {
        SHA256Result result;
        std::uint32_t len = result.size();
        ThrowOnError(EVP_DigestFinal_ex(m_ctx, result.data(), &len));
        return result;
    }
};

} // namespace crypto

#endif