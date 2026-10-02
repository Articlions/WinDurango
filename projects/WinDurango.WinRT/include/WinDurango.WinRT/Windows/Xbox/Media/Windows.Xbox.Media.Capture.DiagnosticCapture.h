#pragma once
#include "Windows.Xbox.Media.Capture.DiagnosticCapture.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct DiagnosticCapture : DiagnosticCaptureT<DiagnosticCapture>
    {
        DiagnosticCapture() = default;

        static winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Media::Capture::TemporaryClip> RecordTemporaryClipAsync(winrt::Windows::Foundation::DateTime startTime, winrt::Windows::Foundation::TimeSpan duration);
    };
}
namespace winrt::Windows::Xbox::Media::Capture::factory_implementation
{
    struct DiagnosticCapture : DiagnosticCaptureT<DiagnosticCapture, implementation::DiagnosticCapture>
    {
    };
}
