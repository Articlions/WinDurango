#include "Windows.Xbox.System.UserDisplayInfo.h"
#include "WinDurangoWinRT.h"
#include <winrt/windows.foundation.collections.h>
#include "Windows.Xbox.System.GetPictureResult.h"

namespace winrt::Windows::Xbox::System::implementation
{
    winrt::Windows::Xbox::System::User UserDisplayInfoChangedEventArgs::User()
    {
        return user;
    }

    hstring UserDisplayInfo::Gamertag()
    {
        return winrt::to_hstring(p_wd->config["gamertag"]);
    }

    uint32_t UserDisplayInfo::GamerScore()
    {
        return p_wd->config["gamerscore"];
    }

    hstring UserDisplayInfo::ApplicationDisplayName()
    {
        return winrt::to_hstring(p_wd->config["gamertag"]);
    }

    hstring UserDisplayInfo::GameDisplayName()
    {
        return winrt::to_hstring(p_wd->config["gamertag"]);
    }

    uint32_t UserDisplayInfo::Reputation()
    {
        return reputation;
    }

    winrt::Windows::Xbox::System::UserAgeGroup UserDisplayInfo::AgeGroup()
    {
        return UserAgeGroup::Adult;
    }

    winrt::Windows::Foundation::Collections::IVectorView<uint32_t> UserDisplayInfo::Privileges()
    {
        auto vector = winrt::single_threaded_vector<uint32_t>();
        vector.Append(1);
        return vector.GetView();
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetPictureResult> UserDisplayInfo::GetApplicationPictureAsync(winrt::Windows::Xbox::System::UserPictureSize unk, winrt::Windows::Storage::Streams::IBuffer unka)
    {
        p_wd->log.Warn("WinDurango::WinRT::Windows::Xbox::System", "Unimplemented: GetApplicationPictureAsync");
        throw hresult_not_implemented();
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::System::GetPictureResult> UserDisplayInfo::GetGamerPictureAsync(winrt::Windows::Xbox::System::UserPictureSize unk, winrt::Windows::Storage::Streams::IBuffer unka)
    {
        co_return winrt::make<implementation::GetPictureResult>(unk, unka);
    }
}
