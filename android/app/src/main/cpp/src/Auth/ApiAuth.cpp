#include "orange/Auth/ApiAuth.h"
#include "orange/Auth/Crypto.h"
#include "orange/Game/Offsets.h"
#include "orange/Auth/HttpClient.h"
#include "orange/Core/Toast.h"
#include "orange/Gui/Gui.h"
#include "orange/Gui/JsonValue.h"
#include "orange/Modules/Script/ScriptModules.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <atomic>
#include <fstream>
#include <mutex>

static const char* ORANGE_DIR = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange";
static const char* CARD_PATH = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/kami.txt";
static const char* SESS_PATH = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/session.txt";
static const char* HWID_PATH = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/hwid.txt";
static const char* ACC_PATH = "/storage/emulated/0/Android/data/com.netease.x19/files/Orange/Account.txt";

static const char* GATEWAY_URL = "http://47.242.221.255/api.php";
static const char* CLIENT_KEY = "1f4e8d804a24b71972933476ce00967c6a42aa41fd169d91";

static const char* SUCCESS_MSG = "Orange - API - 对接成功 开始你的游戏之旅 - 2.0.6";
static const char* FAIL_MSG = "Orange - API - 对接失败 请重新验证 - 2.0.6";
static const char* EMPTY_MSG = "Orange - API - 对接失败 请检查卡密 - 2.0.6";
static const char* USED_MSG = "Orange - API - 对接失败 卡密已被使用 - 2.0.6";
static const char* EXPIRED_MSG = "Orange - API - 对接失败 卡密到期 - 2.0.6";
static const char* BANNED_MSG = "Orange - API - 对接失败 卡密已被禁用 - 2.0.6";
static const char* LOGGED_MSG = "Orange - API - 你已经登录过一次 - 2.0.6";
static const char* NET_MSG = "Orange - API - 服务器连接失败 请检查网络后重试 - 2.0.6";
static const char* KICK_MSG = "Orange - API - 授权已失效 请重新输入卡密 - 2.0.6";
static const char* BUSY_MSG = "Orange - API - 请求过于频繁 请稍后再试 - 2.0.6";
static const char* ERRCODE_MSG = "Orange - API - 对接失败 错误码 ";
static const char* HWFAIL_MSG = "Orange - API - 对接失败 设备码获取失败 - 2.0.6";
static const char* NOOFF_MSG = "Orange - API - 对接失败 服务端未配置偏移 - 2.0.6";
static const char* OFFLOAD_MSG = "Orange - API - 对接失败 偏移加载失败 - 2.0.6";
static const char* INVALID_MSG = "卡密无效";
static const char* REGOK_MSG = "注册成功 请登录";
static const char* LOGINING_MSG = "登录中...";

static const long long HEARTBEAT_SEC = 900;
static const long long NOTICE_SEC = 30;
static const int MAX_FAIL_STREAK = 3;

namespace Orange::Auth::ApiAuth
{
    static std::mutex gMutex;
    static std::string gCard;
    static std::string gToken;
    static long long gExp = 0;

    static std::atomic<bool> gVerified{false};
    static std::atomic<bool> gLoginBusy{false};
    static std::atomic<bool> gRefreshBusy{false};

    static std::atomic<bool> gNoticeBusy{false};
    static std::atomic<int> gFailStreak{0};
    static std::atomic<int> gState{static_cast<int>(AuthState::Idle)};
    static long long gLastHeartbeat = 0;
    static long long gLastNotice = 0;
    static bool gStarted = false;

    static std::string ReadFile(const char* Path)
    {
        std::ifstream Stream(Path, std::ios::binary);
        if (!Stream)
        {
            return std::string();
        }
        std::string Content((std::istreambuf_iterator<char>(Stream)), std::istreambuf_iterator<char>());
        while (!Content.empty() && (Content.back() == '\n' || Content.back() == '\r' || Content.back() == ' '))
        {
            Content.pop_back();
        }
        return Content;
    }

    static void WriteFile(const char* Path, const std::string& Content)
    {
        std::ofstream Stream(Path, std::ios::binary | std::ios::trunc);
        if (Stream)
        {
            Stream << Content;
        }
    }

    static long long NowSeconds()
    {
        return static_cast<long long>(std::time(nullptr));
    }

    static const char* MessageForCode(int Code)
    {
        switch (Code)
        {
        case -1: return NET_MSG;
        case -2: return FAIL_MSG;
        case -6: return HWFAIL_MSG;
        case -7: return OFFLOAD_MSG;
        case 500: return NOOFF_MSG;
        case 400: return EMPTY_MSG;
        case 148: return EMPTY_MSG;
        case 149: return EMPTY_MSG;
        case 150: return USED_MSG;
        case 152: return EXPIRED_MSG;
        case 153: return BANNED_MSG;
        case 429: return BUSY_MSG;
        case 401: return FAIL_MSG;
        case 403: return FAIL_MSG;
        case 409: return FAIL_MSG;
        default: return nullptr;
        }
    }

    static AuthResponse Send(const std::string& Query)
    {
        AuthResponse Out;
        HttpResult Result = PostForm(GATEWAY_URL, Query, 8);
        if (!Result.Ok)
        {
            Out.Code = -1;
            Out.Message = Result.Error;
            return Out;
        }
        Gui::JsonValue Root;
        if (!Gui::JsonParse(Result.Body, Root))
        {
            Out.Code = -2;
            Out.Message = Result.Body.substr(0, 120);
            return Out;
        }
        if (const Gui::JsonValue* Code = Root.Find("code"))
        {
            Out.Code = static_cast<int>(Code->NumberOr(-1.0));
        }
        if (const Gui::JsonValue* Msg = Root.Find("msg"))
        {
            Out.Message = Msg->StringOr(std::string());
        }
        if (const Gui::JsonValue* Token = Root.Find("token"))
        {
            Out.Token = Token->StringOr(std::string());
        }
        if (const Gui::JsonValue* Data = Root.Find("data"))
        {
            Out.Data = Data->Serialize();
        }
        if (const Gui::JsonValue* Blob = Root.Find("blob"))
        {
            Out.Blob = Blob->StringOr(std::string());
        }
        if (const Gui::JsonValue* Exp = Root.Find("exp"))
        {
            Out.Expire = static_cast<long long>(Exp->NumberOr(0.0));
        }
        if (const Gui::JsonValue* Cex = Root.Find("cex"))
        {
            Out.CustomExpire = static_cast<long long>(Cex->NumberOr(0.0));
        }
        if (const Gui::JsonValue* Wy = Root.Find("wy"))
        {
            int Invalid = static_cast<int>(Wy->NumberOr(0.0));
            if (Invalid >= 1 && Out.Code == 0)
            {
                Out.Code = Invalid;
            }
        }
        if (const Gui::JsonValue* Ec = Root.Find("ec"))
        {
            int ErrorCode = static_cast<int>(Ec->NumberOr(0.0));
            if (ErrorCode >= 1 && Out.Code == 0)
            {
                Out.Code = ErrorCode;
            }
        }
        return Out;
    }

    void NotifyResult(bool Ok, const std::string& Message)
    {
        std::string Text = Ok ? SUCCESS_MSG : Message;
        if (Text.empty())
        {
            return;
        }
        Orange::Core::Toast::ShowOnMain(Text);
        Orange::Modules::Script::Message::SendMessage(Text);
    }

    void OnAuthFail(int Code, const std::string& Message)
    {
        const char* Base = MessageForCode(Code);
        std::string Text;
        if (Base)
        {
            Text = Base;
        }
        else if (Code > 0 || Code <= -2)
        {
            Text = ERRCODE_MSG;
            Text += std::to_string(Code);
            Text += " - 2.0.6";
        }
        else
        {
            Text = FAIL_MSG;
        }
        if (Code == -2 && !Message.empty())
        {
            Text += " ";
            Text += Message;
        }
        if (Code == -1)
        {
            int Streak = gFailStreak.fetch_add(1) + 1;
            if (Streak >= MAX_FAIL_STREAK)
            {
                gFailStreak.store(0);
                gVerified.store(false);
                gState.store(static_cast<int>(AuthState::Failed));
                Text = KICK_MSG;
            }
        }
        else
        {
            gFailStreak.store(0);
        }
        Orange::Core::Toast::ShowOnMain(Text);
        Orange::Modules::Script::Message::SendMessage(Text);
    }

    void OnAuthSuccess()
    {
        gFailStreak.store(0);
        gVerified.store(true);
        gState.store(static_cast<int>(AuthState::Verified));
        gExp = NowSeconds();
        Gui::AuthClose();
        Orange::Modules::Script::Chatroom::OnAuthSuccess();
        NotifyResult(true, std::string());
    }

    void Startup()
    {
        if (gStarted)
        {
            return;
        }
        gStarted = true;
        (void)ORANGE_DIR;
        (void)HWID_PATH;
        (void)ACC_PATH;
        gCard = ReadFile(CARD_PATH);
        std::string Session = ReadFile(SESS_PATH);
        std::size_t Split = Session.find('\n');
        if (Split != std::string::npos)
        {
            gCard = Session.substr(0, Split);
            gToken = Session.substr(Split + 1);
        }
        if (!gCard.empty())
        {
            if (!gToken.empty())
            {
                (void)LOGGED_MSG;
            }
            gState.store(static_cast<int>(AuthState::Verifying));
        }
    }

    void Shutdown()
    {
        gStarted = false;
        gVerified.store(false);
        gState.store(static_cast<int>(AuthState::Idle));
    }

    void ProcessTick()
    {
        if (!gStarted)
        {
            return;
        }
        long long Now = NowSeconds();
        if (gState.load() == static_cast<int>(AuthState::Verifying))
        {
            if (gLastHeartbeat == 0)
            {
                gLastHeartbeat = Now;
            }
            if (Now - gLastHeartbeat >= HEARTBEAT_SEC)
            {
                gLastHeartbeat = Now;
                Refresh();
            }
            return;
        }
        if (!gVerified.load())
        {
            return;
        }
        if (Now - gLastHeartbeat >= HEARTBEAT_SEC)
        {
            gLastHeartbeat = Now;
            Refresh();
        }
        if (Now - gLastNotice >= NOTICE_SEC)
        {
            gLastNotice = Now;
            Notice();
        }
    }

    int Login(const std::string& Card)
    {
        if (Card.empty())
        {
            OnAuthFail(-2, std::string());
            return -2;
        }
        bool Expected = false;
        if (!gLoginBusy.compare_exchange_strong(Expected, true))
        {
            return -3;
        }
        std::string Hwid = BuildHwid();
        std::string Timestamp = NowTimestamp();
        std::string Nonce = BuildNonce();
        std::string Signature = BuildSignature("login", Card, Hwid, Timestamp, Nonce);
        (void)CLIENT_KEY;

        std::string Query;
        Query += "action=login&card=";
        Query += UrlEncode(Card);
        Query += "&hwid=";
        Query += UrlEncode(Hwid);
        Query += "&ts=";
        Query += Timestamp;
        Query += "&nonce=";
        Query += Nonce;
        Query += "&sig=";
        Query += Signature;

        gState.store(static_cast<int>(AuthState::Verifying));
        AuthResponse Response = Send(Query);
        gLoginBusy.store(false);
        if (!Response.Ok())
        {
            gState.store(static_cast<int>(AuthState::Failed));
            OnAuthFail(Response.Code, Response.Message);
            return Response.Code;
        }
        if (Response.Expire > 0 && Response.Expire <= NowSeconds() + 30)
        {
            gState.store(static_cast<int>(AuthState::Failed));
            OnAuthFail(-4, std::string());
            return -4;
        }
        if (!Response.Blob.empty())
        {
            std::string Plain;
            if (!DecodeBlob(Response.Blob, Response.Token, Plain))
            {
                gState.store(static_cast<int>(AuthState::Failed));
                OnAuthFail(-5, std::string());
                return -5;
            }
            Orange::Offsets::ApplyFromJson(Plain);
            if (!Orange::Offsets::IsReady())
            {
                gState.store(static_cast<int>(AuthState::Failed));
                OnAuthFail(-7, std::string());
                return -7;
            }
        }
        {
            std::lock_guard<std::mutex> Lock(gMutex);
            gCard = Card;
            gToken = Response.Token;
        }
        WriteFile(CARD_PATH, Card);
        std::string Session = Card;
        Session += "\n";
        Session += Response.Token;
        WriteFile(SESS_PATH, Session);
        OnAuthSuccess();
        return 0;
    }

    int LoginAccount(const std::string& Card, const std::string& Password)
    {
        if (Card.empty() || Password.empty())
        {
            OnAuthFail(-2, std::string());
            return -2;
        }
        std::string Hwid = BuildHwid();
        std::string Timestamp = NowTimestamp();
        std::string Nonce = BuildNonce();
        std::string Signature = BuildSignature("alogin", Card, Hwid, Timestamp, Nonce);

        std::string Query;
        Query += "action=alogin&card=";
        Query += UrlEncode(Card);
        Query += "&pwd=";
        Query += UrlEncode(Password);
        Query += "&hwid=";
        Query += UrlEncode(Hwid);
        Query += "&ts=";
        Query += Timestamp;
        Query += "&nonce=";
        Query += Nonce;
        Query += "&sig=";
        Query += Signature;

        AuthResponse Response = Send(Query);
        if (!Response.Ok())
        {
            OnAuthFail(Response.Code, Response.Message);
            return Response.Code;
        }
        {
            std::lock_guard<std::mutex> Lock(gMutex);
            gCard = Card;
            gToken = Response.Token;
        }
        OnAuthSuccess();
        return 0;
    }

    int Register(const std::string& Card, const std::string& Password, const std::string& Kami)
    {
        if (Card.empty() || Password.empty())
        {
            return -2;
        }
        std::string Hwid = BuildHwid();
        std::string Timestamp = NowTimestamp();
        std::string Nonce = BuildNonce();
        std::string Signature = BuildSignature("reg", Card, Hwid, Timestamp, Nonce);

        std::string Query;
        Query += "action=reg&card=";
        Query += UrlEncode(Card);
        Query += "&pwd=";
        Query += UrlEncode(Password);
        Query += "&kami=";
        Query += UrlEncode(Kami);
        Query += "&hwid=";
        Query += UrlEncode(Hwid);
        Query += "&ts=";
        Query += Timestamp;
        Query += "&nonce=";
        Query += Nonce;
        Query += "&sig=";
        Query += Signature;

        AuthResponse Response = Send(Query);
        if (Response.Ok())
        {
            Orange::Core::Toast::ShowOnMain(REGOK_MSG);
        }
        else
        {
            OnAuthFail(Response.Code, Response.Message);
        }
        return Response.Code;
    }

    void Refresh()
    {
        bool Expected = false;
        if (!gRefreshBusy.compare_exchange_strong(Expected, true))
        {
            return;
        }
        std::string Card;
        std::string Token;
        {
            std::lock_guard<std::mutex> Lock(gMutex);
            Card = gCard;
            Token = gToken;
        }
        if (Card.empty() || Token.empty())
        {
            gRefreshBusy.store(false);
            return;
        }
        std::string Hwid = BuildHwid();
        std::string Query;
        Query += "action=refresh&card=";
        Query += UrlEncode(Card);
        Query += "&hwid=";
        Query += UrlEncode(Hwid);
        Query += "&token=";
        Query += UrlEncode(Token);
        AuthResponse Response = Send(Query);
        gRefreshBusy.store(false);
        if (!Response.Ok())
        {
            OnAuthFail(Response.Code, Response.Message);
            return;
        }
        std::lock_guard<std::mutex> Lock(gMutex);
        gToken = Response.Token;
        gExp = NowSeconds() + HEARTBEAT_SEC;
    }

    void Notice()
    {
        bool Expected = false;
        if (!gNoticeBusy.compare_exchange_strong(Expected, true))
        {
            return;
        }
        std::string Token;
        {
            std::lock_guard<std::mutex> Lock(gMutex);
            Token = gToken;
        }
        if (Token.empty())
        {
            gNoticeBusy.store(false);
            return;
        }
        std::string Hwid = BuildHwid();
        std::string Query;
        Query += "action=notice&hwid=";
        Query += UrlEncode(Hwid);
        Query += "&token=";
        Query += UrlEncode(Token);
        AuthResponse Response = Send(Query);
        gNoticeBusy.store(false);
        if (Response.Ok() && !Response.Message.empty())
        {
            Orange::Modules::Script::Message::SendMessage(Response.Message);
        }
    }

    bool IsVerified()
    {
        return gVerified.load();
    }

    bool CanShowGui()
    {
        return gVerified.load() || gState.load() == static_cast<int>(AuthState::Limited);
    }

    void EnterLimited()
    {
        gState.store(static_cast<int>(AuthState::Limited));
    }

    AuthState State()
    {
        return static_cast<AuthState>(gState.load());
    }

    std::string GetSavedCard()
    {
        std::lock_guard<std::mutex> Lock(gMutex);
        if (!gCard.empty())
        {
            return gCard;
        }
        return ReadFile(CARD_PATH);
    }

    std::string GetToken()
    {
        std::lock_guard<std::mutex> Lock(gMutex);
        return gToken;
    }

    long long ExpireTime()
    {
        return gExp;
    }

    int FailStreak()
    {
        return gFailStreak.load();
    }

    void OnKamiSubmit(const std::string& Card)
    {
        if (Card.empty())
        {
            Orange::Core::Toast::ShowOnMain(INVALID_MSG);
            return;
        }
        Orange::Core::Toast::ShowOnMain(LOGINING_MSG);
        Login(Card);
    }

    void OnKamiCancel()
    {
        gState.store(static_cast<int>(AuthState::Idle));
        Gui::AuthClose();
    }

    void ShowUserDialog()
    {
        Gui::AuthClose();
    }
}
