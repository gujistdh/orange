#pragma once

#include "orange/Modules/ModuleRegistry.h"

namespace Orange::Modules::Combat
{
    void BindAttackCount(Gui::Module* Module);
    void BindAttackDamage(Gui::Module* Module);
    void BindAttackEffects(Gui::Module* Module);
    void BindAttackMessage(Gui::Module* Module);
    void BindAuraRing(Gui::Module* Module);
    void BindAutoBow(Gui::Module* Module);
    void BindAutoFollow(Gui::Module* Module);
    void BindCircle(Gui::Module* Module);
    void BindCounterattack(Gui::Module* Module);
    void BindEntityRiding(Gui::Module* Module);
    void BindHitbox(Gui::Module* Module);
    void BindInfiniteAura(Gui::Module* Module);
    void BindKillAura(Gui::Module* Module);
    void BindKillMessage(Gui::Module* Module);
    void BindKillis(Gui::Module* Module);
    void BindKnockback(Gui::Module* Module);
    void BindSelfTrap(Gui::Module* Module);
    void BindSpacing(Gui::Module* Module);
    void BindTrap(Gui::Module* Module);
}
