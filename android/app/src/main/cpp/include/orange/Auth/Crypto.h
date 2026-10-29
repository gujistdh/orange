#pragma once

#include "orange/Common.h"

namespace Orange::Auth
{
    extern const char* const Endpoint;
    extern const char* const Secret;
    extern const char* const HexCharset;

    uint32_t Fnv1a(uint32_t Seed, const std::string& Data);
    std::string BuildNonce();
    std::string BuildSignature(const std::string& Action, const std::string& Card,
                               const std::string& Hwid, const std::string& Timestamp,
                               const std::string& Nonce);
    std::string BuildHwid();
    std::string NowTimestamp();
    std::string UrlEncode(const std::string& Value);
    std::string Rc4(const std::string& Key, const std::string& Data);
    bool Base64Decode(const std::string& Input, std::string& Out);
    bool DecodeBlob(const std::string& Blob, const std::string& Token, std::string& Out);
}
