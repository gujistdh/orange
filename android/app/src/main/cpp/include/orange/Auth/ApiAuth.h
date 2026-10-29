#pragma once

#include "orange/Common.h"

namespace Orange::Auth
{
    enum class AuthState
    {
        Idle,
        Verifying,
        Verified,
        Failed,
        Limited,
    };

    struct AuthResponse
    {
        int Code = 0;
        std::string Message;
        std::string Token;
        std::string Blob;
        std::string Data;
        long long Expire = 0;
        long long CustomExpire = 0;
        bool Ok() const { return Code == 0; }
    };

    namespace ApiAuth
    {
        void Startup();
        void ProcessTick();
        void Shutdown();

        int Login(const std::string& Card);
        int LoginAccount(const std::string& Card, const std::string& Password);
        int Register(const std::string& Card, const std::string& Password, const std::string& Kami);
        void Refresh();
        void Notice();

        bool IsVerified();
        bool CanShowGui();
        void EnterLimited();
        AuthState State();

        std::string GetSavedCard();
        std::string GetToken();
        long long ExpireTime();
        int FailStreak();

        void OnKamiSubmit(const std::string& Card);
        void OnKamiCancel();
        void OnAuthSuccess();
        void OnAuthFail(int Code, const std::string& Message);
        void ShowUserDialog();
        void NotifyResult(bool Ok, const std::string& Message);
    }
}
