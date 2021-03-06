//
// Tomato Media
// 流渲染 Sink 基类
// 
// 作者：SunnyCase 
// 创建日期 2015-08-16
#pragma once
#include "common.h"
#include "../../include/WeakReferenceBase.h"
#include <mfidl.h>
#include <mutex>

DEFINE_NS_MEDIA

class MediaRenderSink;

///<summary>流渲染 Sink 基类</summary>
class __declspec(novtable) StreamRenderSinkBase : public Core::WeakReferenceBase<StreamRenderSinkBase, WRL::RuntimeClassFlags<WRL::ClassicCom>, IMFStreamSink, IMFMediaTypeHandler,
#if (WINVER >= _WIN32_WINNT_WIN8)
	IMFRealTimeClientEx
#elif (WINVER >= _WIN32_WINNT_VISTA) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
	IMFRealTimeClient
#endif
	>
{
public:
	StreamRenderSinkBase(DWORD identifier, MediaRenderSink* mediaSink);
	virtual ~StreamRenderSinkBase();
	
	// 通过 RuntimeClass 继承
	STDMETHODIMP GetEvent(DWORD dwFlags, IMFMediaEvent ** ppEvent) override;
	STDMETHODIMP BeginGetEvent(IMFAsyncCallback * pCallback, IUnknown * punkState) override;
	STDMETHODIMP EndGetEvent(IMFAsyncResult * pResult, IMFMediaEvent ** ppEvent) override;
	STDMETHODIMP QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT * pvValue) override;
	STDMETHODIMP GetMediaSink(IMFMediaSink ** ppMediaSink) override;
	STDMETHODIMP GetIdentifier(DWORD * pdwIdentifier) override;

	// 设置呈现时钟
	virtual void SetPresentationClock(IMFPresentationClock* presentationClock) = 0;
	// 提示开始缓冲
	virtual void NotifyPreroll(MFTIME hnsUpcomingStartTime) = 0;
	// 开始播放
	virtual void Play(MFTIME startTime) = 0;
protected:
	const DWORD identifier;
	WRL::ComPtr<MediaRenderSink> mediaSink;
	WRL::ComPtr<IMFMediaEventQueue> eventQueue;
};

END_NS_MEDIA