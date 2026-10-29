#include "orange/Gui/Panel.h"

#include <algorithm>

namespace Orange::Gui
{
    Panel::Panel(const std::string& Name, const std::string& Category)
        : NameValue(Name), CategoryValue(Category), ExpandedValue(true), VisibleValue(true),
          ScrollValue(0.0f), AnimationValue(0.0f), IndexValue(0)
    {
    }

    Panel::~Panel() = default;

    const std::string& Panel::GetName() const { return NameValue; }
    const std::string& Panel::GetCategory() const { return CategoryValue; }

    void Panel::AddModule(Module* Value)
    {
        if (Value)
        {
            Value->SetOwner(this);
            Modules.push_back(Value);
        }
    }

    void Panel::RemoveModule(Module* Value)
    {
        Modules.erase(std::remove(Modules.begin(), Modules.end(), Value), Modules.end());
    }

    const std::vector<Module*>& Panel::GetModules() const { return Modules; }

    void Panel::SortModules()
    {
        std::stable_sort(Modules.begin(), Modules.end(), [](Module* Left, Module* Right)
        {
            return Left->GetSortIndex() < Right->GetSortIndex();
        });
    }

    Module* Panel::FindModule(const std::string& Id) const
    {
        for (Module* Item : Modules)
        {
            if (Item && Item->GetId() == Id)
            {
                return Item;
            }
        }
        return nullptr;
    }

    bool Panel::IsExpanded() const { return ExpandedValue; }
    void Panel::SetExpanded(bool Value) { ExpandedValue = Value; }

    float Panel::Scroll() const { return ScrollValue; }
    void Panel::SetScroll(float Value) { ScrollValue = Value; }

    float Panel::Animation() const { return AnimationValue; }
    void Panel::SetAnimation(float Value) { AnimationValue = Value; }

    int Panel::GetIndex() const { return IndexValue; }
    void Panel::SetIndex(int Value) { IndexValue = Value; }

    bool Panel::IsVisible() const { return VisibleValue; }
    void Panel::SetVisible(bool Value) { VisibleValue = Value; }
}
