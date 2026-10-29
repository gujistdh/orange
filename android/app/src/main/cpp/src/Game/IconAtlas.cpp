#include "orange/Game/IconAtlas.h"

#include <GLES3/gl3.h>
#include <zlib.h>
#include <fstream>

namespace Orange::Game
{
    const char* const IconAtlas::EnchantDir = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/textures/enchant";
    const char* const IconAtlas::TextureDir = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/textures";

    namespace
    {
        std::mutex gAtlasMutex;
        std::map<std::string, unsigned int> gTextures;

        uint32_t ReadBe32(const uint8_t* Data)
        {
            return (static_cast<uint32_t>(Data[0]) << 24) |
                   (static_cast<uint32_t>(Data[1]) << 16) |
                   (static_cast<uint32_t>(Data[2]) << 8) |
                   static_cast<uint32_t>(Data[3]);
        }

        int PaethPredictor(int A, int B, int C)
        {
            int P = A + B - C;
            int PA = std::abs(P - A);
            int PB = std::abs(P - B);
            int PC = std::abs(P - C);
            if (PA <= PB && PA <= PC)
            {
                return A;
            }
            if (PB <= PC)
            {
                return B;
            }
            return C;
        }
    }

    bool IconAtlas::DecodeFile(const std::string& Path, int& OutWidth, int& OutHeight, std::vector<uint8_t>& OutRgba)
    {
        std::ifstream Stream(Path, std::ios::binary);
        if (!Stream)
        {
            return false;
        }
        std::vector<uint8_t> Data((std::istreambuf_iterator<char>(Stream)), std::istreambuf_iterator<char>());
        if (Data.size() < 33)
        {
            return false;
        }
        static const uint8_t kSignature[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
        for (int Index = 0; Index < 8; ++Index)
        {
            if (Data[static_cast<std::size_t>(Index)] != kSignature[Index])
            {
                return false;
            }
        }
        std::size_t Cursor = 8;
        int Width = 0;
        int Height = 0;
        int BitDepth = 0;
        int ColorType = 0;
        int Interlace = 0;
        std::vector<uint8_t> Compressed;
        while (Cursor + 12 <= Data.size())
        {
            uint32_t ChunkLength = ReadBe32(&Data[Cursor]);
            const uint8_t* Type = &Data[Cursor + 4];
            const uint8_t* Payload = &Data[Cursor + 8];
            if (Type[0] == 'I' && Type[1] == 'H' && Type[2] == 'D' && Type[3] == 'R')
            {
                if (ChunkLength >= 13)
                {
                    Width = static_cast<int>(ReadBe32(Payload));
                    Height = static_cast<int>(ReadBe32(Payload + 4));
                    BitDepth = Payload[8];
                    ColorType = Payload[9];
                    Interlace = Payload[12];
                }
            }
            else if (Type[0] == 'I' && Type[1] == 'D' && Type[2] == 'A' && Type[3] == 'T')
            {
                Compressed.insert(Compressed.end(), Payload, Payload + ChunkLength);
            }
            else if (Type[0] == 'I' && Type[1] == 'E' && Type[2] == 'N' && Type[3] == 'D')
            {
                break;
            }
            Cursor += 12 + ChunkLength;
        }
        if (Width <= 0 || Height <= 0 || BitDepth != 8 || Interlace != 0)
        {
            return false;
        }
        int Channels = 0;
        if (ColorType == 6)
        {
            Channels = 4;
        }
        else if (ColorType == 2)
        {
            Channels = 3;
        }
        else if (ColorType == 4)
        {
            Channels = 2;
        }
        else if (ColorType == 0)
        {
            Channels = 1;
        }
        else
        {
            return false;
        }
        std::size_t Stride = static_cast<std::size_t>(Width) * static_cast<std::size_t>(Channels);
        std::size_t RawSize = (Stride + 1) * static_cast<std::size_t>(Height);
        std::vector<uint8_t> Raw(RawSize);
        uLongf DestLen = static_cast<uLongf>(RawSize);
        if (uncompress(Raw.data(), &DestLen, Compressed.data(), static_cast<uLong>(Compressed.size())) != Z_OK)
        {
            return false;
        }
        if (DestLen < RawSize)
        {
            return false;
        }
        std::vector<uint8_t> Pixels(Stride * static_cast<std::size_t>(Height));
        for (int Row = 0; Row < Height; ++Row)
        {
            const uint8_t* Source = &Raw[static_cast<std::size_t>(Row) * (Stride + 1)];
            uint8_t Filter = Source[0];
            const uint8_t* Input = Source + 1;
            uint8_t* Output = &Pixels[static_cast<std::size_t>(Row) * Stride];
            const uint8_t* Prior = Row > 0 ? &Pixels[static_cast<std::size_t>(Row - 1) * Stride] : nullptr;
            for (std::size_t Col = 0; Col < Stride; ++Col)
            {
                int Left = Col >= static_cast<std::size_t>(Channels) ? Output[Col - Channels] : 0;
                int Up = Prior ? Prior[Col] : 0;
                int UpLeft = (Prior && Col >= static_cast<std::size_t>(Channels)) ? Prior[Col - Channels] : 0;
                int Value = Input[Col];
                switch (Filter)
                {
                case 0:
                    break;
                case 1:
                    Value += Left;
                    break;
                case 2:
                    Value += Up;
                    break;
                case 3:
                    Value += (Left + Up) / 2;
                    break;
                case 4:
                    Value += PaethPredictor(Left, Up, UpLeft);
                    break;
                default:
                    return false;
                }
                Output[Col] = static_cast<uint8_t>(Value & 0xFF);
            }
        }
        OutWidth = Width;
        OutHeight = Height;
        OutRgba.assign(static_cast<std::size_t>(Width) * static_cast<std::size_t>(Height) * 4, 0);
        for (int Row = 0; Row < Height; ++Row)
        {
            const uint8_t* Source = &Pixels[static_cast<std::size_t>(Row) * Stride];
            uint8_t* Output = &OutRgba[static_cast<std::size_t>(Row) * static_cast<std::size_t>(Width) * 4];
            for (int Col = 0; Col < Width; ++Col)
            {
                const uint8_t* Pixel = Source + static_cast<std::size_t>(Col) * static_cast<std::size_t>(Channels);
                uint8_t* Target = Output + static_cast<std::size_t>(Col) * 4;
                if (Channels == 4)
                {
                    Target[0] = Pixel[0];
                    Target[1] = Pixel[1];
                    Target[2] = Pixel[2];
                    Target[3] = Pixel[3];
                }
                else if (Channels == 3)
                {
                    Target[0] = Pixel[0];
                    Target[1] = Pixel[1];
                    Target[2] = Pixel[2];
                    Target[3] = 255;
                }
                else if (Channels == 2)
                {
                    Target[0] = Pixel[0];
                    Target[1] = Pixel[0];
                    Target[2] = Pixel[0];
                    Target[3] = Pixel[1];
                }
                else
                {
                    Target[0] = Pixel[0];
                    Target[1] = Pixel[0];
                    Target[2] = Pixel[0];
                    Target[3] = 255;
                }
            }
        }
        return true;
    }

    unsigned int IconAtlas::LoadTexture(const std::string& Path)
    {
        std::lock_guard<std::mutex> Lock(gAtlasMutex);
        auto It = gTextures.find(Path);
        if (It != gTextures.end())
        {
            return It->second;
        }
        int Width = 0;
        int Height = 0;
        std::vector<uint8_t> Rgba;
        if (!DecodeFile(Path, Width, Height, Rgba))
        {
            gTextures[Path] = 0;
            return 0;
        }
        GLuint Texture = 0;
        glGenTextures(1, &Texture);
        if (!Texture)
        {
            gTextures[Path] = 0;
            return 0;
        }
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Rgba.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        gTextures[Path] = Texture;
        return Texture;
    }

    unsigned int IconAtlas::EnchantIcon(int EnchantId)
    {
        if (EnchantId <= 0 || EnchantId > 64)
        {
            return 0;
        }
        std::string Path = std::string(EnchantDir) + "/" + std::to_string(EnchantId) + ".png";
        return LoadTexture(Path);
    }

    unsigned int IconAtlas::TextureIcon()
    {
        std::string Path = std::string(TextureDir) + "/Orange.png";
        return LoadTexture(Path);
    }

    void IconAtlas::Init()
    {
        std::lock_guard<std::mutex> Lock(gAtlasMutex);
        gTextures.clear();
    }

    int IconAtlas::CachedCount()
    {
        std::lock_guard<std::mutex> Lock(gAtlasMutex);
        return static_cast<int>(gTextures.size());
    }

    void IconAtlas::Release()
    {
        std::lock_guard<std::mutex> Lock(gAtlasMutex);
        for (auto& Entry : gTextures)
        {
            if (Entry.second)
            {
                GLuint Handle = Entry.second;
                glDeleteTextures(1, &Handle);
            }
        }
        gTextures.clear();
    }
}
