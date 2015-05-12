﻿#pragma once
#include "IAudioPlayer.h"
#include "MediaSource.h"
#include "MediaSinkHandlerAgent.h"

namespace Tomato
{
	namespace Media
	{
		///<summary>音频播放器</summary>
		[Windows::Foundation::Metadata::WebHostHidden]
		public ref class AudioPlayer sealed : public IAudioPlayer
		{
		public:
			AudioPlayer(Windows::UI::Core::CoreDispatcher^ uiDispatcher, 
				Windows::Media::SystemMediaTransportControls^ mediaControls);

			property Windows::Media::MediaPlaybackStatus CurrentStatus
			{
				virtual Windows::Media::MediaPlaybackStatus get();
			}

			virtual void SetMediaSource(MediaSource^ mediaSource);
			virtual void StartPlayback();
			virtual void PausePlayback();
			virtual void StopPlayback();

			property bool IsMediaTransportControlsEnabled
			{
				virtual bool get();
				virtual void set(bool value);
			}

			property bool IsPlayEnabled
			{
				virtual bool get();
				virtual void set(bool value);
			}

			property bool IsPauseEnabled
			{
				virtual bool get();
				virtual void set(bool value);
			}

			property bool IsStopEnabled
			{
				virtual bool get();
				virtual void set(bool value);
			}

			property bool IsPreviousEnabled
			{
				virtual bool get();
				virtual void set(bool value);
			}

			property bool IsNextEnabled
			{
				virtual bool get();
				virtual void set(bool value);
			}

			property Windows::Foundation::TimeSpan Position
			{
				virtual Windows::Foundation::TimeSpan get();
				virtual void set(Windows::Foundation::TimeSpan value);
			}

			property Windows::Foundation::TimeSpan Duration
			{
				virtual Windows::Foundation::TimeSpan get();
			}

			property double Volume
			{
				double get();
				void set(double value);
			}

			virtual event Windows::Foundation::EventHandler<Windows::Foundation::HResult>^ Error;
			virtual event Windows::Foundation::EventHandler<Windows::Media::MediaPlaybackStatus>^ MediaPlaybackStatusChanged;
			virtual event Windows::Foundation::EventHandler<Windows::Media::SystemMediaTransportControlsButton>^ MediaTransportButtonPressed;
			virtual event Windows::Foundation::EventHandler<Platform::Object^>^ MediaOpened;
			virtual event Windows::Foundation::EventHandler<Platform::Object^>^ MediaEnded;
			virtual event Windows::Foundation::EventHandler<Platform::Object^>^ SeekCompleted;
		private:
			void InitializeMediaTransportControls();

			void OnButtonPressed(Windows::Media::SystemMediaTransportControls ^sender, Windows::Media::SystemMediaTransportControlsButtonPressedEventArgs ^args);
			void RunOnUIDispatcher(std::function<void()>&& handler);
			void SetMediaPlaybackStatus(Windows::Media::MediaPlaybackStatus newStatus);

			void OnSinkStatusChanged(MediaSinkStatus status);
			void OnSinkMediaOpened();
			void OnSinkMediaEnded();
			void OnSinkSeekCompleted();
			void OnSinkError(HRESULT error);

			friend class MediaSinkHandlerAgent;
		private:
			MediaSinkHandlerAgent sinkHandler;
			std::unique_ptr<IMediaSink> sink;
			Windows::Media::SystemMediaTransportControls^ mediaControls;
			Windows::UI::Core::CoreDispatcher^ uiDispatcher;
			Windows::Media::MediaPlaybackStatus currentStatus;
		};
	}
}