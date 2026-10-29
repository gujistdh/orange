#pragma once

#include "orange/Common.h"

namespace Orange::Core
{
    class Toast
    {
    public:
        static void CacheClass(JNIEnv* Env);
        static void Show(const std::string& Text);
        static void ShowOnMain(const std::string& Text);
    };
}
