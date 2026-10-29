#pragma once

#include "orange/Common.h"

namespace Orange::Modules
{
    void InstallAll();
    void TickAll();
    void RenderAll();

    namespace AutoTool { void ProcessTick(void* Actor); }
    namespace EntityRiding { void ProcessTick(void* Actor); }
    namespace BigTop { void ProcessTick(void* Actor); }
    namespace Enchant { void ProcessTick(void* Actor); }
    namespace GlobalNightVision { void ProcessTick(void* Actor); }
    namespace Inventory { void ProcessTick(void* Actor); }
    namespace ViewCommandBlock { void ProcessTick(void* Actor); }
    namespace ItemSet { void ProcessTick(void* Actor); }
    namespace ModelFace { void ProcessTick(); }
    namespace AutoApple { void ProcessTick(void* Actor); }
    namespace Quickbuild { void ProcessTick(void* Actor); }
    namespace PlayerAbility { void ProcessTick(void* Actor); }
    namespace Nbt { void ProcessTick(void* Actor); }
    namespace ParticleEditor { void ProcessTick(void* Actor); }
    namespace SoundEditor { void ProcessTick(void* Actor); }
    namespace EffectEditor { void ProcessTick(void* Actor); }
}
