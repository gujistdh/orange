#pragma once

#include "orange/Common.h"

namespace Orange::Gui
{
    class NotifState
    {
    public:
        NotifState();
        NotifState(const NotifState& Other);
        ~NotifState();

        NotifState& operator=(const NotifState& Other);

        std::string Title;
        std::string Body;
        float Time;
        float Life;
        int Kind;
        bool Enabled;
    };
}
