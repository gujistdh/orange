#pragma once

#include "orange/Common.h"

namespace Orange::Config
{
    const char* ConfigDir();

    std::vector<std::string> ListConfigs();
    bool LoadConfig(const std::string& Name);
    bool SaveConfig(const std::string& Name);

    std::string Serialize();
    bool Deserialize(const std::string& Text);

    const std::string& CurrentName();
    void SetCurrentName(const std::string& Value);
}
