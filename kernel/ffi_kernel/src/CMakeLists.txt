# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(ffi_kernel_library VERSION 0.0.1 LANGUAGES C CXX)

if (ANDROID_ABI MATCHES "^x86_64")
    set(ANDROID_TARGET_ABI "android-x86_64")
elseif(ANDROID_ABI MATCHES "^arm64")
    set(ANDROID_TARGET_ABI "android-arm64")
endif()

set(OPENSSL_COMPILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/extprojs/compile/${ANDROID_TARGET_ABI}")
set(OPENSSL_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/extprojs/install/${ANDROID_TARGET_ABI}")
file(MAKE_DIRECTORY ${OPENSSL_COMPILE_DIR})
file(MAKE_DIRECTORY ${OPENSSL_INSTALL_DIR})

set(OPENSSL_CRYPTO_LINKING_LIB ${OPENSSL_INSTALL_DIR}/lib/libcrypto.a)
set(OPENSSL_SSL_LINKING_LIB ${OPENSSL_INSTALL_DIR}/lib/libssl.a)

set(OPENSSL_INCLUDE_DIR "${OPENSSL_INSTALL_DIR}/include")

include(ExternalProject)
ExternalProject_Add(openssl
    URL https://www.openssl.org/source/openssl-3.2.0.tar.gz
    PREFIX ${OPENSSL_COMPILE_DIR}
    CONFIGURE_COMMAND ./Configure no-apps --prefix=${OPENSSL_INSTALL_DIR} ${ANDROID_TARGET_ABI} -static -fPIC
    BUILD_IN_SOURCE TRUE
    BUILD_BYPRODUCTS ${OPENSSL_CRYPTO_LINKING_LIB}
    BUILD_BYPRODUCTS ${OPENSSL_SSL_LINKING_LIB}
)

add_library(OpenSSL_Crypto SHARED IMPORTED)
add_library(OpenSSL_SSL SHARED IMPORTED)

set_target_properties(OpenSSL_Crypto PROPERTIES IMPORTED_LOCATION ${OPENSSL_CRYPTO_LINKING_LIB})
set_target_properties(OpenSSL_SSL PROPERTIES IMPORTED_LOCATION ${OPENSSL_SSL_LINKING_LIB})

add_library(ffi_kernel SHARED
  "ffi_kernel.cpp"
)
set_target_properties(ffi_kernel PROPERTIES
  PUBLIC_HEADER ffi_kernel.h
  OUTPUT_NAME "ffi_kernel"
)
target_include_directories(ffi_kernel PRIVATE ${OPENSSL_INCLUDE_DIR})
target_compile_definitions(ffi_kernel PUBLIC DART_SHARED_LIB)
target_compile_features(ffi_kernel PRIVATE cxx_std_17)
target_link_libraries(ffi_kernel PRIVATE OpenSSL_Crypto OpenSSL_SSL)

add_dependencies(OpenSSL_Crypto openssl)
add_dependencies(OpenSSL_SSL openssl)
