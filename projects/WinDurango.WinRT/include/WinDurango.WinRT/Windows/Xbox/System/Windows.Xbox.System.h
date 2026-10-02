#pragma once
#include "Windows.Xbox.System.GetTokenAndSignatureResult.g.h"
#include "Windows.Xbox.System.OnlineStateChangedEventArgs.g.h"
#include "Windows.Xbox.System.GetPictureResult.g.h"
#include "Windows.Xbox.System.SignInCompletedEventArgs.g.h"
#include "Windows.Xbox.System.SignOutStartedEventArgs.g.h"
#include "Windows.Xbox.System.SignOutCompletedEventArgs.g.h"
#include "Windows.Xbox.System.SignOutDeferral.g.h"

namespace winrt::Windows::Xbox::System::implementation
{
    struct GetTokenAndSignatureResult : GetTokenAndSignatureResultT<GetTokenAndSignatureResult>
    {
        GetTokenAndSignatureResult() = default;
        GetTokenAndSignatureResult(hstring signature, hstring token) : signature(signature), token(token) {}

        hstring Signature();
        hstring Token();
    private:
        hstring signature = L"0";
        hstring token = L"0";
    };

    struct OnlineStateChangedEventArgs : OnlineStateChangedEventArgsT<OnlineStateChangedEventArgs>
    {
        OnlineStateChangedEventArgs() = default;
        OnlineStateChangedEventArgs(winrt::Windows::Xbox::System::UserOnlineState state) : state(state) {}

        winrt::Windows::Xbox::System::UserOnlineState State();
    private:
        winrt::Windows::Xbox::System::UserOnlineState state = winrt::Windows::Xbox::System::UserOnlineState::Offline;
    };

    struct SignInCompletedEventArgs : SignInCompletedEventArgsT<SignInCompletedEventArgs>
    {
        SignInCompletedEventArgs() = default;
        SignInCompletedEventArgs(winrt::Windows::Xbox::System::User user) : user(user) {}

        winrt::Windows::Xbox::System::User User();
    private:
        winrt::Windows::Xbox::System::User user;
    };

    struct SignOutStartedEventArgs : SignOutStartedEventArgsT<SignOutStartedEventArgs>
    {
        SignOutStartedEventArgs() = default;
        SignOutStartedEventArgs(winrt::Windows::Xbox::System::SignOutDeferral deferral, winrt::Windows::Xbox::System::User user)
            : deferral(deferral), user(user) {}

        winrt::Windows::Xbox::System::SignOutDeferral GetDeferral();
        winrt::Windows::Xbox::System::User User();
    private:
        winrt::Windows::Xbox::System::SignOutDeferral deferral;
        winrt::Windows::Xbox::System::User user;
    };

    struct SignOutCompletedEventArgs : SignOutCompletedEventArgsT<SignOutCompletedEventArgs>
    {
        SignOutCompletedEventArgs() = default;
        SignOutCompletedEventArgs(winrt::hresult result, winrt::Windows::Xbox::System::User user) : result(result), user(user) {}

        winrt::hresult Result();
        winrt::Windows::Xbox::System::User User();
    private:
        winrt::hresult result;
        winrt::Windows::Xbox::System::User user;
    };

    struct SignOutDeferral : SignOutDeferralT<SignOutDeferral>
    {
        SignOutDeferral() = default;

        void Complete();
    };
}
