#include "orange/Gui/JsonValue.h"

#include <cstdlib>

namespace Orange::Gui
{
    struct JsonValue::Impl
    {
        Type Kind = Type::Null;
        bool BoolValue = false;
        double NumValue = 0.0;
        std::string StrValue;
        std::vector<JsonValue> Items;
        std::vector<std::pair<std::string, JsonValue>> Members;
    };

    JsonValue::JsonValue() : Data(std::make_unique<Impl>())
    {
    }

    JsonValue::JsonValue(bool Value) : JsonValue()
    {
        Data->Kind = Type::Bool;
        Data->BoolValue = Value;
    }

    JsonValue::JsonValue(double Value) : JsonValue()
    {
        Data->Kind = Type::Number;
        Data->NumValue = Value;
    }

    JsonValue::JsonValue(const char* Value) : JsonValue()
    {
        Data->Kind = Type::String;
        Data->StrValue = Value ? Value : "";
    }

    JsonValue::JsonValue(const std::string& Value) : JsonValue()
    {
        Data->Kind = Type::String;
        Data->StrValue = Value;
    }

    JsonValue::JsonValue(const JsonValue& Other) : Data(std::make_unique<Impl>(*Other.Data))
    {
    }

    JsonValue::JsonValue(JsonValue&& Other) noexcept : Data(std::move(Other.Data))
    {
        if (!Data)
        {
            Data = std::make_unique<Impl>();
        }
    }

    JsonValue::~JsonValue() = default;

    JsonValue& JsonValue::operator=(const JsonValue& Other)
    {
        if (this != &Other)
        {
            Data = std::make_unique<Impl>(*Other.Data);
        }
        return *this;
    }

    JsonValue& JsonValue::operator=(JsonValue&& Other) noexcept
    {
        if (this != &Other)
        {
            Data = std::move(Other.Data);
            if (!Data)
            {
                Data = std::make_unique<Impl>();
            }
        }
        return *this;
    }

    JsonValue::Type JsonValue::GetType() const { return Data->Kind; }
    bool JsonValue::IsNull() const { return Data->Kind == Type::Null; }
    bool JsonValue::IsString() const { return Data->Kind == Type::String; }
    bool JsonValue::IsObject() const { return Data->Kind == Type::Object; }
    bool JsonValue::IsArray() const { return Data->Kind == Type::Array; }
    bool JsonValue::IsNumber() const { return Data->Kind == Type::Number; }
    bool JsonValue::IsBool() const { return Data->Kind == Type::Bool; }

    const JsonValue* JsonValue::Find(const std::string& Key) const
    {
        for (const auto& Member : Data->Members)
        {
            if (Member.first == Key)
            {
                return &Member.second;
            }
        }
        return nullptr;
    }

    JsonValue* JsonValue::Find(const std::string& Key)
    {
        for (auto& Member : Data->Members)
        {
            if (Member.first == Key)
            {
                return &Member.second;
            }
        }
        return nullptr;
    }

    std::string JsonValue::StringOr(const std::string& Fallback) const
    {
        return Data->Kind == Type::String ? Data->StrValue : Fallback;
    }

    double JsonValue::NumberOr(double Fallback) const
    {
        return Data->Kind == Type::Number ? Data->NumValue : Fallback;
    }

    bool JsonValue::BoolOr(bool Fallback) const
    {
        return Data->Kind == Type::Bool ? Data->BoolValue : Fallback;
    }

    std::string JsonValue::AsString() const { return Data->StrValue; }
    double JsonValue::AsNumber() const { return Data->NumValue; }
    bool JsonValue::AsBool() const { return Data->BoolValue; }

    std::size_t JsonValue::Size() const
    {
        return Data->Kind == Type::Array ? Data->Items.size() : Data->Members.size();
    }

    const JsonValue* JsonValue::At(std::size_t Index) const
    {
        if (Index >= Data->Items.size())
        {
            return nullptr;
        }
        return &Data->Items[Index];
    }

    void JsonValue::Set(const std::string& Key, const JsonValue& Value)
    {
        Data->Kind = Type::Object;
        for (auto& Member : Data->Members)
        {
            if (Member.first == Key)
            {
                Member.second = Value;
                return;
            }
        }
        Data->Members.emplace_back(Key, Value);
    }

    void JsonValue::Push(const JsonValue& Value)
    {
        Data->Kind = Type::Array;
        Data->Items.push_back(Value);
    }

    static void Escape(const std::string& Text, std::string& Out)
    {
        Out.push_back('"');
        for (char Ch : Text)
        {
            switch (Ch)
            {
            case '"': Out += "\\\""; break;
            case '\\': Out += "\\\\"; break;
            case '\n': Out += "\\n"; break;
            case '\r': Out += "\\r"; break;
            case '\t': Out += "\\t"; break;
            default: Out.push_back(Ch); break;
            }
        }
        Out.push_back('"');
    }

    std::string JsonValue::Serialize() const
    {
        std::string Out;
        switch (Data->Kind)
        {
        case Type::Null:
            Out = "null";
            break;
        case Type::Bool:
            Out = Data->BoolValue ? "true" : "false";
            break;
        case Type::Number:
        {
            char Buffer[64];
            std::snprintf(Buffer, sizeof(Buffer), "%g", Data->NumValue);
            Out = Buffer;
            break;
        }
        case Type::String:
            Escape(Data->StrValue, Out);
            break;
        case Type::Array:
        {
            Out.push_back('[');
            for (std::size_t Index = 0; Index < Data->Items.size(); ++Index)
            {
                if (Index)
                {
                    Out.push_back(',');
                }
                Out += Data->Items[Index].Serialize();
            }
            Out.push_back(']');
            break;
        }
        case Type::Object:
        {
            Out.push_back('{');
            for (std::size_t Index = 0; Index < Data->Members.size(); ++Index)
            {
                if (Index)
                {
                    Out.push_back(',');
                }
                Escape(Data->Members[Index].first, Out);
                Out.push_back(':');
                Out += Data->Members[Index].second.Serialize();
            }
            Out.push_back('}');
            break;
        }
        }
        return Out;
    }

    class JsonReader
    {
    public:
        explicit JsonReader(const std::string& Source) : Text(Source)
        {
        }

        bool Parse(JsonValue& Out)
        {
            Skip();
            if (!ParseValue(Out))
            {
                return false;
            }
            Skip();
            return true;
        }

    private:
        void Skip()
        {
            while (Cursor < Text.size() && (Text[Cursor] == ' ' || Text[Cursor] == '\t' ||
                                            Text[Cursor] == '\n' || Text[Cursor] == '\r'))
            {
                ++Cursor;
            }
        }

        bool ParseValue(JsonValue& Out)
        {
            Skip();
            if (Cursor >= Text.size())
            {
                return false;
            }
            char Ch = Text[Cursor];
            if (Ch == '{')
            {
                return ParseObject(Out);
            }
            if (Ch == '[')
            {
                return ParseArray(Out);
            }
            if (Ch == '"')
            {
                std::string Value;
                if (!ParseString(Value))
                {
                    return false;
                }
                Out = JsonValue(Value);
                return true;
            }
            if (Ch == 't')
            {
                if (Text.compare(Cursor, 4, "true") == 0)
                {
                    Cursor += 4;
                    Out = JsonValue(true);
                    return true;
                }
                return false;
            }
            if (Ch == 'f')
            {
                if (Text.compare(Cursor, 5, "false") == 0)
                {
                    Cursor += 5;
                    Out = JsonValue(false);
                    return true;
                }
                return false;
            }
            if (Ch == 'n')
            {
                if (Text.compare(Cursor, 4, "null") == 0)
                {
                    Cursor += 4;
                    Out = JsonValue();
                    return true;
                }
                return false;
            }
            char* End = nullptr;
            double Number = std::strtod(Text.c_str() + Cursor, &End);
            if (End == Text.c_str() + Cursor)
            {
                return false;
            }
            Cursor = static_cast<std::size_t>(End - Text.c_str());
            Out = JsonValue(Number);
            return true;
        }

        bool ParseObject(JsonValue& Out)
        {
            ++Cursor;
            Out = JsonValue();
            Skip();
            if (Cursor < Text.size() && Text[Cursor] == '}')
            {
                ++Cursor;
                return true;
            }
            while (Cursor < Text.size())
            {
                Skip();
                std::string Key;
                if (!ParseString(Key))
                {
                    return false;
                }
                Skip();
                if (Cursor >= Text.size() || Text[Cursor] != ':')
                {
                    return false;
                }
                ++Cursor;
                JsonValue Member;
                if (!ParseValue(Member))
                {
                    return false;
                }
                Out.Set(Key, Member);
                Skip();
                if (Cursor < Text.size() && Text[Cursor] == ',')
                {
                    ++Cursor;
                    continue;
                }
                if (Cursor < Text.size() && Text[Cursor] == '}')
                {
                    ++Cursor;
                    return true;
                }
                return false;
            }
            return false;
        }

        bool ParseArray(JsonValue& Out)
        {
            ++Cursor;
            Out = JsonValue();
            Skip();
            if (Cursor < Text.size() && Text[Cursor] == ']')
            {
                ++Cursor;
                return true;
            }
            while (Cursor < Text.size())
            {
                JsonValue Item;
                if (!ParseValue(Item))
                {
                    return false;
                }
                Out.Push(Item);
                Skip();
                if (Cursor < Text.size() && Text[Cursor] == ',')
                {
                    ++Cursor;
                    continue;
                }
                if (Cursor < Text.size() && Text[Cursor] == ']')
                {
                    ++Cursor;
                    return true;
                }
                return false;
            }
            return false;
        }

        bool ParseString(std::string& Out)
        {
            if (Cursor >= Text.size() || Text[Cursor] != '"')
            {
                return false;
            }
            ++Cursor;
            while (Cursor < Text.size())
            {
                char Ch = Text[Cursor++];
                if (Ch == '"')
                {
                    return true;
                }
                if (Ch == '\\' && Cursor < Text.size())
                {
                    char EscapeCh = Text[Cursor++];
                    switch (EscapeCh)
                    {
                    case 'n': Out.push_back('\n'); break;
                    case 'r': Out.push_back('\r'); break;
                    case 't': Out.push_back('\t'); break;
                    case 'b': Out.push_back('\b'); break;
                    case 'f': Out.push_back('\f'); break;
                    case 'u':
                    {
                        if (Cursor + 4 > Text.size())
                        {
                            return false;
                        }
                        unsigned int Code = 0;
                        for (int Index = 0; Index < 4; ++Index)
                        {
                            char Hex = Text[Cursor++];
                            Code <<= 4;
                            if (Hex >= '0' && Hex <= '9') Code |= static_cast<unsigned int>(Hex - '0');
                            else if (Hex >= 'a' && Hex <= 'f') Code |= static_cast<unsigned int>(Hex - 'a' + 10);
                            else if (Hex >= 'A' && Hex <= 'F') Code |= static_cast<unsigned int>(Hex - 'A' + 10);
                            else return false;
                        }
                        if (Code < 0x80)
                        {
                            Out.push_back(static_cast<char>(Code));
                        }
                        else if (Code < 0x800)
                        {
                            Out.push_back(static_cast<char>(0xC0 | (Code >> 6)));
                            Out.push_back(static_cast<char>(0x80 | (Code & 0x3F)));
                        }
                        else
                        {
                            Out.push_back(static_cast<char>(0xE0 | (Code >> 12)));
                            Out.push_back(static_cast<char>(0x80 | ((Code >> 6) & 0x3F)));
                            Out.push_back(static_cast<char>(0x80 | (Code & 0x3F)));
                        }
                        break;
                    }
                    default:
                        Out.push_back(EscapeCh);
                        break;
                    }
                    continue;
                }
                Out.push_back(Ch);
            }
            return false;
        }

        const std::string& Text;
        std::size_t Cursor = 0;
    };

    bool JsonParse(const std::string& Text, JsonValue& Out)
    {
        JsonReader Reader(Text);
        return Reader.Parse(Out);
    }
}
