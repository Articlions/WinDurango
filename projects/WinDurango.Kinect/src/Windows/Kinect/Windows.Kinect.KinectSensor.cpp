#include "Windows.Kinect.KinectSensor.h"
#include "Windows.Kinect.Body.h"
#include "WinDurangoKinect.h"

namespace winrt::Windows::Kinect::implementation
{
    Kinect::AudioSource AudioBeam::AudioSource()
    {
        return m_audioSource;
    }

    AudioBeamMode AudioBeam::AudioBeamMode()
    {
        return m_audioBeamMode;
    }

    void AudioBeam::AudioBeamMode(Kinect::AudioBeamMode const& value)
    {
        m_audioBeamMode = value;
    }

    float AudioBeam::BeamAngle()
    {
        return m_beamAngle;
    }

    void AudioBeam::BeamAngle(float value)
    {
        m_beamAngle = value;
    }

    float AudioBeam::BeamAngleConfidence()
    {
        return m_beamAngleConfidence;
    }

    Storage::Streams::IInputStream AudioBeam::OpenInputStream()
    {
        return m_inputStream;
    }

    Foundation::TimeSpan AudioBeam::RelativeTime()
    {
        return m_relativeTime;
    }

    Kinect::AudioSource AudioBeamFrame::AudioSource()
    {
        return m_audioSource;
    }
    
    Foundation::TimeSpan AudioBeamFrame::Duration()
    {
        return m_duration;
    }

    Kinect::AudioBeam AudioBeamFrame::AudioBeam()
    {
        return m_audioBeam;
    }

    Foundation::Collections::IVectorView<Kinect::AudioBeamSubFrame> AudioBeamFrame::SubFrames()
    {
        return m_subFrames;
    }

    Foundation::TimeSpan AudioBeamFrame::RelativeTimeStart()
    {
        return m_relativeTimeStart;
    }

    Kinect::AudioBeamFrameReference AudioBeamFrameArrivedEventArgs::FrameReference()
    {
        return m_frameReference;
    }

    event_token AudioBeamFrameReader::FrameArrived(Foundation::TypedEventHandler<Kinect::AudioBeamFrameReader, Kinect::AudioBeamFrameArrivedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::AudioBeamFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }

    void AudioBeamFrameReader::FrameArrived(event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::AudioBeamFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }

    Foundation::Collections::IVectorView<Kinect::AudioBeamFrame> AudioBeamFrameReader::AcquireLatestBeamFrames()
    {
        return m_latestBeamFrames;
    }

    bool AudioBeamFrameReader::IsPaused()
    {
        return m_isPaused;
    }

    void AudioBeamFrameReader::IsPaused(bool value)
    {
        m_isPaused = value;
    }

    Kinect::AudioSource AudioBeamFrameReader::AudioSource()
    {
        return m_audioSource;
    }

    Foundation::Collections::IVectorView<Kinect::AudioBeamFrame> AudioBeamFrameReference::AcquireBeamFrames()
    {
        return m_audioBeamFrames;
    }

    Foundation::TimeSpan AudioBeamFrameReference::RelativeTime()
    {
        return m_relativeTime;
    }

    uint32_t AudioBeamSubFrame::FrameLengthInBytes()
    {
        // TODO: Maybe the sizeof(TimeSpan) x duration of the frame?
        return sizeof(AudioBeamSubFrame::Duration()) * m_duration.count();
    }
    
    Foundation::TimeSpan AudioBeamSubFrame::Duration()
    {
        return m_duration;
    }

    float AudioBeamSubFrame::BeamAngle()
    {
        return m_beamAngle;
    }

    float AudioBeamSubFrame::BeamAngleConfidence()
    {
        return m_beamAngleConfidence;
    }

    AudioBeamMode AudioBeamSubFrame::AudioBeamMode()
    {
        return m_audioBeamMode;
    }

    Foundation::Collections::IVectorView<Kinect::AudioBodyCorrelation> AudioBeamSubFrame::AudioBodyCorrelations()
    {
        return m_audioBodyCorrelations;
    }

    com_array<uint8_t> AudioBeamSubFrame::CopyFrameDataToArray()
    {
        // TODO: I'm not sure what the source buffer is.
        return winrt::com_array<uint8_t>(m_lockAudioBuffer.data(),m_lockAudioBuffer.data() + m_lockAudioBuffer.Length());
    }

    void AudioBeamSubFrame::CopyFrameDataToBuffer(Storage::Streams::IBuffer const& buffer)
    {
        // TODO: I'm not sure what the source buffer is.
        m_lockAudioBuffer = Storage::Streams::Buffer{buffer.Length()};

        std::memcpy(m_lockAudioBuffer.data(), buffer.data(), buffer.Length());

        m_lockAudioBuffer.Length(buffer.Length());
    }

    Storage::Streams::IBuffer AudioBeamSubFrame::LockAudioBuffer()
    {
        return m_lockAudioBuffer;
    }
    
    Foundation::TimeSpan AudioBeamSubFrame::RelativeTime()
    {
        return m_relativeTime;
    }

    uint64_t AudioBodyCorrelation::BodyTrackingId()
    {
        return m_bodyTrackingId;
    }

    KinectAudioCalibrationState AudioSource::AudioCalibrationState()
    {
        return m_kinectAudioCalibrationState;
    }

    event_token AudioSource::FrameCaptured(Foundation::TypedEventHandler<Kinect::AudioSource, Kinect::FrameCapturedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::AudioSource", "Unimplemented: FrameCaptured");
        throw hresult_not_implemented();
    }

    void AudioSource::FrameCaptured(winrt::event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::AudioSource", "Unimplemented: FrameCaptured");
        throw hresult_not_implemented();
    }

    Kinect::KinectSensor AudioSource::KinectSensor()
    {
        return m_kinectSensor;
    }

    bool AudioSource::IsActive()
    {
        return m_isActive;
    }

    uint32_t AudioSource::SubFrameLengthInBytes()
    {
        return m_subFrameLengthInBytes;
    }
    
    Foundation::TimeSpan AudioSource::SubFrameDuration()
    {
        return m_subFrameDuration;
    }

    uint32_t AudioSource::MaxSubFrameCount()
    {
        return m_maxSubFrameCount;
    }
    
    Kinect::AudioBeamFrameReader AudioSource::OpenReader()
    {
        return m_audioBeamReader;
    }
    
    Foundation::Collections::IVectorView<Kinect::AudioBeam> AudioSource::AudioBeams()
    {
        return m_audioBeams;
    }
    
    Foundation::TimeSpan ColorCameraSettings::ExposureTime()
    {
        return m_exposureTime;
    }
    
    Foundation::TimeSpan ColorCameraSettings::FrameInterval()
    {
        return m_frameInterval;
    }

    float ColorCameraSettings::Gain()
    {
        return m_gain;
    }

    float ColorCameraSettings::Gamma()
    {
        return m_gamma;
    }

    ColorImageFormat ColorFrame::RawColorImageFormat()
    {
        return m_rawColorImageFormat;
    }
    
    Kinect::FrameDescription ColorFrame::FrameDescription()
    {
        return m_frameDescription;
    }

    com_array<uint8_t> ColorFrame::CopyRawFrameDataToArray()
    {
        return winrt::com_array<uint8_t>(m_rawImageBuffer.data(), m_rawImageBuffer.data() + m_rawImageBuffer.Length());
    }

    void ColorFrame::CopyRawFrameDataToBuffer(Storage::Streams::IBuffer const& buffer)
    {
        m_rawImageBuffer = Storage::Streams::Buffer{buffer.Length()};

        std::memcpy(m_rawImageBuffer.data(), buffer.data(), buffer.Length());

        m_rawImageBuffer.Length(buffer.Length());
    }

    Storage::Streams::IBuffer ColorFrame::LockRawImageBuffer()
    {
        return m_rawImageBuffer;
    }

    com_array<uint8_t> ColorFrame::CopyConvertedFrameDataToArray(ColorImageFormat const& colorFormat)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrame", "Unimplemented: CopyConvertedFrameDataToArray");
        throw hresult_not_implemented();
    }
    
    void ColorFrame::CopyConvertedFrameDataToBuffer(Storage::Streams::Buffer const& buffer, ColorImageFormat const& colorFormat)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrame", "Unimplemented: CopyConvertedFrameDataToBuffer");
        throw hresult_not_implemented();
    }
    
    Kinect::FrameDescription ColorFrame::CreateFrameDescription(ColorImageFormat const& format)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrame", "Unimplemented: CreateFrameDescription");
        throw hresult_not_implemented();
    }
    
    Kinect::ColorCameraSettings ColorFrame::ColorCameraSettings()
    {
        return m_colorCameraSettings;
    }
    
    Foundation::TimeSpan ColorFrame::RelativeTime()
    {
        return m_relativeTime;
    }
    
    Kinect::ColorFrameSource ColorFrame::ColorFrameSource()
    {
        return m_colorFrameSource;
    }
    
    Kinect::ColorFrameReference ColorFrameArrivedEventArgs::FrameReference()
    {
        return m_frameReference;
    }
    
    event_token ColorFrameReader::FrameArrived(Foundation::TypedEventHandler<Kinect::ColorFrameReader, Kinect::ColorFrameArrivedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }

    void ColorFrameReader::FrameArrived(winrt::event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }
    
    Kinect::ColorFrame ColorFrameReader::AcquireLatestFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrameReader", "Unimplemented: AcquireLatestFrame");
        throw hresult_not_implemented();
    }

    bool ColorFrameReader::IsPaused()
    {
        return m_isPaused;
    }

    void ColorFrameReader::IsPaused(bool value)
    {
        m_isPaused = value;
    }
    
    Kinect::ColorFrameSource ColorFrameReader::ColorFrameSource()
    {
        return m_colorFrameSource;
    }
    
    Kinect::ColorFrame ColorFrameReference::AcquireFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrameReference", "Unimplemented: AcquireFrame");
        throw hresult_not_implemented();
    }
    
    Foundation::TimeSpan ColorFrameReference::RelativeTime()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrameReference", "Unimplemented: RelativeTime");
        throw hresult_not_implemented();
    }
    
    event_token ColorFrameSource::FrameCaptured(Foundation::TypedEventHandler<Kinect::ColorFrameSource, Kinect::FrameCapturedEventArgs> const& handler)
    {
        return {};
    }
    
    void ColorFrameSource::FrameCaptured(event_token const& token) noexcept
    {

    }

    bool ColorFrameSource::IsActive()
    {
        BOOLEAN isActive = 0;
        
        HRESULT hresult = m_pColorFrameSource->get_IsActive(&isActive);
        
        return isActive;
    }
    
    Kinect::ColorFrameReader ColorFrameSource::OpenReader()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrameSource", "Unimplemented: OpenReader");
        throw hresult_not_implemented();
    }
    
    Kinect::FrameDescription ColorFrameSource::CreateFrameDescription(ColorImageFormat const& format)
    {
        ::IFrameDescription *pFrameDescription = nullptr;
        m_pColorFrameSource->CreateFrameDescription((::ColorImageFormat)format, &pFrameDescription);
        return winrt::make<implementation::FrameDescription>(pFrameDescription);
    }
    
    Kinect::FrameDescription ColorFrameSource::FrameDescription()
    {
        ::IFrameDescription *pFrameDescription = nullptr;
        m_pColorFrameSource->get_FrameDescription(&pFrameDescription);
        return winrt::make<implementation::FrameDescription>(pFrameDescription);
    }
    
    Kinect::KinectSensor ColorFrameSource::KinectSensor()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::ColorFrameSource", "Unimplemented: KinectSensor");
        throw hresult_not_implemented();
    }
    
    CameraIntrinsics CoordinateMapper::GetDepthCameraIntrinsics()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: GetDepthCameraIntrinsics");
        throw hresult_not_implemented();
    }
    winrt::event_token CoordinateMapper::CoordinateMappingChanged(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Kinect::CoordinateMapper, winrt::Windows::Kinect::CoordinateMappingChangedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: CoordinateMappingChanged");
        throw hresult_not_implemented();
    }
    void CoordinateMapper::CoordinateMappingChanged(winrt::event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: CoordinateMappingChanged");
        throw hresult_not_implemented();
    }
    winrt::Windows::Kinect::DepthSpacePoint CoordinateMapper::MapCameraPointToDepthSpace(winrt::Windows::Kinect::CameraSpacePoint const& cameraPoint)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapCameraPointToDepthSpace");
        throw hresult_not_implemented();
    }
    winrt::Windows::Kinect::ColorSpacePoint CoordinateMapper::MapCameraPointToColorSpace(winrt::Windows::Kinect::CameraSpacePoint const& cameraPoint)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapCameraPointToColorSpace");
        throw hresult_not_implemented();
    }
    winrt::Windows::Kinect::CameraSpacePoint CoordinateMapper::MapDepthPointToCameraSpace(winrt::Windows::Kinect::DepthSpacePoint const& depthPoint, uint16_t depth)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapDepthPointToCameraSpace");
        throw hresult_not_implemented();
    }
    winrt::Windows::Kinect::ColorSpacePoint CoordinateMapper::MapDepthPointToColorSpace(winrt::Windows::Kinect::DepthSpacePoint const& depthPoint, uint16_t depth)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapDepthPointToColorSpace");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::DepthSpacePoint> CoordinateMapper::MapCameraPointsToDepthSpace(array_view<winrt::Windows::Kinect::CameraSpacePoint const> cameraPoints)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapCameraPointsToDepthSpace");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::ColorSpacePoint> CoordinateMapper::MapCameraPointsToColorSpace(array_view<winrt::Windows::Kinect::CameraSpacePoint const> cameraPoints)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapCameraPointsToColorSpace");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::CameraSpacePoint> CoordinateMapper::MapDepthPointsToCameraSpace(array_view<winrt::Windows::Kinect::DepthSpacePoint const> depthPoints, array_view<uint16_t const> depths)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapDepthPointsToCameraSpace");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::ColorSpacePoint> CoordinateMapper::MapDepthPointsToColorSpace(array_view<winrt::Windows::Kinect::DepthSpacePoint const> depthPoints, array_view<uint16_t const> depths)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapDepthPointsToColorSpace");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::CameraSpacePoint> CoordinateMapper::MapDepthFrameToCameraSpace(array_view<uint16_t const> depthFrameData)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapDepthFrameToCameraSpace");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::ColorSpacePoint> CoordinateMapper::MapDepthFrameToColorSpace(array_view<uint16_t const> depthFrameData)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapDepthFrameToColorSpace");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::CameraSpacePoint> CoordinateMapper::MapDepthFrameToCameraSpaceUsingIBuffer(winrt::Windows::Storage::Streams::IBuffer const& depthFrameData)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapDepthFrameToCameraSpaceUsingIBuffer");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::ColorSpacePoint> CoordinateMapper::MapDepthFrameToColorSpaceUsingIBuffer(winrt::Windows::Storage::Streams::IBuffer const& depthFrameData)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapDepthFrameToColorSpaceUsingIBuffer");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::DepthSpacePoint> CoordinateMapper::MapColorFrameToDepthSpace(array_view<uint16_t const> depthFrameData)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapColorFrameToDepthSpace");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::CameraSpacePoint> CoordinateMapper::MapColorFrameToCameraSpace(array_view<uint16_t const> depthFrameData)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapColorFrameToCameraSpace");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::DepthSpacePoint> CoordinateMapper::MapColorFrameToDepthSpaceUsingIBuffer(winrt::Windows::Storage::Streams::IBuffer const& depthFrameData)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapColorFrameToDepthSpaceUsingIBuffer");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Kinect::CameraSpacePoint> CoordinateMapper::MapColorFrameToCameraSpaceUsingIBuffer(winrt::Windows::Storage::Streams::IBuffer const& depthFrameData)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: MapColorFrameToCameraSpaceUsingIBuffer");
        throw hresult_not_implemented();
    }
    com_array<winrt::Windows::Foundation::Point> CoordinateMapper::GetDepthFrameToCameraSpaceTable()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::CoordinateMapper", "Unimplemented: GetDepthFrameToCameraSpaceTable");
        throw hresult_not_implemented();
    }

    com_array<uint16_t> DepthFrame::CopyFrameDataToArray()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrame", "Unimplemented: CopyFrameDataToArray");
        throw hresult_not_implemented();
    }

    // TODO: I'm not sure if the m_lockImageBuffer is the buffer we are copying to.
    void DepthFrame::CopyFrameDataToBuffer(Storage::Streams::IBuffer const& buffer)
    {
        m_lockImageBuffer = Storage::Streams::Buffer{ buffer.Length() };

        std::memcpy(m_lockImageBuffer.data(), buffer.data(), buffer.Length());

        m_lockImageBuffer.Length(buffer.Length());
    }
    
    Storage::Streams::IBuffer DepthFrame::LockImageBuffer()
    {
        return m_lockImageBuffer;
    }
    
    Kinect::FrameDescription DepthFrame::FrameDescription()
    {
        return m_frameDescription;
    }
    
    Foundation::TimeSpan DepthFrame::RelativeTime()
    {
        return m_relativeTime;
    }
    
    Kinect::DepthFrameSource DepthFrame::DepthFrameSource()
    {
        return m_pDepthFrameSource;
    }
    
    uint16_t DepthFrame::DepthMinReliableDistance()
    {
        return m_depthMinReliableDistance;
    }

    uint16_t DepthFrame::DepthMaxReliableDistance()
    {
        return m_depthMaxReliableDistance;
    }
    
    Kinect::DepthFrameReference DepthFrameArrivedEventArgs::FrameReference()
    {
        return m_frameReference;
    }
    
    event_token DepthFrameReader::FrameArrived(Foundation::TypedEventHandler<Kinect::DepthFrameReader, Kinect::DepthFrameArrivedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }
    
    void DepthFrameReader::FrameArrived(event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }
    
    Kinect::DepthFrame DepthFrameReader::AcquireLatestFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameReader", "Unimplemented: AcquireLatestFrame");
        throw hresult_not_implemented();
    }

    bool DepthFrameReader::IsPaused()
    {
        return m_isPaused;
    }
    
    void DepthFrameReader::IsPaused(bool value)
    {
        m_isPaused = value;
    }
    
    Kinect::DepthFrameSource DepthFrameReader::DepthFrameSource()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameReader", "Unimplemented: DepthFrameSource");
        throw hresult_not_implemented();
    }
    
    Kinect::DepthFrame DepthFrameReference::AcquireFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameReference", "Unimplemented: AcquireFrame");
        throw hresult_not_implemented();
    }
    
    Foundation::TimeSpan DepthFrameReference::RelativeTime()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameReference", "Unimplemented: RelativeTime");
        throw hresult_not_implemented();
    }
    
    event_token DepthFrameSource::FrameCaptured(Foundation::TypedEventHandler<Kinect::DepthFrameSource, Kinect::FrameCapturedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameSource", "Unimplemented: FrameCaptured");
        throw hresult_not_implemented();
    }
    void DepthFrameSource::FrameCaptured(winrt::event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameSource", "Unimplemented: FrameCaptured");
        throw hresult_not_implemented();
    }
    bool DepthFrameSource::IsActive()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameSource", "Unimplemented: IsActive");
        throw hresult_not_implemented();
    }
    winrt::Windows::Kinect::DepthFrameReader DepthFrameSource::OpenReader()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameSource", "Unimplemented: OpenReader");
        throw hresult_not_implemented();
    }
    uint16_t DepthFrameSource::DepthMinReliableDistance()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameSource", "Unimplemented: DepthMinReliableDistance");
        throw hresult_not_implemented();
    }
    uint16_t DepthFrameSource::DepthMaxReliableDistance()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameSource", "Unimplemented: DepthMaxReliableDistance");
        throw hresult_not_implemented();
    }
    winrt::Windows::Kinect::FrameDescription DepthFrameSource::FrameDescription()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameSource", "Unimplemented: FrameDescription");
        throw hresult_not_implemented();
    }
    winrt::Windows::Kinect::KinectSensor DepthFrameSource::KinectSensor()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::DepthFrameSource", "Unimplemented: KinectSensor");
        throw hresult_not_implemented();
    }
    winrt::Windows::Kinect::FrameSourceTypes FrameCapturedEventArgs::FrameType()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::FrameCapturedEventArgs", "Unimplemented: FrameType");
        throw hresult_not_implemented();
    }
    winrt::Windows::Kinect::FrameCapturedStatus FrameCapturedEventArgs::FrameStatus()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::FrameCapturedEventArgs", "Unimplemented: FrameStatus");
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::TimeSpan FrameCapturedEventArgs::RelativeTime()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::FrameCapturedEventArgs", "Unimplemented: RelativeTime");
        throw hresult_not_implemented();
    }
    
    int32_t FrameDescription::Width()
    {
        int32_t Width = 0;
        
        HRESULT hresult = m_pFrameDescription->get_Width(&Width);
    
        return Width;
    }
    
    int32_t FrameDescription::Height()
    {
        int32_t Height = 0;
        
        HRESULT hresult = m_pFrameDescription->get_Height(&Height);
        
        return Height;
    }
    
    float FrameDescription::HorizontalFieldOfView()
    {
        float HorizontalFieldOfView = 0;
        
        HRESULT hresult = m_pFrameDescription->get_HorizontalFieldOfView(&HorizontalFieldOfView);
    
        return HorizontalFieldOfView;
    }

    float FrameDescription::VerticalFieldOfView()
    {
        float VerticalFieldOfView = 0;
        
        HRESULT hresult = m_pFrameDescription->get_VerticalFieldOfView(&VerticalFieldOfView);
        
        return VerticalFieldOfView;
    }

    float FrameDescription::DiagonalFieldOfView()
    {
        float DiagonalFieldOfView = 0;
        
        HRESULT hresult = m_pFrameDescription->get_DiagonalFieldOfView(&DiagonalFieldOfView);
        
        return DiagonalFieldOfView;
    }

    uint32_t FrameDescription::LengthInPixels()
    {
        uint32_t LengthInPixels = 0;
        
        HRESULT hresult = m_pFrameDescription->get_LengthInPixels(&LengthInPixels);
        
        return LengthInPixels;
    }

    uint32_t FrameDescription::BytesPerPixel()
    {
        uint32_t BytesPerPixel = 0;
        
        HRESULT hresult = m_pFrameDescription->get_BytesPerPixel(&BytesPerPixel);
    
        return BytesPerPixel;
    }

    com_array<uint16_t> InfraredFrame::CopyFrameDataToArray()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrame", "Unimplemented: CopyFrameDataToArray");
        throw hresult_not_implemented();
    }

    void InfraredFrame::CopyFrameDataToBuffer(Storage::Streams::IBuffer const& buffer)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrame", "Unimplemented: CopyFrameDataToBuffer");
        throw hresult_not_implemented();
    }

    Storage::Streams::IBuffer InfraredFrame::LockImageBuffer()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrame", "Unimplemented: LockImageBuffer");
        throw hresult_not_implemented();
    }

    Kinect::FrameDescription InfraredFrame::FrameDescription()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrame", "Unimplemented: FrameDescription");
        throw hresult_not_implemented();
    }
    
    Foundation::TimeSpan InfraredFrame::RelativeTime()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrame", "Unimplemented: RelativeTime");
        throw hresult_not_implemented();
    }
    
    Kinect::InfraredFrameSource InfraredFrame::InfraredFrameSource()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrame", "Unimplemented: InfraredFrameSource");
        throw hresult_not_implemented();
    }
    
    Kinect::InfraredFrameReference InfraredFrameArrivedEventArgs::FrameReference()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameArrivedEventArgs", "Unimplemented: FrameReference");
        throw hresult_not_implemented();
    }
    
    event_token InfraredFrameReader::FrameArrived(Foundation::TypedEventHandler<Kinect::InfraredFrameReader, Kinect::InfraredFrameArrivedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }

    void InfraredFrameReader::FrameArrived(winrt::event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }
    
    Kinect::InfraredFrame InfraredFrameReader::AcquireLatestFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameReader", "Unimplemented: AcquireLatestFrame");
        throw hresult_not_implemented();
    }

    bool InfraredFrameReader::IsPaused()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameReader", "Unimplemented: IsPaused");
        throw hresult_not_implemented();
    }

    void InfraredFrameReader::IsPaused(bool value)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameReader", "Unimplemented: IsPaused");
        throw hresult_not_implemented();
    }
    
    Kinect::InfraredFrameSource InfraredFrameReader::InfraredFrameSource()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameReader", "Unimplemented: InfraredFrameSource");
        throw hresult_not_implemented();
    }
    
    Kinect::InfraredFrame InfraredFrameReference::AcquireFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameReference", "Unimplemented: AcquireFrame");
        throw hresult_not_implemented();
    }
    
    Foundation::TimeSpan InfraredFrameReference::RelativeTime()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameReference", "Unimplemented: RelativeTime");
        throw hresult_not_implemented();
    }
    
    event_token InfraredFrameSource::FrameCaptured(Foundation::TypedEventHandler<Kinect::InfraredFrameSource, Kinect::FrameCapturedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameSource", "Unimplemented: FrameCaptured");
        throw hresult_not_implemented();
    }

    void InfraredFrameSource::FrameCaptured(event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameSource", "Unimplemented: FrameCaptured");
        throw hresult_not_implemented();
    }

    bool InfraredFrameSource::IsActive()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameSource", "Unimplemented: IsActive");
        throw hresult_not_implemented();
    }

    Kinect::InfraredFrameReader InfraredFrameSource::OpenReader()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameSource", "Unimplemented: OpenReader");
        throw hresult_not_implemented();
    }

    Kinect::FrameDescription InfraredFrameSource::FrameDescription()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameSource", "Unimplemented: FrameDescription");
        throw hresult_not_implemented();
    }
    
    Kinect::KinectSensor InfraredFrameSource::KinectSensor()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::InfraredFrameSource", "Unimplemented: KinectSensor");
        throw hresult_not_implemented();
    }

    bool IsAvailableChangedEventArgs::IsAvailable()
    {
        return m_isAvailable;
    }

    com_array<uint16_t> LongExposureInfraredFrame::CopyFrameDataToArray()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrame", "Unimplemented: CopyFrameDataToArray");
        throw hresult_not_implemented();
    }
    
    void LongExposureInfraredFrame::CopyFrameDataToBuffer(Storage::Streams::IBuffer const& buffer)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrame", "Unimplemented: CopyFrameDataToBuffer");
        throw hresult_not_implemented();
    }
    
    Storage::Streams::IBuffer LongExposureInfraredFrame::LockImageBuffer()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrame", "Unimplemented: LockImageBuffer");
        throw hresult_not_implemented();
    }
    
    Kinect::FrameDescription LongExposureInfraredFrame::FrameDescription()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrame", "Unimplemented: FrameDescription");
        throw hresult_not_implemented();
    }
    
    Foundation::TimeSpan LongExposureInfraredFrame::RelativeTime()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrame", "Unimplemented: RelativeTime");
        throw hresult_not_implemented();
    }
    
    Kinect::LongExposureInfraredFrameSource LongExposureInfraredFrame::LongExposureInfraredFrameSource()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrame", "Unimplemented: LongExposureInfraredFrameSource");
        throw hresult_not_implemented();
    }
    
    Kinect::LongExposureInfraredFrameReference LongExposureInfraredFrameArrivedEventArgs::FrameReference()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameArrivedEventArgs", "Unimplemented: FrameReference");
        throw hresult_not_implemented();
    }
    
    event_token LongExposureInfraredFrameReader::FrameArrived(Foundation::TypedEventHandler<Kinect::LongExposureInfraredFrameReader, Kinect::LongExposureInfraredFrameArrivedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }

    void LongExposureInfraredFrameReader::FrameArrived(event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameReader", "Unimplemented: FrameArrived");
        throw hresult_not_implemented();
    }

    Kinect::LongExposureInfraredFrame LongExposureInfraredFrameReader::AcquireLatestFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameReader", "Unimplemented: AcquireLatestFrame");
        throw hresult_not_implemented();
    }

    bool LongExposureInfraredFrameReader::IsPaused()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameReader", "Unimplemented: IsPaused");
        throw hresult_not_implemented();
    }

    void LongExposureInfraredFrameReader::IsPaused(bool value)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameReader", "Unimplemented: IsPaused");
        throw hresult_not_implemented();
    }

    Kinect::LongExposureInfraredFrameSource LongExposureInfraredFrameReader::LongExposureInfraredFrameSource()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameReader", "Unimplemented: LongExposureInfraredFrameSource");
        throw hresult_not_implemented();
    }

    Kinect::LongExposureInfraredFrame LongExposureInfraredFrameReference::AcquireFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameReference", "Unimplemented: AcquireFrame");
        throw hresult_not_implemented();
    }

    Foundation::TimeSpan LongExposureInfraredFrameReference::RelativeTime()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameReference", "Unimplemented: RelativeTime");
        throw hresult_not_implemented();
    }

    event_token LongExposureInfraredFrameSource::FrameCaptured(Foundation::TypedEventHandler<Kinect::LongExposureInfraredFrameSource, Kinect::FrameCapturedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameSource", "Unimplemented: FrameCaptured");
        throw hresult_not_implemented();
    }

    void LongExposureInfraredFrameSource::FrameCaptured(winrt::event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameSource", "Unimplemented: FrameCaptured");
        throw hresult_not_implemented();
    }

    bool LongExposureInfraredFrameSource::IsActive()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameSource", "Unimplemented: IsActive");
        throw hresult_not_implemented();
    }
    
    Kinect::LongExposureInfraredFrameReader LongExposureInfraredFrameSource::OpenReader()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameSource", "Unimplemented: OpenReader");
        throw hresult_not_implemented();
    }

    Kinect::FrameDescription LongExposureInfraredFrameSource::FrameDescription()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::LongExposureInfraredFrameSource", "Unimplemented: FrameDescription");
        throw hresult_not_implemented();
    }

    Kinect::KinectSensor LongExposureInfraredFrameSource::KinectSensor()
    {
        return m_kinectSensor;
    }

    Kinect::ColorFrameReference MultiSourceFrame::ColorFrameReference()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrame", "Unimplemented: ColorFrameReference");
        throw hresult_not_implemented();
    }
    
    Kinect::DepthFrameReference MultiSourceFrame::DepthFrameReference()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrame", "Unimplemented: DepthFrameReference");
        throw hresult_not_implemented();
    }
    
    Kinect::BodyFrameReference MultiSourceFrame::BodyFrameReference()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrame", "Unimplemented: BodyFrameReference");
        throw hresult_not_implemented();
    }
    
    Kinect::BodyIndexFrameReference MultiSourceFrame::BodyIndexFrameReference()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrame", "Unimplemented: BodyIndexFrameReference");
        throw hresult_not_implemented();
    }
    
    Kinect::InfraredFrameReference MultiSourceFrame::InfraredFrameReference()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrame", "Unimplemented: InfraredFrameReference");
        throw hresult_not_implemented();
    }
    
    Kinect::LongExposureInfraredFrameReference MultiSourceFrame::LongExposureInfraredFrameReference()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrame", "Unimplemented: LongExposureInfraredFrameReference");
        throw hresult_not_implemented();
    }
    
    Kinect::MultiSourceFrameReference MultiSourceFrameArrivedEventArgs::FrameReference()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrameArrivedEventArgs", "Unimplemented: FrameReference");
        throw hresult_not_implemented();
    }
    
    event_token MultiSourceFrameReader::MultiSourceFrameArrived(Foundation::TypedEventHandler<Kinect::MultiSourceFrameReader, Kinect::MultiSourceFrameArrivedEventArgs> const& handler)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrameReader", "Unimplemented: MultiSourceFrameArrived");
        throw hresult_not_implemented();
    }
    void MultiSourceFrameReader::MultiSourceFrameArrived(winrt::event_token const& token) noexcept
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrameReader", "Unimplemented: MultiSourceFrameArrived");
        throw hresult_not_implemented();
    }

    Kinect::MultiSourceFrame MultiSourceFrameReader::AcquireLatestFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrameReader", "Unimplemented: AcquireLatestFrame");
        throw hresult_not_implemented();
    }

    FrameSourceTypes MultiSourceFrameReader::FrameSourceTypes()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrameReader", "Unimplemented: FrameSourceTypes");
        throw hresult_not_implemented();
    }

    bool MultiSourceFrameReader::IsPaused()
    {
        return m_isPaused;
    }

    void MultiSourceFrameReader::IsPaused(bool value)
    {
        m_isPaused = value;
    }
    
    Kinect::KinectSensor MultiSourceFrameReader::KinectSensor()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrameReference", "Unimplemented: KinectSensor");

        throw hresult_not_implemented();
    }
    
    Kinect::MultiSourceFrame MultiSourceFrameReference::AcquireFrame()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::MultiSourceFrameReference", "Unimplemented: AcquireFrame");

        throw hresult_not_implemented();
    }
    
    Kinect::KinectSensor KinectSensor::GetDefault()
    {
        return winrt::make<KinectSensor>();
    }

    Foundation::Collections::IObservableMap<hstring, Kinect::KinectSensor> KinectSensor::Sensors()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::KinectSensor", "Unimplemented: Sensors");

        throw hresult_not_implemented();
    }

    event_token KinectSensor::IsAvailableChanged(Foundation::TypedEventHandler<Kinect::KinectSensor, Kinect::IsAvailableChangedEventArgs> const& handler)
    {
        return {};
    }

    void KinectSensor::IsAvailableChanged(event_token const& token) noexcept
    {

    }

    void KinectSensor::Open()
    {
        m_pKinectSensor->Open();
    }
    
    void KinectSensor::Close()
    {
        m_pKinectSensor->Close();
    }
    
    bool KinectSensor::IsOpen()
    {
        BOOLEAN isOpen = 0;
        m_pKinectSensor->get_IsOpen(&isOpen);
        return (bool)isOpen;
    }
    
    bool KinectSensor::IsAvailable()
    {
        BOOLEAN isAvailable = 0;
        m_pKinectSensor->get_IsAvailable(&isAvailable);
        return (bool)isAvailable;
    }
    
    Kinect::ColorFrameSource KinectSensor::ColorFrameSource()
    {
        ::IColorFrameSource *pColorFrameSource = nullptr;

        HRESULT hresult = m_pKinectSensor->get_ColorFrameSource(&pColorFrameSource);

        return winrt::make<implementation::ColorFrameSource>(pColorFrameSource);
    }
    
    Kinect::DepthFrameSource KinectSensor::DepthFrameSource()
    {
        ::IDepthFrameSource *pDepthFrameSource = nullptr;

        HRESULT hresult = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);

        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::KinectSensor", "Unimplemented: DepthFrameSource");
        
        throw hresult_not_implemented();
    }
    
    Kinect::BodyFrameSource KinectSensor::BodyFrameSource()
    {
        ::IBodyFrameSource *pBodyFrameSource = nullptr;

        HRESULT hresult = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);

        return winrt::make<implementation::BodyFrameSource>(pBodyFrameSource);
    }

    Kinect::BodyIndexFrameSource KinectSensor::BodyIndexFrameSource()
    { 
        ::IBodyIndexFrameSource *pBodyIndexFrameSource = nullptr;

        HRESULT hresult = m_pKinectSensor->get_BodyIndexFrameSource(&pBodyIndexFrameSource);

        return winrt::make<implementation::BodyIndexFrameSource>();
    }
    
    Kinect::InfraredFrameSource KinectSensor::InfraredFrameSource()
    {
        ::IInfraredFrameSource *pInfraredFrameSource = nullptr;

        HRESULT hresult = m_pKinectSensor->get_InfraredFrameSource(&pInfraredFrameSource);

        return winrt::make<implementation::InfraredFrameSource>();
    }
    
    Kinect::LongExposureInfraredFrameSource KinectSensor::LongExposureInfraredFrameSource()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::KinectSensor", "Unimplemented: LongExposureInfraredFrameSource");

        throw hresult_not_implemented();
    }
    
    Kinect::AudioSource KinectSensor::AudioSource()
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::KinectSensor", "Unimplemented: AudioSource");

        throw hresult_not_implemented();
    }
    
    Kinect::MultiSourceFrameReader KinectSensor::OpenMultiSourceFrameReader(FrameSourceTypes const& enabledFrameSourceTypes)
    {
        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::KinectSensor", "Unimplemented: OpenMultiSourceFrameReader");
        
        throw hresult_not_implemented();
    }
    
    Kinect::CoordinateMapper KinectSensor::CoordinateMapper()
    {
        ::ICoordinateMapper *pCoordinateMapper = nullptr;

        HRESULT hresult = m_pKinectSensor->get_CoordinateMapper(&pCoordinateMapper);

        p_wd->log.Warn("WinDurango::Kinect::Windows::Kinect::KinectSensor", "Unimplemented: OpenMultiSourceFrameReader");
        
        throw hresult_not_implemented();
    }
    
    hstring KinectSensor::UniqueKinectId()
    {
        return L"0";
    }
    
    KinectCapabilities KinectSensor::KinectCapabilities()
    {
        DWORD kinectCapabilities = 0;

        HRESULT hresult = m_pKinectSensor->get_KinectCapabilities(&kinectCapabilities);
        
        return static_cast<Kinect::KinectCapabilities>(kinectCapabilities);
    }
}