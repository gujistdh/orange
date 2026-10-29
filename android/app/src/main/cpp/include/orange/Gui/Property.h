#pragma once

#include "orange/Common.h"

namespace Orange::Gui
{
    class Module;

    class Property
    {
    public:
        enum class Kind
        {
            Number,
            Flag,
            Mode,
            Check,
            Text,
            Color,
        };

        Property();
        ~Property();

        static Property* MakeNum(const std::string& Name, double Min, double Max, double Value, int Decimals);
        static Property* MakeMode(const std::string& Name, const std::vector<std::string>& Modes, int Index);
        static Property* MakeCheck(const std::string& Name, const std::vector<std::string>& Modes, int Mask);
        static Property* MakeFlag(const std::string& Name, bool Value);
        static Property* MakeText(const std::string& Name, const std::string& Value);

        const std::string& GetName() const;
        Kind GetKind() const;

        bool GetBool() const;
        double GetNum() const;
        int GetModeIndex() const;
        int GetCheckMask() const;
        const std::string& GetText() const;

        void SetBool(bool Value);
        void SetNum(double Value);
        void SetModeIndex(int Value);
        void SetCheckMask(int Value);
        void SetText(const std::string& Value);

        double Min() const;
        double Max() const;
        int Decimals() const;
        const std::vector<std::string>& Modes() const;

        void Bind(std::function<void()> Handler);
        void Notify();

    private:
        std::string NameValue;
        Kind KindValue;
        double MinValue;
        double MaxValue;
        double NumValue;
        int DecimalsValue;
        int IntValue;
        bool BoolValue;
        std::string TextValue;
        std::vector<std::string> ModesValue;
        std::function<void()> Handler;
    };

    bool CheckBitAt(const Property* Prop, int Bit);
}
