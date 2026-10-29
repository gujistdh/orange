#include "orange/Gui/NotifState.h"

namespace Orange::Gui
{
    NotifState::NotifState() : Time(0.0f), Life(3.0f), Kind(0), Enabled(false)
    {
    }

    NotifState::NotifState(const NotifState& Other) = default;

    NotifState::~NotifState() = default;

    NotifState& NotifState::operator=(const NotifState& Other) = default;
}
