#include "orange/Config/NbtNode.h"

namespace Orange::Config
{
    NbtNode::NbtNode() : KindValue(Kind::Null), NumberValue(0.0)
    {
    }

    NbtNode::NbtNode(const NbtNode& Other) = default;
    NbtNode::NbtNode(NbtNode&& Other) noexcept = default;
    NbtNode::~NbtNode() = default;
    NbtNode& NbtNode::operator=(const NbtNode& Other) = default;
    NbtNode& NbtNode::operator=(NbtNode&& Other) noexcept = default;

    NbtNode::Kind NbtNode::GetKind() const { return KindValue; }
    void NbtNode::SetKind(Kind Value) { KindValue = Value; }

    const std::string& NbtNode::GetKey() const { return KeyValue; }
    void NbtNode::SetKey(const std::string& Value) { KeyValue = Value; }

    const std::string& NbtNode::GetString() const { return StringValue; }
    void NbtNode::SetString(const std::string& Value) { StringValue = Value; KindValue = Kind::String; }

    double NbtNode::GetNumber() const { return NumberValue; }
    void NbtNode::SetNumber(double Value) { NumberValue = Value; KindValue = Kind::Double; }

    void NbtNode::AddChild(const NbtNode& Value) { ChildrenValue.push_back(Value); }
    const std::vector<NbtNode>& NbtNode::GetChildren() const { return ChildrenValue; }
    std::vector<NbtNode>& NbtNode::Children() { return ChildrenValue; }

    const NbtNode* NbtNode::Find(const std::string& Key) const
    {
        for (const NbtNode& Child : ChildrenValue)
        {
            if (Child.KeyValue == Key)
            {
                return &Child;
            }
        }
        return nullptr;
    }

    std::string NbtNode::Serialize() const
    {
        switch (KindValue)
        {
        case Kind::Compound:
        {
            std::string Out = "{";
            for (std::size_t Index = 0; Index < ChildrenValue.size(); ++Index)
            {
                if (Index)
                {
                    Out += ",";
                }
                Out += ChildrenValue[Index].KeyValue;
                Out += ":";
                Out += ChildrenValue[Index].Serialize();
            }
            Out += "}";
            return Out;
        }
        case Kind::List:
        {
            std::string Out = "[";
            for (std::size_t Index = 0; Index < ChildrenValue.size(); ++Index)
            {
                if (Index)
                {
                    Out += ",";
                }
                Out += ChildrenValue[Index].Serialize();
            }
            Out += "]";
            return Out;
        }
        case Kind::String:
        {
            std::string Out = "\"";
            Out += StringValue;
            Out += "\"";
            return Out;
        }
        case Kind::Byte:
        case Kind::Short:
        case Kind::Int:
        case Kind::Long:
        case Kind::Float:
        case Kind::Double:
        {
            char Buffer[64];
            std::snprintf(Buffer, sizeof(Buffer), "%g", NumberValue);
            return std::string(Buffer);
        }
        default:
            return "null";
        }
    }

    NbtParser::NbtParser(const std::string& Text) : Source(Text), Cursor(0)
    {
    }

    void NbtParser::SkipWs()
    {
        while (Cursor < Source.size())
        {
            char Ch = Source[Cursor];
            if (Ch == ' ' || Ch == '\t' || Ch == '\n' || Ch == '\r')
            {
                ++Cursor;
                continue;
            }
            break;
        }
    }

    bool NbtParser::Parse(NbtNode& Out)
    {
        SkipWs();
        if (!ParseValue(Out))
        {
            return false;
        }
        SkipWs();
        return true;
    }

    bool NbtParser::ParseKey(std::string& Out)
    {
        SkipWs();
        std::size_t Start = Cursor;
        while (Cursor < Source.size())
        {
            char Ch = Source[Cursor];
            if (Ch == ':' || Ch == '{' || Ch == '[' || Ch == ',' || Ch == '}' || Ch == ']')
            {
                break;
            }
            ++Cursor;
        }
        Out = Source.substr(Start, Cursor - Start);
        while (!Out.empty() && (Out.back() == ' ' || Out.back() == '\t'))
        {
            Out.pop_back();
        }
        return !Out.empty();
    }

    bool NbtParser::ParseString(std::string& Out)
    {
        if (Cursor >= Source.size() || Source[Cursor] != '"')
        {
            return false;
        }
        ++Cursor;
        while (Cursor < Source.size())
        {
            char Ch = Source[Cursor++];
            if (Ch == '"')
            {
                return true;
            }
            if (Ch == '\\' && Cursor < Source.size())
            {
                char Escape = Source[Cursor++];
                switch (Escape)
                {
                case 'n': Out.push_back('\n'); break;
                case 'r': Out.push_back('\r'); break;
                case 't': Out.push_back('\t'); break;
                default: Out.push_back(Escape); break;
                }
                continue;
            }
            Out.push_back(Ch);
        }
        return false;
    }

    bool NbtParser::ParseBareString(NbtNode& Out)
    {
        std::size_t Start = Cursor;
        while (Cursor < Source.size())
        {
            char Ch = Source[Cursor];
            if (Ch == ',' || Ch == '}' || Ch == ']' || Ch == ' ' || Ch == '\n' || Ch == '\r' || Ch == '\t')
            {
                break;
            }
            ++Cursor;
        }
        std::string Text = Source.substr(Start, Cursor - Start);
        if (Text.empty())
        {
            return false;
        }
        Out.SetString(Text);
        return true;
    }

    bool NbtParser::ParseNumber(NbtNode& Out)
    {
        std::size_t Start = Cursor;
        if (Cursor < Source.size() && (Source[Cursor] == '-' || Source[Cursor] == '+'))
        {
            ++Cursor;
        }
        while (Cursor < Source.size())
        {
            char Ch = Source[Cursor];
            if ((Ch >= '0' && Ch <= '9') || Ch == '.' || Ch == 'e' || Ch == 'E' || Ch == '-' || Ch == '+')
            {
                ++Cursor;
                continue;
            }
            break;
        }
        std::string Text = Source.substr(Start, Cursor - Start);
        char* End = nullptr;
        double Value = std::strtod(Text.c_str(), &End);
        if (End == Text.c_str())
        {
            return false;
        }
        char Suffix = 0;
        if (Cursor < Source.size())
        {
            char Ch = Source[Cursor];
            if (Ch == 'b' || Ch == 'B' || Ch == 's' || Ch == 'S' || Ch == 'l' || Ch == 'L' || Ch == 'f' || Ch == 'F' || Ch == 'd' || Ch == 'D')
            {
                Suffix = static_cast<char>(std::tolower(Ch));
                ++Cursor;
            }
        }
        switch (Suffix)
        {
        case 'b': Out.SetKind(NbtNode::Kind::Byte); break;
        case 's': Out.SetKind(NbtNode::Kind::Short); break;
        case 'l': Out.SetKind(NbtNode::Kind::Long); break;
        case 'f': Out.SetKind(NbtNode::Kind::Float); break;
        default: Out.SetKind(NbtNode::Kind::Double); break;
        }
        Out.SetNumber(Value);
        return true;
    }

    bool NbtParser::ParseList(NbtNode& Out)
    {
        ++Cursor;
        Out.SetKind(NbtNode::Kind::List);
        SkipWs();
        if (Cursor < Source.size() && Source[Cursor] == ']')
        {
            ++Cursor;
            return true;
        }
        while (Cursor < Source.size())
        {
            NbtNode Child;
            if (!ParseValue(Child))
            {
                return false;
            }
            Out.AddChild(Child);
            SkipWs();
            if (Cursor < Source.size() && Source[Cursor] == ',')
            {
                ++Cursor;
                SkipWs();
                continue;
            }
            if (Cursor < Source.size() && Source[Cursor] == ']')
            {
                ++Cursor;
                return true;
            }
            return false;
        }
        return false;
    }

    bool NbtParser::ParseCompound(NbtNode& Out)
    {
        ++Cursor;
        Out.SetKind(NbtNode::Kind::Compound);
        SkipWs();
        if (Cursor < Source.size() && Source[Cursor] == '}')
        {
            ++Cursor;
            return true;
        }
        while (Cursor < Source.size())
        {
            std::string Key;
            if (!ParseKey(Key))
            {
                return false;
            }
            SkipWs();
            if (Cursor < Source.size() && Source[Cursor] == ':')
            {
                ++Cursor;
            }
            NbtNode Child;
            if (!ParseValue(Child))
            {
                return false;
            }
            Child.SetKey(Key);
            Out.AddChild(Child);
            SkipWs();
            if (Cursor < Source.size() && Source[Cursor] == ',')
            {
                ++Cursor;
                SkipWs();
                continue;
            }
            if (Cursor < Source.size() && Source[Cursor] == '}')
            {
                ++Cursor;
                return true;
            }
            return false;
        }
        return false;
    }

    bool NbtParser::ParseValue(NbtNode& Out)
    {
        SkipWs();
        if (Cursor >= Source.size())
        {
            return false;
        }
        char Ch = Source[Cursor];
        if (Ch == '{')
        {
            return ParseCompound(Out);
        }
        if (Ch == '[')
        {
            return ParseList(Out);
        }
        if (Ch == '"')
        {
            std::string Text;
            if (!ParseString(Text))
            {
                return false;
            }
            Out.SetString(Text);
            return true;
        }
        if (Source.compare(Cursor, 4, "true") == 0)
        {
            Cursor += 4;
            Out.SetKind(NbtNode::Kind::Byte);
            Out.SetNumber(1.0);
            return true;
        }
        if (Source.compare(Cursor, 5, "false") == 0)
        {
            Cursor += 5;
            Out.SetKind(NbtNode::Kind::Byte);
            Out.SetNumber(0.0);
            return true;
        }
        if (Ch == '-' || (Ch >= '0' && Ch <= '9'))
        {
            return ParseNumber(Out);
        }
        return ParseBareString(Out);
    }
}
