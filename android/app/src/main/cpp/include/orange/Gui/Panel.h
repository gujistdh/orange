#pragma once

#include "orange/Common.h"
#include "orange/Gui/Module.h"

namespace Orange::Gui
{
    class Panel
    {
    public:
        Panel(const std::string& Name, const std::string& Category);
        ~Panel();

        const std::string& GetName() const;
        const std::string& GetCategory() const;

        void AddModule(Module* Value);
        void RemoveModule(Module* Value);
        const std::vector<Module*>& GetModules() const;

        void SortModules();

        Module* FindModule(const std::string& Id) const;

        bool IsExpanded() const;
        void SetExpanded(bool Value);

        float Scroll() const;
        void SetScroll(float Value);

        float Animation() const;
        void SetAnimation(float Value);

        int GetIndex() const;
        void SetIndex(int Value);

        bool IsVisible() const;
        void SetVisible(bool Value);

    private:
        std::string NameValue;
        std::string CategoryValue;
        bool ExpandedValue;
        bool VisibleValue;
        float ScrollValue;
        float AnimationValue;
        int IndexValue;
        std::vector<Module*> Modules;
    };
}
