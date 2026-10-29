#pragma once

#include "orange/Common.h"

namespace Orange::Gui
{
    class JsonValue
    {
    public:
        enum class Type
        {
            Null,
            Bool,
            Number,
            String,
            Array,
            Object,
        };

        JsonValue();
        JsonValue(bool Value);
        JsonValue(double Value);
        JsonValue(const char* Value);
        JsonValue(const std::string& Value);
        JsonValue(const JsonValue& Other);
        JsonValue(JsonValue&& Other) noexcept;
        ~JsonValue();

        JsonValue& operator=(const JsonValue& Other);
        JsonValue& operator=(JsonValue&& Other) noexcept;

        Type GetType() const;
        bool IsNull() const;
        bool IsString() const;
        bool IsObject() const;
        bool IsArray() const;
        bool IsNumber() const;
        bool IsBool() const;

        const JsonValue* Find(const std::string& Key) const;
        JsonValue* Find(const std::string& Key);

        std::string StringOr(const std::string& Fallback) const;
        double NumberOr(double Fallback) const;
        bool BoolOr(bool Fallback) const;

        std::string AsString() const;
        double AsNumber() const;
        bool AsBool() const;

        std::size_t Size() const;
        const JsonValue* At(std::size_t Index) const;

        void Set(const std::string& Key, const JsonValue& Value);
        void Push(const JsonValue& Value);

        std::string Serialize() const;

    private:
        struct Impl;
        std::unique_ptr<Impl> Data;
    };

    bool JsonParse(const std::string& Text, JsonValue& Out);
}
