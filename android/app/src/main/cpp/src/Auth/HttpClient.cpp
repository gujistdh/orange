#include "orange/Auth/HttpClient.h"

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <arpa/inet.h>

namespace Orange::Auth
{
    bool ParseUrl(const std::string& Url, UrlParts& Out)
    {
        std::string Rest = Url;
        if (Rest.compare(0, 7, "http://") == 0)
        {
            Out.Secure = false;
            Out.Port = 80;
            Rest = Rest.substr(7);
        }
        else if (Rest.compare(0, 8, "https://") == 0)
        {
            Out.Secure = true;
            Out.Port = 443;
            Rest = Rest.substr(8);
        }
        else
        {
            return false;
        }

        std::size_t Slash = Rest.find('/');
        std::string Authority = Slash == std::string::npos ? Rest : Rest.substr(0, Slash);
        Out.Path = Slash == std::string::npos ? "/" : Rest.substr(Slash);

        std::size_t Colon = Authority.find(':');
        if (Colon == std::string::npos)
        {
            Out.Host = Authority;
        }
        else
        {
            Out.Host = Authority.substr(0, Colon);
            Out.Port = static_cast<uint16_t>(std::atoi(Authority.c_str() + Colon + 1));
        }
        return !Out.Host.empty();
    }

    namespace
    {
        bool Connect(const std::string& Host, uint16_t Port, int TimeoutSeconds, int& OutSocket)
        {
            struct addrinfo Hints{};
            Hints.ai_family = AF_UNSPEC;
            Hints.ai_socktype = SOCK_STREAM;
            char PortText[16];
            std::snprintf(PortText, sizeof(PortText), "%u", static_cast<unsigned int>(Port));

            struct addrinfo* Result = nullptr;
            if (getaddrinfo(Host.c_str(), PortText, &Hints, &Result) != 0 || !Result)
            {
                return false;
            }

            int Socket = -1;
            for (struct addrinfo* Item = Result; Item; Item = Item->ai_next)
            {
                Socket = ::socket(Item->ai_family, Item->ai_socktype, Item->ai_protocol);
                if (Socket < 0)
                {
                    continue;
                }
                struct timeval Timeout{};
                Timeout.tv_sec = TimeoutSeconds;
                Timeout.tv_usec = 0;
                ::setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, &Timeout, sizeof(Timeout));
                ::setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, &Timeout, sizeof(Timeout));
                if (::connect(Socket, Item->ai_addr, Item->ai_addrlen) == 0)
                {
                    break;
                }
                ::close(Socket);
                Socket = -1;
            }
            freeaddrinfo(Result);
            OutSocket = Socket;
            return Socket >= 0;
        }
    }

    int ParseStatus(const std::string& Response)
    {
        std::size_t Space = Response.find(' ');
        if (Space == std::string::npos || Response.compare(0, 5, "HTTP/") != 0)
        {
            return 0;
        }
        return std::atoi(Response.c_str() + Space + 1);
    }

    std::string StripHeaders(const std::string& Response)
    {
        std::size_t Split = Response.find("\r\n\r\n");
        if (Split == std::string::npos)
        {
            return std::string();
        }
        return Response.substr(Split + 4);
    }

    HttpResult PostForm(const std::string& Url, const std::string& Body, int TimeoutSeconds)
    {
        HttpResult Result;
        UrlParts Parts;
        if (!ParseUrl(Url, Parts))
        {
            Result.Error = "bad url";
            return Result;
        }

        int Socket = -1;
        if (!Connect(Parts.Host, Parts.Port, TimeoutSeconds, Socket))
        {
            Result.Error = "connect failed";
            return Result;
        }

        std::string Request;
        Request += "POST ";
        Request += Parts.Path;
        Request += " HTTP/1.1\r\n";
        Request += "Host: ";
        Request += Parts.Host;
        if (Parts.Port != 80)
        {
            Request += ":";
            Request += std::to_string(Parts.Port);
        }
        Request += "\r\n";
        Request += "Content-Type: application/x-www-form-urlencoded\r\n";
        Request += "Content-Length: ";
        Request += std::to_string(Body.size());
        Request += "\r\n";
        Request += "Connection: close\r\n";
        Request += "Accept: */*\r\n\r\n";
        Request += Body;

        std::size_t Sent = 0;
        while (Sent < Request.size())
        {
            ssize_t Written = ::send(Socket, Request.data() + Sent, Request.size() - Sent, 0);
            if (Written <= 0)
            {
                ::close(Socket);
                Result.Error = "send failed";
                return Result;
            }
            Sent += static_cast<std::size_t>(Written);
        }

        std::string Response;
        char Buffer[4096];
        while (true)
        {
            ssize_t Read = ::recv(Socket, Buffer, sizeof(Buffer), 0);
            if (Read <= 0)
            {
                break;
            }
            Response.append(Buffer, static_cast<std::size_t>(Read));
        }
        ::close(Socket);

        Result.Status = ParseStatus(Response);
        Result.Body = StripHeaders(Response);
        Result.Ok = Result.Status == 200;
        return Result;
    }
}
