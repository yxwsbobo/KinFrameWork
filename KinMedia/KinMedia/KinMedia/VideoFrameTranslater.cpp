#include "VideoFrameTranslater.h"
#include "MediaInputStream.h"
#include "MediaOutputStream.h"
#include <KinBase/KinBase.h>


KinMedia::VideoFrameTranslater::VideoFrameTranslater( const MediaInputStream & sStream, const MediaOutputStream & dStream )
	: MediaFrameTranslater( sStream, dStream )
{
	auto sP = Source->codecpar;
	auto dP = Dest->codecpar;

	DestIncrement = ( int64_t ) av_q2d( av_div_q( av_inv_q( Dest->time_base ), Dest->avg_frame_rate ) );
	HalfDestIncrement = ( int ) ( DestIncrement >> 1 );
	Ratio = av_div_q( Source->time_base, Dest->time_base );


	MediaPicInfo Source;
	MediaPicInfo Dest;
	Source.Width = sP->width;
	Source.Height = sP->height;
	Source.Format = ( AVPixelFormat ) sP->format;
	Dest.Width = dP->width;
	Dest.Height = dP->height;
	Dest.Format = ( AVPixelFormat ) dP->format;
	PicConvert.Init( Source, Dest, SWS_BILINEAR );
}


void KinMedia::VideoFrameTranslater::Translate( const MediaFrame & sFrame )
{
	PicConvert.Convert( sFrame->data, sFrame->linesize, 0, sFrame->height, InterFrame->data, InterFrame->linesize );
	auto SourcePts = sFrame.GetBestPts() - Source.GetStartPts();
	SourcePts = ( SourcePts * Ratio.num ) / Ratio.den;
	auto DestPts = Dest.GetLastDuration();

	auto TempPts = SourcePts + HalfDestIncrement - DestPts;

	if( TempPts > 0 )
	{
		InterFrame->pts = DestPts;
		do
		{
			InterFrame->pts += DestIncrement;
		} while( ( TempPts -= DestIncrement ) > 0 );

		InterFrame.TimeArrive = true;
	}
	else
	{
		InterFrame->pts = Dest.GetLastPts();
		InterFrame.TimeArrive = false;
	}
	TranslateNotify( InterFrame );
}