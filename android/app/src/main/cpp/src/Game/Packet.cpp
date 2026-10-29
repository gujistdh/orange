#include "orange/Game/Packet.h"
#include "orange/Game/Offsets.h"
#include "orange/Game/Sdk.h"
#include "orange/Core/Main.h"

#include <cmath>

namespace Orange::Game
{
    namespace
    {
        template <typename Fn>
        Fn Resolve(Orange::Offsets::Slot Index)
        {
            intptr_t Offset = Orange::Offsets::Load(Index);
            if (!Offset || !Orange::Core::BaseAddress)
            {
                return nullptr;
            }
            return reinterpret_cast<Fn>(Orange::Core::BaseAddress + static_cast<Address>(Offset));
        }
    }

    bool Packet::IsNetworkReady()
    {
        return Orange::Core::BaseAddress != 0;
    }

    void* Packet::LocalPlayer()
    {
        return Actor::GetLocalPlayerInstance();
    }

    void* Packet::GetRuntimeId(void* ActorValue)
    {
        using Fn = void* (*)(void*);
        Fn Target = Resolve<Fn>(Orange::Offsets::Slot::Global_runtimeIdGetter);
        if (!Target)
        {
            return nullptr;
        }
        return Target(ActorValue);
    }

    BlockFace Packet::PickFace(float X, float Y, float Z, const BlockPos& Position)
    {
        float DeltaX = X - (static_cast<float>(Position.X) + 0.5f);
        float DeltaY = Y - (static_cast<float>(Position.Y) + 0.5f);
        float DeltaZ = Z - (static_cast<float>(Position.Z) + 0.5f);
        float AbsX = std::fabs(DeltaX);
        float AbsY = std::fabs(DeltaY);
        float AbsZ = std::fabs(DeltaZ);

        if (AbsY < AbsX || AbsY < AbsZ)
        {
            if (AbsX < AbsZ)
            {
                return DeltaZ < 0.0f ? BlockFace::North : BlockFace::South;
            }
            return DeltaX < 0.0f ? BlockFace::West : BlockFace::East;
        }
        return DeltaY >= 0.0f ? BlockFace::Up : BlockFace::Down;
    }

    bool Packet::SendPlayerAction(PlayerAction Action, const BlockPos& Position, int Face)
    {
        if (!IsNetworkReady())
        {
            return false;
        }
        intptr_t HandlerOffset = Orange::Offsets::Load(Orange::Offsets::Slot::SendToServer);
        intptr_t PayloadCtor = Orange::Offsets::Load(Orange::Offsets::Slot::PlayerActionPayloadCtor);
        intptr_t PacketCtor = Orange::Offsets::Load(Orange::Offsets::Slot::PlayerActionPacketCtor);
        if (!HandlerOffset || !PayloadCtor || !PacketCtor)
        {
            return false;
        }
        void* Handler = *reinterpret_cast<void**>(reinterpret_cast<Byte*>(LocalPlayer()) + Orange::Offsets::Load(Orange::Offsets::Slot::LocalPlayer_sendPacketVtableOffset));
        if (!Handler)
        {
            return false;
        }
        unsigned char Payload[64] = { 0 };
        unsigned char Buffer[160] = { 0 };
        int Coordinates[3] = { Position.X, Position.Y, Position.Z };
        using PayloadCtorFn = void (*)(void*, int, int*, int);
        using PacketCtorFn = void (*)(void*, void*);
        using SendFn = void (*)(void*);
        PayloadCtorFn BuildPayload = reinterpret_cast<PayloadCtorFn>(Orange::Core::BaseAddress + static_cast<Address>(PayloadCtor));
        PacketCtorFn BuildPacket = reinterpret_cast<PacketCtorFn>(Orange::Core::BaseAddress + static_cast<Address>(PacketCtor));
        SendFn Send = reinterpret_cast<SendFn>(Orange::Core::BaseAddress + static_cast<Address>(HandlerOffset));
        BuildPayload(Payload, static_cast<int>(Action), Coordinates, Face);
        BuildPacket(Buffer, Payload);
        Send(Handler);
        return true;
    }

    bool Packet::SendCommandRequest(const std::string& CommandValue)
    {
        if (!IsNetworkReady() || CommandValue.empty())
        {
            return false;
        }
        intptr_t HandlerOffset = Orange::Offsets::Load(Orange::Offsets::Slot::SendToServer);
        intptr_t VtableOffset = Orange::Offsets::Load(Orange::Offsets::Slot::CommandRequestPacket_vtable);
        if (!HandlerOffset || !VtableOffset)
        {
            return false;
        }
        void* Handler = *reinterpret_cast<void**>(reinterpret_cast<Byte*>(LocalPlayer()) + Orange::Offsets::Load(Orange::Offsets::Slot::LocalPlayer_sendPacketVtableOffset));
        if (!Handler)
        {
            return false;
        }
        using SendFn = void (*)(void*);
        SendFn Send = reinterpret_cast<SendFn>(Orange::Core::BaseAddress + static_cast<Address>(HandlerOffset));
        Send(Handler);
        (void)CommandValue;
        return true;
    }

    bool Packet::SendText(const std::string& TextValue)
    {
        if (!IsNetworkReady() || TextValue.empty())
        {
            return false;
        }
        intptr_t HandlerOffset = Orange::Offsets::Load(Orange::Offsets::Slot::SendToServer);
        intptr_t VtableOffset = Orange::Offsets::Load(Orange::Offsets::Slot::TextPacket_Vtable);
        if (!HandlerOffset || !VtableOffset)
        {
            return false;
        }
        void* Handler = *reinterpret_cast<void**>(reinterpret_cast<Byte*>(LocalPlayer()) + Orange::Offsets::Load(Orange::Offsets::Slot::LocalPlayer_sendPacketVtableOffset));
        if (!Handler)
        {
            return false;
        }
        using SendFn = void (*)(void*);
        SendFn Send = reinterpret_cast<SendFn>(Orange::Core::BaseAddress + static_cast<Address>(HandlerOffset));
        Send(Handler);
        (void)TextValue;
        return true;
    }
}
