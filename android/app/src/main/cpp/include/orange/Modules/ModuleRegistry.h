#pragma once

#include "orange/Common.h"
#include "orange/Gui/Module.h"
#include "orange/Gui/Panel.h"

namespace Orange::Modules
{
    void RegisterCategory(const std::string& Name, Gui::Panel* Target);
    void RegisterAll();
    Gui::Module* Find(const std::string& Id);
    std::size_t Count();
    void ApplyBinder(const std::string& Id, Gui::Module* Module);
    std::size_t BinderCount();
}
