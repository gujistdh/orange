#include "orange/Game/IconAnimation.h"
#include "orange/Game/ItemIconPak.h"

namespace Orange::Game
{
    namespace
    {
        float gElapsed = 0.0f;
        float gGlobalSpeed = 1.0f;
        std::map<int, unsigned int> gFrameTextures;

        int FrameCountOf(int IconIndex)
        {
            int Frames = ItemIconPak::FrameCount(IconIndex);
            return Frames < 1 ? 1 : Frames;
        }
    }

    void IconAnimation::Reset()
    {
        gElapsed = 0.0f;
        gGlobalSpeed = 1.0f;
        gFrameTextures.clear();
    }

    void IconAnimation::Tick(float DeltaSeconds)
    {
        gElapsed += DeltaSeconds * gGlobalSpeed;
        if (gElapsed > 3600.0f)
        {
            gElapsed -= 3600.0f;
        }
    }

    int IconAnimation::FrameFor(int IconIndex)
    {
        return FrameForSpeed(IconIndex, 8.0f);
    }

    int IconAnimation::FrameForSpeed(int IconIndex, float FramesPerSecond)
    {
        int Frames = FrameCountOf(IconIndex);
        if (Frames <= 1 || FramesPerSecond <= 0.0f)
        {
            return 0;
        }
        float Position = gElapsed * FramesPerSecond;
        int Frame = static_cast<int>(Position);
        Frame %= Frames;
        if (Frame < 0)
        {
            Frame += Frames;
        }
        return Frame;
    }

    unsigned int IconAnimation::TextureFor(int IconIndex)
    {
        int Frames = FrameCountOf(IconIndex);
        if (Frames <= 1)
        {
            return ItemIconPak::EnsureTexture(IconIndex);
        }
        int Frame = FrameFor(IconIndex);
        int Key = IconIndex * 1024 + Frame;
        auto It = gFrameTextures.find(Key);
        if (It != gFrameTextures.end())
        {
            return It->second;
        }
        unsigned int Texture = ItemIconPak::EnsureFrameTexture(IconIndex, Frame);
        gFrameTextures[Key] = Texture;
        return Texture;
    }

    float IconAnimation::Elapsed()
    {
        return gElapsed;
    }

    void IconAnimation::SetGlobalSpeed(float Value)
    {
        gGlobalSpeed = Value < 0.0f ? 0.0f : Value;
    }

    float IconAnimation::GlobalSpeed()
    {
        return gGlobalSpeed;
    }
}
