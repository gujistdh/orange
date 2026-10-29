#pragma once

#include "orange/Common.h"
#include "orange/Gui/Property.h"

namespace Orange::Gui
{
    class Panel;

    class Module
    {
    public:
        Module(const std::string& Id, bool Enabled);
        virtual ~Module();

        const std::string& GetId() const;
        const std::string& GetName() const;
        const std::string& GetCategory() const;

        void SetName(const std::string& Value);
        void SetCategory(const std::string& Value);
        void SetDescription(const std::string& Value);

        bool IsOn() const;
        void SetOn(bool Value);

        bool IsKeybind() const;
        void SetKeybind(bool Value);

        bool IsVisible() const;
        void SetVisible(bool Value);

        float Animation() const;
        void SetAnimation(float Value);

        void SetInfoText(const std::string& Value);
        const std::string& GetInfoText() const;

        bool HasProps() const;
        Property* GetProp(const std::string& Name) const;

        double GetNum(const std::string& Name) const;
        bool GetBool(const std::string& Name) const;
        std::string GetText(const std::string& Name) const;
        int GetModeIndex(const std::string& Name) const;
        int GetCheckMask(const std::string& Name) const;
        std::string GetMode(const std::string& Name) const;

        void AddProperty(Property* Prop);
        const std::vector<Property*>& GetProperties() const;

        void SetApplyHandler(std::function<void()> Handler);
        void Apply();

        Panel* GetOwner() const;
        void SetOwner(Panel* Value);

        int GetKeyCode() const;
        void SetKeyCode(int Value);

        bool IsExpanded() const;
        void SetExpanded(bool Value);

        bool IsBinding() const;
        void SetBinding(bool Value);

        bool IsPinned() const;
        void SetPinned(bool Value);

        int GetSortIndex() const;
        void SetSortIndex(int Value);

    protected:
        std::string IdValue;
        std::string NameValue;
        std::string CategoryValue;
        std::string DescriptionValue;
        std::string InfoValue;

        bool EnabledValue;
        bool KeybindValue;
        bool VisibleValue;
        bool ExpandedValue;
        bool BindingValue;
        bool PinnedValue;

        int KeyCodeValue;
        int SortIndexValue;
        int ReservedA;
        int ReservedB;

        float AnimationValue;

        Panel* OwnerValue;
        std::vector<Property*> Properties;
        std::function<void()> ApplyHandler;
    };
}
