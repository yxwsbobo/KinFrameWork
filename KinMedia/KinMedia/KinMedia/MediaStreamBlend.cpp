#include "MediaStreamBlend.h"
#include "MediaInputStream.h"
#include <KinBase/KinBase.h>


KinMedia::MediaStreamBlend::MediaStreamBlend( MediaInputStream & Source, MediaInputStream & Dest, const StreamBlendSetting & Settings )
{
	auto &sP = Source->codecpar;
	auto &dP = Dest->codecpar;
	Must( sP->codec_type == AVMEDIA_TYPE_VIDEO, "Only Video Can Blend" );
	Must( dP->codec_type == AVMEDIA_TYPE_VIDEO, "Only Video Can Blend" );

	Source.DecodeNotify.Add( std::bind( &MediaStreamBlend::SetFrame, this, std::placeholders::_1 ) );
	RichFunction<void( const MediaFrame& )> Fun = std::bind( &MediaStreamBlend::PutFrame, this, std::placeholders::_1 );
	Fun.SetIndex( 100 );
	Dest.DecodeNotify.Add( Fun );

	MaxX = dP->width;
	MaxY = dP->height;

	BlendSets.Relative = Settings.Relative;

	if( Settings.Width == Settings.BlendUsingSource )
	{
		BlendSets.Width = sP->width;
	}
	else
	{
		BlendSets.Width = Settings.Width;
	}

	if( Settings.Height == Settings.BlendUsingSource )
	{
		BlendSets.Height = sP->height;
	}
	else
	{
		BlendSets.Height = Settings.Height;
	}

	BlendSets.Width >>= 1;
	BlendSets.Width <<= 1;
	BlendSets.Height >>= 1;
	BlendSets.Height <<= 1;

	if( Settings.Format == Settings.BlendUsingSource )
	{
		BlendSets.Format = ( AVPixelFormat ) sP->format;
	}

	SetStartPosition( Settings.StartX, Settings.StartY );

	MediaPicInfo SaveSets;
	MediaPicInfo PutSets;
	MediaPicInfo InterSets;

	SaveSets.Width = sP->width;
	SaveSets.Height = sP->height;
	SaveSets.Format = ( AVPixelFormat ) sP->format;

	PutSets.Width = BlendSets.Width;
	PutSets.Height = BlendSets.Height;
	PutSets.Format = ( AVPixelFormat ) dP->format;


	InterSets.Width = BlendSets.Width;
	InterSets.Height = BlendSets.Height;
	InterSets.Format = ( AVPixelFormat ) BlendSets.Format;

	InterFrame.Init( InterSets );

	ConvertSave.Init( SaveSets, InterSets, SWS_BILINEAR );
	ConvertPut.Init( InterSets, PutSets, SWS_BILINEAR );
}

void KinMedia::MediaStreamBlend::SetFrame( const MediaFrame & frame )
{
	std::lock_guard<std::mutex> lg( mtx );
	ConvertSave.Convert( frame->data, frame->linesize, 0, frame->height, InterFrame->data, InterFrame->linesize );
}

void KinMedia::MediaStreamBlend::PutFrame( const MediaFrame & frame )
{
	std::lock_guard<std::mutex> lg( mtx );

	uint8_t *PicBuffer[ AV_NUM_DATA_POINTERS ];

	const auto &PixInfo = av_pix_fmt_desc_get( ( AVPixelFormat ) frame->format );

	for( size_t i = 0; frame->data[ i ]; i++ )
	{
		PicBuffer[ i ] = frame->data[ i ] + ( BlendSets.StartX * PixInfo->comp[ i ].step ) + ( frame->linesize[ i ] * BlendSets.StartY );
	}
	ConvertPut.Convert( InterFrame->data, InterFrame->linesize, 0, InterFrame->height, PicBuffer, frame->linesize );
}

void KinMedia::MediaStreamBlend::SetStartPosition( int X, int Y )
{
	Must( ( X + InterFrame->width ) < MaxX, "StartX To large" );
	Must( ( Y + InterFrame->height ) < MaxY, "StartX To large" );

	if( BlendSets.Relative == BlendSets.LeftBottom )
	{
		BlendSets.StartX = X;
		BlendSets.StartY = MaxY - BlendSets.Height - Y;
	}
	else if( BlendSets.Relative == BlendSets.RightTop )
	{
		BlendSets.StartX = MaxX - BlendSets.Width - X;
		BlendSets.StartY = Y;
	}
	else if( BlendSets.Relative == BlendSets.RightBottom )
	{
		BlendSets.StartX = MaxX - BlendSets.Width - X;
		BlendSets.StartY = MaxY - BlendSets.Height - Y;
	}
	else
	{
		BlendSets.StartX = X;
		BlendSets.StartY = Y;
	}
}
