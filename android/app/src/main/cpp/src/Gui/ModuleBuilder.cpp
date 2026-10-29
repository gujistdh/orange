#include "orange/Gui/ModuleBuilder.h"

namespace Orange::Gui
{
    ModuleBuilder::ModuleBuilder(const std::string& Id, bool Enabled)
        : Instance(new Module(Id, Enabled))
    {
    }

    ModuleBuilder::~ModuleBuilder() = default;

    Module* ModuleBuilder::Build() { return Instance; }
    Module* ModuleBuilder::Get() const { return Instance; }

    ModuleBuilder& ModuleBuilder::Num(const std::string& Name, double Min, double Max, double Value, int Decimals)
    {
        Property* Prop = Property::MakeNum(Name, Min, Max, Value, Decimals);
        Owned.push_back(Prop);
        Instance->AddProperty(Prop);
        return *this;
    }

    ModuleBuilder& ModuleBuilder::Num(const std::string& Name, double Min, double Max, double Value)
    {
        return Num(Name, Min, Max, Value, 0);
    }

    ModuleBuilder& ModuleBuilder::Flag(const std::string& Name, bool Value)
    {
        Property* Prop = Property::MakeFlag(Name, Value);
        Owned.push_back(Prop);
        Instance->AddProperty(Prop);
        return *this;
    }

    ModuleBuilder& ModuleBuilder::Mode(const std::string& Name, const std::vector<std::string>& Modes)
    {
        return Mode(Name, Modes, 0);
    }

    ModuleBuilder& ModuleBuilder::Mode(const std::string& Name, const std::vector<std::string>& Modes, int Index)
    {
        Property* Prop = Property::MakeMode(Name, Modes, Index);
        Owned.push_back(Prop);
        Instance->AddProperty(Prop);
        return *this;
    }

    ModuleBuilder& ModuleBuilder::Check(const std::string& Name, const std::vector<std::string>& Modes, int Mask)
    {
        Property* Prop = Property::MakeCheck(Name, Modes, Mask);
        Owned.push_back(Prop);
        Instance->AddProperty(Prop);
        return *this;
    }

    ModuleBuilder& ModuleBuilder::Text(const std::string& Name, const std::string& Value)
    {
        Property* Prop = Property::MakeText(Name, Value);
        Owned.push_back(Prop);
        Instance->AddProperty(Prop);
        return *this;
    }

    ModuleBuilder& ModuleBuilder::Depend(const std::string& Name, int Mode)
    {
        (void)Name;
        (void)Mode;
        return *this;
    }

    ModuleBuilder& ModuleBuilder::Apply(std::function<void()> Handler)
    {
        Instance->SetApplyHandler(std::move(Handler));
        return *this;
    }
}
