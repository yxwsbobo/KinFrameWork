#include "MediaInputStream.h"
#include "MediaPacket.h"
#include "MediaFrame.h"
#include <KinBase/KinBase.h>
#include "MediaInputFile.h"
#include "MediaOutputStream.h"
#include "MediaFrameTranslater.h"
#include "AudioFrameTranslater.h"
#include "VideoFrameTranslater.h"
#include "MediaStreamBlend.h"


void KinMedia::MediaInputStream::DecodePacket( const MediaPacket & pkt )
{
	if( pkt->stream_index != Pointer->index )
	{
		return;
	}

	Maybe( avcodec_send_packet( Decoder.Get(), pkt.Get() ) == 0, "avcodec_send_packet error" );

	while( avcodec_receive_frame( Decoder.Get(), InterFrame.Get() ) == 0 )
	{
		LastPts = InterFrame.GetBestPts();
		DecodeNotify( InterFrame );
	}
}

KinMedia::MediaDecoder& KinMedia::MediaInputStream::GetDecoder() noexcept
{
	return Decoder;
}

std::shared_ptr<KinMedia::MediaFrameTranslater> KinMedia::MediaInputStream::GetFrameTranslate( const MediaOutputStream & stream )
{
	Must( Decoder->codec_type == stream.GetEncoder()->codec_type, "Wrong Type in GetFrameTranslate" );
	if( Decoder->codec_type == AVMEDIA_TYPE_VIDEO )
	{
		return std::make_shared<VideoFrameTranslater>( *this, stream );
	}
	if( Decoder->codec_type == AVMEDIA_TYPE_AUDIO )
	{
		return std::make_shared<AudioFrameTranslater>( *this, stream );
	}
	Must( 0, "Error Type in GetFrameTranslate" );
}

void KinMedia::MediaInputStream::ConnectTo( std::shared_ptr<MediaFrameTranslater> & Dest )
{
	DecodeNotify.Add( std::bind( &MediaFrameTranslater::Translate, Dest, std::placeholders::_1 ) );
}

void KinMedia::MediaInputStream::ConnectTo( MediaInputStream & Dest, StreamBlendSetting Settings )
{
	StreamBlend.push_back( std::make_shared<MediaStreamBlend>( *this, Dest, Settings ) );
}


KinMedia::MediaInputStream::MediaInputStream( AVStream * Stream )
{
	Must( Stream != NULL, "NULL Pointer in MediaInputStream" );
	Pointer.reset( Stream, Deleter );
	Decoder.Init( *Stream );
	CodeCtx = &Decoder;
	LastPts = Stream->start_time;
}

void KinMedia::MediaInputStream::Deleter( AVStream * stream )
{
}
