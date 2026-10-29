#include "orange/Common.h"
#include "orange/Core/Main.h"
#include "orange/Core/JavaVm.h"
#include "orange/Core/Toast.h"
#include "orange/Gui/Gui.h"
#include "orange/Modules/Behavior.h"
#include "orange/Auth/ApiAuth.h"
#include "orange/Core/CrashGuard.h"

namespace
{
    jboolean NativeInitHooks(JNIEnv* Env, jclass Class, jbyteArray Data)
    {
        (void)Class;
        (void)Data;
        Orange::Core::InitHooks(Env);
        Orange::Core::CrashGuard::Install();
        Orange::Gui::Build();
        Orange::Modules::InstallAll();
        return JNI_TRUE;
    }

    void NativeKamiCancel(JNIEnv* Env, jclass Class)
    {
        (void)Env;
        (void)Class;
        Orange::Gui::AuthClose();
    }

    void NativeKamiStartup(JNIEnv* Env, jclass Class)
    {
        (void)Env;
        (void)Class;
        Orange::Auth::ApiAuth::Startup();
    }

    void NativeKamiSubmit(JNIEnv* Env, jclass Class, jstring Text)
    {
        (void)Class;
        if (!Text)
        {
            return;
        }
        const char* Raw = Env->GetStringUTFChars(Text, nullptr);
        if (Raw)
        {
            Orange::Gui::AuthRegisterOk(Raw);
            Env->ReleaseStringUTFChars(Text, Raw);
        }
    }

    void NativeSetDensity(JNIEnv* Env, jclass Class, jfloat Density)
    {
        (void)Env;
        (void)Class;
        (void)Density;
    }

    void NativeTextInputResult(JNIEnv* Env, jclass Class, jstring Text)
    {
        (void)Class;
        if (!Text)
        {
            return;
        }
        const char* Raw = Env->GetStringUTFChars(Text, nullptr);
        if (Raw)
        {
            Orange::Gui::OnTextInputResult(Raw);
            Env->ReleaseStringUTFChars(Text, Raw);
        }
    }

    jboolean NativeTouchEvent(JNIEnv* Env, jclass Class, jint Action, jfloat X, jfloat Y)
    {
        (void)Env;
        (void)Class;
        Orange::Modules::TickAll();
        Orange::Auth::ApiAuth::ProcessTick();
        return Orange::Gui::OnTouch(static_cast<int>(Action), X, Y) ? JNI_TRUE : JNI_FALSE;
    }

    void NativeTpEditInput(JNIEnv* Env, jclass Class, jint Index, jint Field, jstring Text)
    {
        (void)Class;
        (void)Index;
        (void)Field;
        if (!Text)
        {
            return;
        }
        const char* Raw = Env->GetStringUTFChars(Text, nullptr);
        if (Raw)
        {
            Env->ReleaseStringUTFChars(Text, Raw);
        }
    }

    void NativeUserNameCancel(JNIEnv* Env, jclass Class)
    {
        (void)Env;
        (void)Class;
    }

    void NativeUserNameResult(JNIEnv* Env, jclass Class, jstring Text)
    {
        (void)Env;
        (void)Class;
        (void)Text;
    }

    const JNINativeMethod kNativeMethods[] = {
        { "nativeInitHooks", "([B)Z", reinterpret_cast<void*>(NativeInitHooks) },
        { "nativeKamiCancel", "()V", reinterpret_cast<void*>(NativeKamiCancel) },
        { "nativeKamiStartup", "()V", reinterpret_cast<void*>(NativeKamiStartup) },
        { "nativeKamiSubmit", "(Ljava/lang/String;)V", reinterpret_cast<void*>(NativeKamiSubmit) },
        { "nativeSetDensity", "(F)V", reinterpret_cast<void*>(NativeSetDensity) },
        { "nativeTextInputResult", "(Ljava/lang/String;)V", reinterpret_cast<void*>(NativeTextInputResult) },
        { "nativeTouchEvent", "(IFF)Z", reinterpret_cast<void*>(NativeTouchEvent) },
        { "nativeTpEditInput", "(IILjava/lang/String;)V", reinterpret_cast<void*>(NativeTpEditInput) },
        { "nativeUserNameCancel", "()V", reinterpret_cast<void*>(NativeUserNameCancel) },
        { "nativeUserNameResult", "(Ljava/lang/String;)V", reinterpret_cast<void*>(NativeUserNameResult) },
    };
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* Vm, void* Reserved)
{
    (void)Reserved;
    Orange::Core::Vm = Vm;

    JNIEnv* Env = nullptr;
    if (Orange::Core::GetEnvVersioned(&Env, JNI_VERSION_1_6) != JNI_OK || !Env)
    {
        return -1;
    }

    jclass LoadingClass = Env->FindClass("com/Orange/Loading");
    if (LoadingClass)
    {
        Env->RegisterNatives(LoadingClass, kNativeMethods,
                             static_cast<jint>(sizeof(kNativeMethods) / sizeof(kNativeMethods[0])));
        Env->DeleteLocalRef(LoadingClass);
    }

    if (Env->ExceptionCheck())
    {
        Env->ExceptionClear();
    }

    Orange::Core::Toast::CacheClass(Env);

    Orange::Core::BaseAddress = Orange::Core::FindLibBaseAddress("libminecraftpe.so");
    Orange::Core::BssAddress = Orange::Core::FindBssBaseAddress("libminecraftpe.so");

    return JNI_VERSION_1_6;
}
