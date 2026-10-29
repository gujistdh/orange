#include "orange/Game/ItemIconPak.h"

#include <GLES3/gl3.h>
#include <zlib.h>
#include <fstream>

namespace Orange::Game
{
    namespace
    {
        const uint8_t kPngSignature[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

        std::vector<uint8_t> gData;
        std::vector<PakIcon> gIcons;
        std::string gSource;
        bool gLoaded = false;

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

    bool ItemIconPak::Load(const std::string& Path)
    {
        Release();
        std::ifstream Stream(Path, std::ios::binary);
        if (!Stream)
        {
            return false;
        }
        gData.assign(std::istreambuf_iterator<char>(Stream), std::istreambuf_iterator<char>());
        if (gData.size() < 8)
        {
            return false;
        }
        std::size_t Cursor = 0;
        while (Cursor + 33 <= gData.size())
        {
            bool Match = true;
            for (int Index = 0; Index < 8; ++Index)
            {
                if (gData[Cursor + static_cast<std::size_t>(Index)] != kPngSignature[Index])
                {
                    Match = false;
                    break;
                }
            }
            if (!Match)
            {
                ++Cursor;
                continue;
            }
            PakIcon Icon;
            Icon.Offset = static_cast<int>(Cursor);
            Icon.Width = static_cast<int>(ReadBe32(&gData[Cursor + 16]));
            Icon.Height = static_cast<int>(ReadBe32(&gData[Cursor + 20]));
            std::size_t Scan = Cursor + 8;
            std::size_t End = Cursor;
            while (Scan + 12 <= gData.size())
            {
                uint32_t Length = ReadBe32(&gData[Scan]);
                const uint8_t* Type = &gData[Scan + 4];
                bool IsEnd = Type[0] == 'I' && Type[1] == 'E' && Type[2] == 'N' && Type[3] == 'D';
                std::size_t Next = Scan + 12 + Length;
                if (IsEnd)
                {
                    End = Next;
                    break;
                }
                if (Next <= Scan)
                {
                    break;
                }
                Scan = Next;
            }
            if (End <= Cursor)
            {
                break;
            }
            Icon.Length = static_cast<int>(End - Cursor);
            gIcons.push_back(Icon);
            Cursor = End;
        }
        gSource = Path;
        gLoaded = !gIcons.empty();
        return gLoaded;
    }

    bool ItemIconPak::IsLoaded()
    {
        return gLoaded;
    }

    int ItemIconPak::Count()
    {
        return static_cast<int>(gIcons.size());
    }

    const PakIcon* ItemIconPak::At(int Index)
    {
        if (Index < 0 || static_cast<std::size_t>(Index) >= gIcons.size())
        {
            return nullptr;
        }
        return &gIcons[static_cast<std::size_t>(Index)];
    }

    const std::string& ItemIconPak::SourcePath()
    {
        return gSource;
    }

    bool ItemIconPak::Decode(int Index, std::vector<uint8_t>& OutRgba)
    {
        const PakIcon* Icon = At(Index);
        if (!Icon || !gLoaded)
        {
            return false;
        }
        const uint8_t* Base = &gData[static_cast<std::size_t>(Icon->Offset)];
        std::size_t Length = static_cast<std::size_t>(Icon->Length);
        std::size_t Cursor = 8;
        int Width = 0;
        int Height = 0;
        int BitDepth = 0;
        int ColorType = 0;
        int Interlace = 0;
        std::vector<uint8_t> Compressed;
        while (Cursor + 12 <= Length)
        {
            uint32_t ChunkLength = ReadBe32(&Base[Cursor]);
            const uint8_t* Type = &Base[Cursor + 4];
            const uint8_t* Payload = &Base[Cursor + 8];
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

    unsigned int ItemIconPak::EnsureTexture(int Index)
    {
        PakIcon* Icon = nullptr;
        if (Index >= 0 && static_cast<std::size_t>(Index) < gIcons.size())
        {
            Icon = &gIcons[static_cast<std::size_t>(Index)];
        }
        if (!Icon)
        {
            return 0;
        }
        if (Icon->Texture)
        {
            return Icon->Texture;
        }
        std::vector<uint8_t> Rgba;
        if (!Decode(Index, Rgba))
        {
            return 0;
        }
        GLuint Texture = 0;
        glGenTextures(1, &Texture);
        if (!Texture)
        {
            return 0;
        }
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Icon->Width, Icon->Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Rgba.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        Icon->Texture = Texture;
        return Texture;
    }

    bool ItemIconPak::IsStrip(int Index)
    {
        const PakIcon* Icon = At(Index);
        if (!Icon)
        {
            return false;
        }
        return Icon->Height > Icon->Width && Icon->Width > 0 && (Icon->Height % Icon->Width) == 0;
    }

    int ItemIconPak::FrameCount(int Index)
    {
        const PakIcon* Icon = At(Index);
        if (!Icon || Icon->Width <= 0 || Icon->Height <= 0)
        {
            return 0;
        }
        int Frames = Icon->Height / Icon->Width;
        return Frames < 1 ? 1 : Frames;
    }

    bool ItemIconPak::DecodeFrame(int Index, int Frame, std::vector<uint8_t>& OutRgba)
    {
        const PakIcon* Icon = At(Index);
        if (!Icon)
        {
            return false;
        }
        std::vector<uint8_t> Full;
        if (!Decode(Index, Full))
        {
            return false;
        }
        int Frames = FrameCount(Index);
        if (Frame < 0 || Frame >= Frames)
        {
            return false;
        }
        int Square = Icon->Width;
        if (!IsStrip(Index))
        {
            OutRgba = Full;
            return true;
        }
        std::size_t Stride = static_cast<std::size_t>(Icon->Width) * 4;
        std::size_t Offset = static_cast<std::size_t>(Frame) * static_cast<std::size_t>(Square) * Stride;
        if (Offset + static_cast<std::size_t>(Square) * Stride > Full.size())
        {
            return false;
        }
        OutRgba.assign(Full.begin() + static_cast<std::ptrdiff_t>(Offset),
                       Full.begin() + static_cast<std::ptrdiff_t>(Offset + static_cast<std::size_t>(Square) * Stride));
        return true;
    }

    unsigned int ItemIconPak::EnsureFrameTexture(int Index, int Frame)
    {
        const PakIcon* Icon = At(Index);
        if (!Icon)
        {
            return 0;
        }
        if (!IsStrip(Index))
        {
            return EnsureTexture(Index);
        }
        std::vector<uint8_t> Rgba;
        if (!DecodeFrame(Index, Frame, Rgba))
        {
            return 0;
        }
        int Square = Icon->Width;
        GLuint Texture = 0;
        glGenTextures(1, &Texture);
        if (!Texture)
        {
            return 0;
        }
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Square, Square, 0, GL_RGBA, GL_UNSIGNED_BYTE, Rgba.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        return Texture;
    }

    void ItemIconPak::Release()
    {
        for (PakIcon& Icon : gIcons)
        {
            if (Icon.Texture)
            {
                GLuint Handle = Icon.Texture;
                glDeleteTextures(1, &Handle);
                Icon.Texture = 0;
            }
        }
        gIcons.clear();
        gData.clear();
        gLoaded = false;
        gSource.clear();
    }
}
