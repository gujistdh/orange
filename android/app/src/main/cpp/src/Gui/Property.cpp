#include "orange/Gui/Property.h"

namespace Orange::Gui
{
    Property::Property()
        : KindValue(Kind::Number), MinValue(0.0), MaxValue(100.0), NumValue(0.0),
          DecimalsValue(0), IntValue(0), BoolValue(false)
    {
    }

    Property::~Property() = default;

    Property* Property::MakeNum(const std::string& Name, double Min, double Max, double Value, int Decimals)
    {
        Property* Result = new Property();
        Result->NameValue = Name;
        Result->KindValue = Kind::Number;
        Result->MinValue = Min;
        Result->MaxValue = Max;
        Result->NumValue = Value;
        Result->DecimalsValue = Decimals;
        return Result;
    }

    Property* Property::MakeMode(const std::string& Name, const std::vector<std::string>& Modes, int Index)
    {
        Property* Result = new Property();
        Result->NameValue = Name;
        Result->KindValue = Kind::Mode;
        Result->ModesValue = Modes;
        Result->IntValue = Index;
        return Result;
    }

    Property* Property::MakeCheck(const std::string& Name, const std::vector<std::string>& Modes, int Mask)
    {
        Property* Result = new Property();
        Result->NameValue = Name;
        Result->KindValue = Kind::Check;
        Result->ModesValue = Modes;
        Result->IntValue = Mask;
        return Result;
    }

    Property* Property::MakeFlag(const std::string& Name, bool Value)
    {
        Property* Result = new Property();
        Result->NameValue = Name;
        Result->KindValue = Kind::Flag;
        Result->BoolValue = Value;
        return Result;
    }

    Property* Property::MakeText(const std::string& Name, const std::string& Value)
    {
        Property* Result = new Property();
        Result->NameValue = Name;
        Result->KindValue = Kind::Text;
        Result->TextValue = Value;
        return Result;
    }

    const std::string& Property::GetName() const { return NameValue; }
    Property::Kind Property::GetKind() const { return KindValue; }

    bool Property::GetBool() const { return BoolValue; }
    double Property::GetNum() const { return NumValue; }
    int Property::GetModeIndex() const { return IntValue; }
    int Property::GetCheckMask() const { return IntValue; }
    const std::string& Property::GetText() const { return TextValue; }

    void Property::SetBool(bool Value) { BoolValue = Value; }
    void Property::SetNum(double Value) { NumValue = Value; }
    void Property::SetModeIndex(int Value) { IntValue = Value; }
    void Property::SetCheckMask(int Value) { IntValue = Value; }
    void Property::SetText(const std::string& Value) { TextValue = Value; }

    double Property::Min() const { return MinValue; }
    double Property::Max() const { return MaxValue; }
    int Property::Decimals() const { return DecimalsValue; }
    const std::vector<std::string>& Property::Modes() const { return ModesValue; }

    void Property::Bind(std::function<void()> InHandler) { Handler = std::move(InHandler); }

    void Property::Notify()
    {
        if (Handler)
        {
            Handler();
        }
    }

    bool CheckBitAt(const Property* Prop, int Bit)
    {
        if (!Prop)
        {
            return false;
        }
        return (Prop->GetCheckMask() & (1 << Bit)) != 0;
    }
}
