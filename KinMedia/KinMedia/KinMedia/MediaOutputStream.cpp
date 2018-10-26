#include "MediaOutputStream.h"
#include "MediaFrame.h"
#include <KinBase/KinBase.h>
#include "MediaOutputFile.h"
#include "MediaFrameTranslater.h"


void KinMedia::MediaOutputStream::EncodeFrame( const MediaFrame & frame )
{
	if( frame.TimeArrive == false )
	{
		return;
	}

	Maybe( avcodec_send_frame( Encoder.Get(), frame.Get()) == 0, "Send Frame Error" );

	while( avcodec_receive_packet( Encoder.Get(), InterPacket.Get() ) == 0 )
	{
		InterPacket->stream_index = Pointer->index;
		//InterPacket->dts = InterPacket->pts;
		LastPts = InterPacket->pts;
		EncodeNotify( InterPacket );
	}
}

const KinMedia::MediaEncoder & KinMedia::MediaOutputStream::GetEncoder() const noexcept
{
	return Encoder;
}

void KinMedia::MediaOutputStream::ConnectTo( MediaOutputFile & outFile )
{
	EncodeNotify.Add( std::bind( &MediaOutputFile::WritePacket, &outFile, std::placeholders::_1 ) );
}

void KinMedia::MediaOutputStream::SetNextKeyFrame()
{
	KeyFrame = 1;
}

KinMedia::MediaOutputStream::MediaOutputStream( MediaOutputFile & vFile, VideoStreamInfo & info )
{
	auto Stream = avformat_new_stream( vFile.Get(), NULL );
	Maybe( Stream, "Can't new stream" );

	Stream->start_time = vFile->start_time;
	Stream->time_base = info.StreamTimeBase;
	Stream->avg_frame_rate = info.FrameRate;
	Stream->r_frame_rate = av_div_q( av_inv_q( Stream->time_base ), info.FrameRate );

	Pointer.reset( Stream, Deleter );

	Encoder.Init( *Stream, info );
	InterPacket = AllocPacket();
	CodeCtx = &Encoder;
	LastPts = Stream->start_time;
}

KinMedia::MediaOutputStream::MediaOutputStream( MediaOutputFile & vFile, AudioStreamInfo & info )
{
	auto Stream = avformat_new_stream( vFile.Get(), NULL );
	Maybe( Stream, "Can't new stream" );
	Stream->start_time = vFile->start_time;
	Stream->time_base = { 1,info.SampleRate };
	Pointer.reset( Stream, Deleter );
	Encoder.Init( *Stream, info );
	CodeCtx = &Encoder;
	LastPts = Stream->start_time;
}

void KinMedia::MediaOutputStream::Deleter( AVStream * stream )
{
}
