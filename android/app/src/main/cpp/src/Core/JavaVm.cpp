#include "orange/Core/JavaVm.h"

namespace Orange::Core
{
    JavaVM* Vm = nullptr;

    JNIEnv* GetEnv()
    {
        JNIEnv* Env = nullptr;
        if (Vm && Vm->GetEnv(reinterpret_cast<void**>(&Env), JNI_VERSION_1_6) == JNI_OK)
        {
            return Env;
        }
        return nullptr;
    }

    int GetEnvVersioned(JNIEnv** Out, int Version)
    {
        if (!Vm)
        {
            return JNI_EDETACHED;
        }
        return Vm->GetEnv(reinterpret_cast<void**>(Out), Version);
    }

    bool AttachCurrentThread(JNIEnv** Out)
    {
        if (!Vm)
        {
            return false;
        }
        return Vm->AttachCurrentThread(Out, nullptr) == JNI_OK;
    }

    void DetachCurrentThread()
    {
        if (Vm)
        {
            Vm->DetachCurrentThread();
        }
    }
}
