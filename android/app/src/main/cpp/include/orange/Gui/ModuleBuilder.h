#pragma once

#include "orange/Common.h"
#include "orange/Gui/Module.h"

namespace Orange::Gui
{
    class ModuleBuilder
    {
    public:
        ModuleBuilder(const std::string& Id, bool Enabled);
        ~ModuleBuilder();

        Module* Build();

        ModuleBuilder& Num(const std::string& Name, double Min, double Max, double Value, int Decimals);
        ModuleBuilder& Num(const std::string& Name, double Min, double Max, double Value);
        ModuleBuilder& Flag(const std::string& Name, bool Value);
        ModuleBuilder& Mode(const std::string& Name, const std::vector<std::string>& Modes);
        ModuleBuilder& Mode(const std::string& Name, const std::vector<std::string>& Modes, int Index);
        ModuleBuilder& Check(const std::string& Name, const std::vector<std::string>& Modes, int Mask);
        ModuleBuilder& Text(const std::string& Name, const std::string& Value);
        ModuleBuilder& Depend(const std::string& Name, int Mode);
        ModuleBuilder& Apply(std::function<void()> Handler);

        Module* Get() const;

    private:
        Module* Instance;
        std::vector<Property*> Owned;
    };
}
