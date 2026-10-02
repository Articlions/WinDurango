#pragma once
#include "Windows.Xbox.Media.Capture.ApplicationClipEventData.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct ApplicationClipEventData : ApplicationClipEventDataT<ApplicationClipEventData>
    {
        ApplicationClipEventData() = default;

        int32_t SectionId();
        void SectionId(int32_t value);
        hstring PlayerSessionId();
        void PlayerSessionId(hstring const& value);
        int32_t GameplayMode();
        void GameplayMode(int32_t value);
        int32_t DifficultyLevelId();
        void DifficultyLevelId(int32_t value);
        int32_t ObjectiveId();
        void ObjectiveId(int32_t value);
        int32_t ExitStatusId();
        void ExitStatusId(int32_t value);
        float LocationX();
        void LocationX(float value);
        float LocationY();
        void LocationY(float value);
        float LocationZ();
        void LocationZ(float value);
    };
}
namespace winrt::Windows::Xbox::Media::Capture::factory_implementation
{
    struct ApplicationClipEventData : ApplicationClipEventDataT<ApplicationClipEventData, implementation::ApplicationClipEventData>
    {
    };
}
