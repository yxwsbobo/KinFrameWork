#include "MediaFrame.h"
#include <KinBase/KinBase.h>

KinMedia::MediaFrame::MediaFrame()
{
	auto Frame = av_frame_alloc();
	Maybe( Frame, "Media Frame Alloc Error" );
	Pointer.reset( Frame, Deleter );
}

KinMedia::MediaFrame::MediaFrame( int Width, int Height, AVPixelFormat PixFmt )
	: MediaFrame()
{
	Init( Width, Height, PixFmt );
}

KinMedia::MediaFrame::MediaFrame( const MediaPicInfo & info )
	: MediaFrame()
{
	Init( info );
}

KinMedia::MediaFrame::MediaFrame( uint64_t ChannelLayout, int Rate, int NumSamples, AVSampleFormat Fmt )
	: MediaFrame()
{
	Init( ChannelLayout, Rate, NumSamples, Fmt );
}

void KinMedia::MediaFrame::Init( int Width, int Height, AVPixelFormat PixFmt )
{
	auto Frame = Pointer.get();
	av_frame_unref( Frame );

	Frame->format = PixFmt;
	Frame->width = Width;
	Frame->height = Height;
	Maybe( av_frame_get_buffer( Frame, 32 ) == 0, "Media Frame Alloc Buffer Error" );
}

void KinMedia::MediaFrame::Init( uint64_t ChannelLayout, int Rate, int NumSamples, AVSampleFormat Fmt )
{
	auto Frame = Pointer.get();
	av_frame_unref( Frame );

	Frame->channel_layout = ChannelLayout;
	Frame->sample_rate = Rate;
	Frame->nb_samples = NumSamples;
	Frame->format = Fmt;

	Must( NumSamples, "Error Parameters in MediaFram Init" );
	Maybe( av_frame_get_buffer( Frame, 0 ) == 0, "Media Frame Alloc Buffer Error" );
}

void KinMedia::MediaFrame::Init( const MediaPicInfo & info )
{
	return Init( info.Width, info.Height, info.Format );
}

int64_t KinMedia::MediaFrame::GetBestPts() const noexcept
{
	//return Pointer->best_effort_timestamp;
	return av_frame_get_best_effort_timestamp(Pointer.get());
}

void KinMedia::MediaFrame::Deleter( AVFrame *& Frame )
{
	av_frame_unref( Frame );
	av_frame_free( &Frame );
}
