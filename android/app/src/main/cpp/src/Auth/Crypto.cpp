#include "orange/Auth/Crypto.h"

#include <ctime>
#include <unistd.h>
#include <sys/stat.h>

namespace Orange::Auth
{
    const char* const Endpoint = "http://47.242.221.255/api.php";
    const char* const Secret = "1f4e8d804a24b71972933476ce00967c6a42aa41fd169d91";
    const char* const HexCharset = "0123456789abcdef";

    namespace
    {
        uint64_t NonceState = 0;
    }

    uint32_t Fnv1a(uint32_t Seed, const std::string& Data)
    {
        for (std::size_t Index = 0; Index < Data.size(); ++Index)
        {
            Seed = 16777619u * (Seed ^ static_cast<uint32_t>(static_cast<uint8_t>(Data[Index])));
        }
        return Seed;
    }

    std::string BuildNonce()
    {
        if (NonceState == 0)
        {
            NonceState = static_cast<uint64_t>(std::time(nullptr)) ^ reinterpret_cast<uintptr_t>(&NonceState);
        }
        char Buffer[33];
        for (int Outer = 0; Outer <= 31; Outer += 8)
        {
            NonceState = 0x5851F42D4C957F2Dull * NonceState + 0x14057B7EF767814Full;
            for (int Inner = 0; Inner <= 7; ++Inner)
            {
                Buffer[Outer + Inner] = HexCharset[(NonceState >> (4 * Inner)) & 0xF];
            }
        }
        Buffer[32] = 0;
        return std::string(Buffer);
    }

    std::string BuildSignature(const std::string& Action, const std::string& Card,
                               const std::string& Hwid, const std::string& Timestamp,
                               const std::string& Nonce)
    {
        std::string Input;
        Input.reserve(256);
        Input += Secret;
        Input += "|";
        Input += Action;
        Input += "|";
        Input += Card;
        Input += "|";
        Input += Hwid;
        Input += "|";
        Input += Timestamp;
        Input += "|";
        Input += Nonce;
        uint32_t First = Fnv1a(2166136261u, Input);
        uint32_t Second = Fnv1a(3735928559u, Input);
        char Buffer[32];
        std::snprintf(Buffer, sizeof(Buffer), "%08x%08x", First, Second);
        return std::string(Buffer);
    }

    std::string BuildHwid()
    {
        std::string Result;
        FILE* Handle = std::fopen("/proc/sys/kernel/random/boot_id", "r");
        if (Handle)
        {
            char Buffer[64] = { 0 };
            if (std::fgets(Buffer, sizeof(Buffer) - 1, Handle))
            {
                Result = Buffer;
                while (!Result.empty() && (Result.back() == '\n' || Result.back() == '\r'))
                {
                    Result.pop_back();
                }
            }
            std::fclose(Handle);
        }
        if (Result.empty())
        {
            char Buffer[32];
            std::snprintf(Buffer, sizeof(Buffer), "%016llx", static_cast<unsigned long long>(std::time(nullptr)));
            Result = Buffer;
        }
        return Result;
    }

    std::string NowTimestamp()
    {
        char Buffer[32];
        std::snprintf(Buffer, sizeof(Buffer), "%lld", static_cast<long long>(std::time(nullptr)));
        return std::string(Buffer);
    }

    std::string UrlEncode(const std::string& Value)
    {
        std::string Out;
        Out.reserve(Value.size() * 3);
        for (char Ch : Value)
        {
            unsigned char Byte = static_cast<unsigned char>(Ch);
            if ((Byte >= 'A' && Byte <= 'Z') || (Byte >= 'a' && Byte <= 'z') ||
                (Byte >= '0' && Byte <= '9') || Byte == '-' || Byte == '_' || Byte == '.' || Byte == '~')
            {
                Out.push_back(static_cast<char>(Byte));
            }
            else
            {
                char Buffer[4];
                std::snprintf(Buffer, sizeof(Buffer), "%%%02X", Byte);
                Out += Buffer;
            }
        }
        return Out;
    }

    std::string Rc4(const std::string& Key, const std::string& Data)
    {
        uint8_t State[256];
        for (int Index = 0; Index < 256; ++Index)
        {
            State[Index] = static_cast<uint8_t>(Index);
        }
        uint8_t KeyByte = 0;
        for (int Index = 0; Index < 256; ++Index)
        {
            KeyByte = static_cast<uint8_t>(KeyByte + State[Index] + static_cast<uint8_t>(Key[Index % Key.size()]));
            std::swap(State[Index], State[KeyByte]);
        }
        std::string Out = Data;
        uint8_t First = 0;
        uint8_t Second = 0;
        for (std::size_t Index = 0; Index < Out.size(); ++Index)
        {
            First = static_cast<uint8_t>(First + 1);
            Second = static_cast<uint8_t>(Second + State[First]);
            std::swap(State[First], State[Second]);
            Out[Index] = static_cast<char>(Out[Index] ^ State[static_cast<uint8_t>(State[First] + State[Second])]);
        }
        return Out;
    }

    bool Base64Decode(const std::string& Input, std::string& Out)
    {
        Out.clear();
        unsigned int Buffer = 0;
        int Bits = 0;
        for (char Ch : Input)
        {
            if (Ch == '=' || Ch == '\r' || Ch == '\n' || Ch == ' ')
            {
                continue;
            }
            int Value = -1;
            if (Ch >= 'A' && Ch <= 'Z') { Value = Ch - 'A'; }
            else if (Ch >= 'a' && Ch <= 'z') { Value = Ch - 'a' + 26; }
            else if (Ch >= '0' && Ch <= '9') { Value = Ch - '0' + 52; }
            else if (Ch == '+') { Value = 62; }
            else if (Ch == '/') { Value = 63; }
            if (Value < 0)
            {
                return false;
            }
            Buffer = (Buffer << 6) | static_cast<unsigned int>(Value);
            Bits += 6;
            if (Bits >= 8)
            {
                Bits -= 8;
                Out.push_back(static_cast<char>((Buffer >> Bits) & 0xFFu));
            }
        }
        return !Out.empty();
    }

    bool DecodeBlob(const std::string& Blob, const std::string& Token, std::string& Out)
    {
        Out.clear();
        if (Blob.empty() || Token.size() <= 15)
        {
            return false;
        }
        std::string Raw;
        if (!Base64Decode(Blob, Raw))
        {
            return false;
        }
        std::string Plain = Rc4(Token.substr(0, 16), Raw);
        if (Plain.size() < 8)
        {
            return false;
        }
        if (Plain[0] != 'K' || Plain[1] != 'M' || Plain[2] != '0' || Plain[3] != '1')
        {
            return false;
        }
        unsigned int Length = (static_cast<unsigned char>(Plain[4]) << 24) |
                              (static_cast<unsigned char>(Plain[5]) << 16) |
                              (static_cast<unsigned char>(Plain[6]) << 8) |
                              static_cast<unsigned char>(Plain[7]);
        if (Plain.size() < static_cast<std::size_t>(Length) + 8)
        {
            return false;
        }
        Out = Plain.substr(8, Length);
        return true;
    }
}
