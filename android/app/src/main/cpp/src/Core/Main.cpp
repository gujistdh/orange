#include "orange/Core/Main.h"

#include <dlfcn.h>
#include <unistd.h>
#include <link.h>
#include <fstream>

namespace Orange::Core
{
    Address BaseAddress = 0;
    Address BssAddress = 0;

    namespace
    {
        struct MapEntry
        {
            Address Start;
            Address End;
            char Permissions[8];
            Address Offset;
            std::string Path;
        };

        std::vector<MapEntry> ReadMaps()
        {
            std::vector<MapEntry> Entries;
            std::ifstream Maps("/proc/self/maps");
            std::string Line;
            while (std::getline(Maps, Line))
            {
                MapEntry Entry{};
                unsigned long Start = 0;
                unsigned long End = 0;
                unsigned long Offset = 0;
                char Perms[8] = { 0 };
                char PathBuffer[512] = { 0 };
                int Fields = std::sscanf(Line.c_str(), "%lx-%lx %7s %lx %*s %*s %511s",
                                         &Start, &End, Perms, &Offset, PathBuffer);
                if (Fields < 4)
                {
                    continue;
                }
                Entry.Start = Start;
                Entry.End = End;
                Entry.Offset = Offset;
                std::memcpy(Entry.Permissions, Perms, sizeof(Entry.Permissions));
                if (Fields >= 5)
                {
                    Entry.Path = PathBuffer;
                }
                Entries.push_back(Entry);
            }
            return Entries;
        }
    }

    Address FindLibBaseAddress(const std::string& Name)
    {
        const std::string Target = Name.empty() ? "libminecraftpe.so" : Name;
        std::vector<MapEntry> Entries = ReadMaps();
        for (const MapEntry& Entry : Entries)
        {
            if (Entry.Path.find(Target) == std::string::npos)
            {
                continue;
            }
            if (Entry.Offset != 0)
            {
                continue;
            }
            return Entry.Start;
        }
        return 0;
    }

    Address FindBssBaseAddress(const std::string& Name)
    {
        const std::string Target = Name.empty() ? "libminecraftpe.so" : Name;
        std::vector<MapEntry> Entries = ReadMaps();
        Address Result = 0;
        for (const MapEntry& Entry : Entries)
        {
            if (Entry.Path.find(Target) == std::string::npos)
            {
                continue;
            }
            Result = Entry.Start;
        }
        return Result;
    }

    int GetPid()
    {
        return static_cast<int>(getpid());
    }

    void InitHooks(JNIEnv* Env)
    {
        (void)Env;
        BaseAddress = FindLibBaseAddress("libminecraftpe.so");
        BssAddress = FindBssBaseAddress("libminecraftpe.so");
    }

    namespace Main
    {
        Address FindLibBaseAddress(const std::string& Name)
        {
            return Orange::Core::FindLibBaseAddress(Name);
        }

        Address FindBssBaseAddress(const std::string& Name)
        {
            return Orange::Core::FindBssBaseAddress(Name);
        }

        void InitHooks(JNIEnv* Env)
        {
            Orange::Core::InitHooks(Env);
        }
    }
}
