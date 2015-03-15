﻿//
// Tomato Media
// 媒体源
// 
// (c) SunnyCase 
// 创建日期 2015-03-15
#pragma once
#include "tomato.media.core.h"

NSDEF_TOMATO_MEDIA

// 媒体源
class IMediaSource
{
public:
	IMediaSource(){}
	virtual ~IMediaSource(){}

	static std::unique_ptr<IMediaSource> CreateRTMediaSource(Windows::Storage::Streams::IRandomAccessStream^ stream);
};

NSED_TOMATO_MEDIA