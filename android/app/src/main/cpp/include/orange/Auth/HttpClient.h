#pragma once

#include "orange/Common.h"

namespace Orange::Auth
{
    struct HttpResult
    {
        bool Ok = false;
        int Status = 0;
        std::string Body;
        std::string Error;
    };

    struct UrlParts
    {
        bool Secure = false;
        std::string Host;
        std::string Path;
        uint16_t Port = 80;
    };

    bool ParseUrl(const std::string& Url, UrlParts& Out);
    HttpResult PostForm(const std::string& Url, const std::string& Body, int TimeoutSeconds);
    std::string StripHeaders(const std::string& Response);
    int ParseStatus(const std::string& Response);
}
