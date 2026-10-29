#pragma once

#include "orange/Common.h"

namespace Orange::Config
{
    class NbtNode
    {
    public:
        enum class Kind
        {
            Null,
            Byte,
            Short,
            Int,
            Long,
            Float,
            Double,
            String,
            Compound,
            List,
        };

        NbtNode();
        NbtNode(const NbtNode& Other);
        NbtNode(NbtNode&& Other) noexcept;
        ~NbtNode();

        NbtNode& operator=(const NbtNode& Other);
        NbtNode& operator=(NbtNode&& Other) noexcept;

        Kind GetKind() const;
        void SetKind(Kind Value);

        const std::string& GetKey() const;
        void SetKey(const std::string& Value);

        const std::string& GetString() const;
        void SetString(const std::string& Value);

        double GetNumber() const;
        void SetNumber(double Value);

        void AddChild(const NbtNode& Value);
        const std::vector<NbtNode>& GetChildren() const;
        std::vector<NbtNode>& Children();

        const NbtNode* Find(const std::string& Key) const;
        std::string Serialize() const;

    private:
        Kind KindValue;
        std::string KeyValue;
        std::string StringValue;
        double NumberValue;
        std::vector<NbtNode> ChildrenValue;
    };

    class NbtParser
    {
    public:
        explicit NbtParser(const std::string& Text);

        bool Parse(NbtNode& Out);

    private:
        void SkipWs();
        bool ParseValue(NbtNode& Out);
        bool ParseCompound(NbtNode& Out);
        bool ParseList(NbtNode& Out);
        bool ParseNumber(NbtNode& Out);
        bool ParseString(std::string& Out);
        bool ParseBareString(NbtNode& Out);
        bool ParseKey(std::string& Out);

        const std::string& Source;
        std::size_t Cursor;
    };
}
