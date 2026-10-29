#include "orange/Core/Toast.h"
#include "orange/Core/JavaVm.h"

namespace Orange::Core
{
    namespace
    {
        jclass ToastClass = nullptr;
        jmethodID MakeTextMethod = nullptr;
        jmethodID ShowMethod = nullptr;
    }

    void Toast::CacheClass(JNIEnv* Env)
    {
        if (!Env || ToastClass)
        {
            return;
        }
        jclass Local = Env->FindClass("android/widget/Toast");
        if (!Local)
        {
            Env->ExceptionClear();
            return;
        }
        ToastClass = static_cast<jclass>(Env->NewGlobalRef(Local));
        Env->DeleteLocalRef(Local);
        if (!ToastClass)
        {
            return;
        }
        MakeTextMethod = Env->GetStaticMethodID(ToastClass, "makeText",
                                                "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
        if (!MakeTextMethod)
        {
            Env->ExceptionClear();
            return;
        }
        jclass ToastLocal = Env->FindClass("android/widget/Toast");
        ShowMethod = Env->GetMethodID(ToastLocal, "show", "()V");
        Env->DeleteLocalRef(ToastLocal);
        if (!ShowMethod)
        {
            Env->ExceptionClear();
        }
    }

    void Toast::Show(const std::string& Text)
    {
        if (Text.empty())
        {
            return;
        }
        JNIEnv* Env = Orange::Core::GetEnv();
        if (!Env || !ToastClass || !MakeTextMethod || !ShowMethod)
        {
            return;
        }
        jobject Activity = nullptr;
        jstring Message = Env->NewStringUTF(Text.c_str());
        if (!Message)
        {
            return;
        }
        jobject Instance = Env->CallStaticObjectMethod(ToastClass, MakeTextMethod, Activity, Message, 0);
        if (Instance)
        {
            Env->CallVoidMethod(Instance, ShowMethod);
            Env->DeleteLocalRef(Instance);
        }
        Env->DeleteLocalRef(Message);
        if (Env->ExceptionCheck())
        {
            Env->ExceptionClear();
        }
    }

    void Toast::ShowOnMain(const std::string& Text)
    {
        Show(Text);
    }
}
