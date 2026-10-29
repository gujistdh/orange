#include "orange/Gui/Module.h"
#include "orange/Modules/ModuleRegistry.h"

namespace Orange::Gui
{
    Module::Module(const std::string& Id, bool Enabled)
        : IdValue(Id), EnabledValue(Enabled), KeybindValue(false), VisibleValue(true),
          ExpandedValue(false), BindingValue(false), PinnedValue(false),
          KeyCodeValue(0), SortIndexValue(0), ReservedA(0), ReservedB(0),
          AnimationValue(Enabled ? 1.0f : 0.0f), OwnerValue(nullptr)
    {
    }

    Module::~Module()
    {
        for (Property* Prop : Properties)
        {
            delete Prop;
        }
        Properties.clear();
    }

    const std::string& Module::GetId() const { return IdValue; }
    const std::string& Module::GetName() const { return NameValue; }
    const std::string& Module::GetCategory() const { return CategoryValue; }

    void Module::SetName(const std::string& Value) { NameValue = Value; }
    void Module::SetCategory(const std::string& Value) { CategoryValue = Value; }
    void Module::SetDescription(const std::string& Value) { DescriptionValue = Value; }

    bool Module::IsOn() const { return EnabledValue; }
    void Module::SetOn(bool Value) { EnabledValue = Value; }

    bool Module::IsKeybind() const { return KeybindValue; }
    void Module::SetKeybind(bool Value) { KeybindValue = Value; }

    bool Module::IsVisible() const { return VisibleValue; }
    void Module::SetVisible(bool Value) { VisibleValue = Value; }

    float Module::Animation() const { return AnimationValue; }
    void Module::SetAnimation(float Value) { AnimationValue = Value; }

    void Module::SetInfoText(const std::string& Value) { InfoValue = Value; }
    const std::string& Module::GetInfoText() const { return InfoValue; }

    bool Module::HasProps() const
    {
        if (!Properties.empty())
        {
            return ExpandedValue;
        }
        return true;
    }

    Property* Module::GetProp(const std::string& Name) const
    {
        for (Property* Prop : Properties)
        {
            if (Prop && Prop->GetName() == Name)
            {
                return Prop;
            }
        }
        return nullptr;
    }

    double Module::GetNum(const std::string& Name) const
    {
        Property* Prop = GetProp(Name);
        return Prop ? Prop->GetNum() : 0.0;
    }

    bool Module::GetBool(const std::string& Name) const
    {
        Property* Prop = GetProp(Name);
        return Prop ? Prop->GetBool() : false;
    }

    std::string Module::GetText(const std::string& Name) const
    {
        Property* Prop = GetProp(Name);
        return Prop ? Prop->GetText() : std::string();
    }

    int Module::GetModeIndex(const std::string& Name) const
    {
        Property* Prop = GetProp(Name);
        return Prop ? Prop->GetModeIndex() : 0;
    }

    int Module::GetCheckMask(const std::string& Name) const
    {
        Property* Prop = GetProp(Name);
        return Prop ? Prop->GetCheckMask() : 0;
    }

    std::string Module::GetMode(const std::string& Name) const
    {
        Property* Prop = GetProp(Name);
        if (!Prop)
        {
            return std::string();
        }
        int Index = Prop->GetModeIndex();
        const std::vector<std::string>& Modes = Prop->Modes();
        if (Index < 0 || static_cast<std::size_t>(Index) >= Modes.size())
        {
            return std::string();
        }
        return Modes[static_cast<std::size_t>(Index)];
    }

    void Module::AddProperty(Property* Prop)
    {
        if (Prop)
        {
            Properties.push_back(Prop);
        }
    }

    const std::vector<Property*>& Module::GetProperties() const { return Properties; }

    void Module::SetApplyHandler(std::function<void()> Handler) { ApplyHandler = std::move(Handler); }

    void Module::Apply()
    {
        if (ApplyHandler)
        {
            ApplyHandler();
        }
        Orange::Modules::ApplyBinder(IdValue, this);
    }

    Panel* Module::GetOwner() const { return OwnerValue; }
    void Module::SetOwner(Panel* Value) { OwnerValue = Value; }

    int Module::GetKeyCode() const { return KeyCodeValue; }
    void Module::SetKeyCode(int Value) { KeyCodeValue = Value; }

    bool Module::IsExpanded() const { return ExpandedValue; }
    void Module::SetExpanded(bool Value) { ExpandedValue = Value; }

    bool Module::IsBinding() const { return BindingValue; }
    void Module::SetBinding(bool Value) { BindingValue = Value; }

    bool Module::IsPinned() const { return PinnedValue; }
    void Module::SetPinned(bool Value) { PinnedValue = Value; }

    int Module::GetSortIndex() const { return SortIndexValue; }
    void Module::SetSortIndex(int Value) { SortIndexValue = Value; }
}
