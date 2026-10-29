#pragma once

#include "orange/Common.h"

namespace Orange::Core
{
    extern JavaVM* Vm;

    JNIEnv* GetEnv();
    int GetEnvVersioned(JNIEnv** Out, int Version);

    bool AttachCurrentThread(JNIEnv** Out);
    void DetachCurrentThread();
}
