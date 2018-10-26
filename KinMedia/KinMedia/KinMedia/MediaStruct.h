#pragma once
#include "FFmpegInclude.h"

namespace KinMedia
{
	struct VideoStreamInfo
	{
		const AVMediaType CodecType = AVMEDIA_TYPE_VIDEO;
		AVPixelFormat PixFmt = AV_PIX_FMT_YUV420P;
		AVCodecID CodeId = AV_CODEC_ID_H265;
		AVRational StreamTimeBase = { 1,90000 };
		AVRational FrameRate = { 24,1 };
		int Width = 640;
		int Height = 360;
		int64_t BitRate = 4000000;
		int GopSize = 12;
		int qMin = 10;
		int qMax = 51;
		int MaxBFrame = 3;
		AVOutputFormat* FileFormat;
	};

	struct AudioStreamInfo
	{
		const AVMediaType CodecType = AVMEDIA_TYPE_AUDIO;
		AVSampleFormat SampleFmt = AV_SAMPLE_FMT_S16;
		AVCodecID CodeId = AV_CODEC_ID_AAC;
		int Channels = 2;
		int SampleRate = 64000;
		int64_t BitRate = 100000;
		AVOutputFormat* FileFormat;
	};

	struct StreamBlendSetting
	{
		static constexpr int BlendUsingSource = -1234543289;
		int Width = BlendUsingSource;
		int Height = BlendUsingSource;
		int Format = BlendUsingSource;

		int StartX = 0;
		int StartY = 0;

		enum
		{
			LeftTop,
			LeftBottom,
			RightTop,
			RightBottom
		}Relative = LeftTop;
	};

	struct MediaPicInfo
	{
		int Width;
		int Height;
		AVPixelFormat Format;
	};
}