//
// Tomato Media
// ��Ƶ����Ⱦ Sink
// 
// ���ߣ�SunnyCase 
// �������� 2015-08-07
#pragma once
#include "IVideoRender.h"
#include "StreamRenderSinkBase.h"
#include "Utility/MFWorkerQueueProvider.h"
#include <atomic>

DEFINE_NS_MEDIA

///<summary>��Ƶ�� Sink</summary>
class VideoStreamRenderSink : public StreamRenderSinkBase
{
	// Sink ״̬
	enum VideoStreamRenderSinkState
	{
		// δ���أ�δ����ý�����ͣ�
		NotInitialized,
		// �Ѽ��أ�������ý�����ͣ��� Flush ��
		Initialized,
		// ������
		Prerolling,
		// ׼����ϣ�������ϣ�
		Ready,
	};
public:
	VideoStreamRenderSink(DWORD identifier, MediaRenderSink* mediaSink, IVideoRender* videoRender);

	STDMETHODIMP GetMediaTypeHandler(IMFMediaTypeHandler ** ppHandler) override;
	STDMETHODIMP ProcessSample(IMFSample * pSample) override;
	STDMETHODIMP PlaceMarker(MFSTREAMSINK_MARKER_TYPE eMarkerType, const PROPVARIANT * pvarMarkerValue, const PROPVARIANT * pvarContextValue) override;
	STDMETHODIMP Flush(void) override;

	STDMETHODIMP IsMediaTypeSupported(IMFMediaType * pMediaType, IMFMediaType ** ppMediaType) override;
	STDMETHODIMP GetMediaTypeCount(DWORD * pdwTypeCount) override;
	STDMETHODIMP GetMediaTypeByIndex(DWORD dwIndex, IMFMediaType ** ppType) override;
	STDMETHODIMP SetCurrentMediaType(IMFMediaType * pMediaType) override;
	STDMETHODIMP GetCurrentMediaType(IMFMediaType ** ppMediaType) override;
	STDMETHODIMP GetMajorType(GUID * pguidMajorType) override;

#if (WINVER >= _WIN32_WINNT_WIN8)
	STDMETHODIMP RegisterThreadsEx(DWORD * pdwTaskIndex, LPCWSTR wszClassName, LONG lBasePriority) override;
	STDMETHODIMP SetWorkQueueEx(DWORD dwMultithreadedWorkQueueId, LONG lWorkItemBasePriority) override;
#elif (WINVER >= _WIN32_WINNT_VISTA) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
	STDMETHODIMP RegisterThreads(DWORD dwTaskIndex, LPCWSTR wszClass) override;
	STDMETHODIMP SetWorkQueue(DWORD dwWorkQueueId) override;
#endif
	STDMETHODIMP UnregisterThreads(void) override;

	virtual void NotifyPreroll(MFTIME hnsUpcomingStartTime) override;
private:
	void OnSetMediaType();
	///<param name="setInited">�Ƿ�����״̬Ϊ Initialized��</param>
	///<remarks>����ǰ���״̬����</remarks>
	void FlushCore(bool setInited = false);

	///<remarks>����ǰ���״̬����</remarks>
	void PostSampleRequest();

	///<remarks>����ǰ���״̬����</remarks>
	void PostSampleRequestIfNeeded();

	///<remarks>����ǰ���ܶ�״̬����</remarks>
	void OnProcessIncomingSamples(IMFSample* sample);

	///<remarks>����ǰ���״̬����</remarks>
	void RegisterWorkThreadIfNeeded();

	///<summary>������Ĳ�������Ϊ֡</summary>
	///<remarks>����ǰ���ܶ�״̬����</remarks>
	void OnDecodeFrame();
private:
	UINT32 frameWidth, frameHeight;
	WRL::ComPtr<IVideoRender> videoRender;
	WRL::ComPtr<IMFMediaType> mediaType;
	std::mutex stateMutex;
	VideoStreamRenderSinkState sinkState = NotInitialized;
	std::queue<WRL::ComPtr<IMFSample>> sampleCache;
	std::queue<Frame> frameCache;
	std::mutex sampleCacheMutex;
	std::mutex frameCacheMutex;
	MFWorkerQueueProviderRef workerQueue;
	bool workThreadRegistered = false;

	std::shared_ptr<WorkerThread> decodeFrameWorker;
};

END_NS_MEDIA