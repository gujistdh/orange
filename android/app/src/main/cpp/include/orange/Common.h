#pragma once

#include <jni.h>

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <memory>
#include <chrono>
#include <algorithm>
#include <mutex>
#include <deque>
#include <set>
#include <utility>

#include "imgui.h"

namespace Orange::Game
{
    class Actor;
    class Player;
    class LocalPlayer;
    class Level;
    class LevelData;
    class Block;
    class BlockSource;
    struct BlockPos;
    class Item;
    class ItemStack;
    class ItemStackBase;
    class CompoundTag;
    class ListTag;
    class NbtNode;
    struct Vec3;
    class GuiData;
    class GameMode;
    class Packet;
    class TextPacket;
    class PlayerInventory;
    class Supplies;
    class MobEffectInstance;
    class RenderContext;
    class MatrixStack;
    class ActorDefinitionIdentifier;
    class JsonValue;
    class MC3100;
    class Offsets;
    class Main;
    class Toast;
    class EglEarly;
}

namespace Orange
{
    using Byte = std::uint8_t;
    using UInt = std::uint32_t;
    using Int = std::int32_t;
    using Int64 = std::int64_t;
    using UInt64 = std::uint64_t;
    using Address = std::uintptr_t;

    template <typename T>
    T* At(void* Base, Address Offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<Byte*>(Base) + Offset);
    }

    template <typename T>
    T& Ref(void* Base, Address Offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<Byte*>(Base) + Offset);
    }

    template <typename T>
    T Read(void* Base, Address Offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<Byte*>(Base) + Offset);
    }

    template <typename Fn>
    Fn FunctionAt(void* Base, Address Offset)
    {
        return reinterpret_cast<Fn>(reinterpret_cast<Byte*>(Base) + Offset);
    }
}
