#pragma once
#include "Windows.Xbox.Media.Capture.ApplicationClip.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct ApplicationClip : ApplicationClipT<ApplicationClip>
    {
        ApplicationClip() = default;

        static winrt::Windows::Foundation::IAsyncAction UpdateClipMetadataAsync(winrt::Windows::Xbox::System::User user, hstring serviceConfigId, hstring cloudId, hstring localId, winrt::Windows::Xbox::Media::Capture::ApplicationClipUpdateMetadataInfo updateInfo);
        static winrt::Windows::Foundation::IAsyncAction DeleteClipAsync(winrt::Windows::Xbox::System::User user, hstring serviceConfigId, hstring cloudId, hstring localId);
        hstring LocalId();
        hstring CloudId();
        hstring ServiceConfigId();
        hstring OwnerXboxUserId();
        hstring ClipName();
        hstring GreatestMomentId();
        winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Media::Capture::ApplicationClipLocation> Locations();
        winrt::Windows::Foundation::DateTime DateRecorded();
        winrt::Windows::Foundation::TimeSpan Duration();
        hstring MultiplayerCorrelationId();
        hstring TitleData();
        hstring UserCaption();
        double Rating();
        uint32_t RatingCount();
        uint32_t Views();
        bool SavedByUser();
        winrt::Windows::Xbox::Media::Capture::ApplicationClipVisibility Visibility();
        winrt::Windows::Foundation::Uri GetCloudThumbnail(winrt::Windows::Xbox::Media::Capture::ApplicationClipThumbnailType const& thumbnailType);
        winrt::Windows::Foundation::IAsyncAction UpdateMetadataAsync(winrt::Windows::Xbox::System::User user, winrt::Windows::Xbox::Media::Capture::ApplicationClipUpdateMetadataInfo updateInfo);
        winrt::Windows::Foundation::IAsyncAction DeleteAsync(winrt::Windows::Xbox::System::User user);
        hstring TitleId();
        hstring AchievementId();
        hstring Locale();
        hstring TitleName();
        winrt::Windows::Xbox::Media::Capture::ClipContentAttributes ClipContentAttributes();
    };
}
namespace winrt::Windows::Xbox::Media::Capture::factory_implementation
{
    struct ApplicationClip : ApplicationClipT<ApplicationClip, implementation::ApplicationClip>
    {
    };
}
