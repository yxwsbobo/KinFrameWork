#include "MediaStream.h"
#include <KinBase/KinBase.h>
#include "MediaPacket.h"
#include "MediaFrame.h"


KinMedia::MediaPacket KinMedia::MediaStream::AllocPacket() const
{
	auto &codecpar = Pointer->codecpar;

	if( codecpar->codec_type == AVMEDIA_TYPE_VIDEO )
	{
		auto picture_size = av_image_get_buffer_size( ( AVPixelFormat ) codecpar->format, codecpar->width, codecpar->height, 32 );
		return MediaPacket( picture_size );
	}
	else if( codecpar->codec_type == AVMEDIA_TYPE_AUDIO )
	{
		return MediaPacket();
	}
	Must( 0, "Unknown codectype" );
}

KinMedia::MediaFrame KinMedia::MediaStream::AllocFrame() const
{
	auto &codecpar = Pointer->codecpar;

	if( codecpar->codec_type == AVMEDIA_TYPE_VIDEO )
	{
		return MediaFrame( codecpar->width, codecpar->height, ( AVPixelFormat ) codecpar->format );
	}
	else if( codecpar->codec_type == AVMEDIA_TYPE_AUDIO )
	{
		return MediaFrame( codecpar->channel_layout, codecpar->sample_rate, codecpar->frame_size, ( AVSampleFormat ) codecpar->format );
	}
	Must( 0, "Unknown codectype" );
}

int64_t KinMedia::MediaStream::GetStartPts() const noexcept
{
	return Pointer->start_time;
}

int64_t KinMedia::MediaStream::GetLastPts() const noexcept
{
	return LastPts;
}