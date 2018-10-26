#include "AudioFrameTranslater.h"
#include "MediaInputStream.h"
#include "MediaOutputStream.h"
#include <KinBase/KinBase.h>

KinMedia::AudioFrameTranslater::~AudioFrameTranslater()
{
	av_freep( ConvertedSamples );
	av_freep( &ConvertedSamples );
}

KinMedia::AudioFrameTranslater::AudioFrameTranslater( const MediaInputStream & sStream, const MediaOutputStream & dStream )
	: MediaFrameTranslater( sStream, dStream )
{
	auto sP = Source->codecpar;
	auto dP = Dest->codecpar;

	Ratio = { dP->sample_rate, sP->sample_rate };
	DestIncrement = ( dP->sample_rate * dP->frame_size ) / sP->sample_rate;
	HalfDestIncrement = ( int ) ( DestIncrement >> 1 );

	//SwrCtx Init
	auto SwrCtx = swr_alloc_set_opts( NULL, dP->channel_layout, ( AVSampleFormat ) dP->format, dP->sample_rate,
		av_get_default_channel_layout( sP->channels ), ( AVSampleFormat ) sP->format, sP->sample_rate, 0, 0 );
	Maybe( SwrCtx, "swr_alloc_set_opts Error" );
	Maybe( swr_init( SwrCtx ) == 0, "swr_init Error" );
	Swr.reset( SwrCtx, SwrDeleter );

	//Fifo Init
	auto ff = av_audio_fifo_alloc( ( AVSampleFormat ) dP->format, dP->channels, ( sP->frame_size + dP->frame_size ) * 10 );
	Maybe( ff, "av_audio_fifo_alloc Error" );
	Fifo.reset( ff, FFDeleter );

	//Buffer Init
	Maybe( av_samples_alloc_array_and_samples( &ConvertedSamples, NULL, dP->channels, sP->frame_size + dP->frame_size,
		( AVSampleFormat ) dP->format, 0 ) >= 0, "av_samples_alloc_array_and_samples Error" );
}



void KinMedia::AudioFrameTranslater::SwrDeleter( SwrContext *& swr )
{
	swr_free( &swr );
}

void KinMedia::AudioFrameTranslater::FFDeleter( AVAudioFifo * ff )
{
	av_audio_fifo_free( ff );
}

void KinMedia::AudioFrameTranslater::Translate( const MediaFrame & sFrame )
{
	if( ConvertStoreFrame( sFrame ) )
	{
		PickFrameDispatch( sFrame );
	}
}

bool KinMedia::AudioFrameTranslater::ConvertStoreFrame( const MediaFrame & sFrame )
{
	auto FF = Fifo.get();
	auto SourceNbSamples = sFrame->nb_samples;

	Maybe( swr_convert( Swr.get(), ConvertedSamples, SourceNbSamples, ( const uint8_t ** ) sFrame->extended_data,
		SourceNbSamples ) >= 0, "swr_convert Error" );

	//Maby need av_audio_fifo_realloc
	Maybe( av_audio_fifo_write( FF, ( void** ) ConvertedSamples, SourceNbSamples ) == SourceNbSamples, "av_audio_fifo_write Error" );

	if( av_audio_fifo_size( FF ) < Dest->codecpar->frame_size )
	{
		return false;
	}
	return true;
}

void KinMedia::AudioFrameTranslater::PickFrameDispatch( const MediaFrame & sFrame )
{
	auto FF = Fifo.get();
	auto DestFrameSize = Dest->codecpar->frame_size;

	//Output Frame
	while( av_audio_fifo_size( FF ) >= DestFrameSize )
	{
		if( av_audio_fifo_read( FF, ( void ** ) InterFrame->data, DestFrameSize ) < DestFrameSize )
		{
			int a = 5;
			return;
		}

		//InterFrame->pkt_pts = InterFrame->pkt_dts = InterFrame->pts = Dest.GetLastDuration( ) + DestFrameSize ;
		InterFrame->pkt_dts = InterFrame->pts = cnt;
		cnt += DestFrameSize;
		InterFrame.TimeArrive = true;
		//auto SourcePts = ( sFrame->pts * Ratio.num ) / Ratio.den;
		//auto DestPts = Dest.GetLastDuration( );

		//auto TempPts = SourcePts - DestPts;

		//if( TempPts >=0 )
		//{
		//	InterFrame->pts = DestPts;
		//	do
		//	{
		//		InterFrame->pts += DestFrameSize;
		//	} while( ( TempPts -= DestFrameSize ) > 0 );

		//	//Need Fix it in futer
		//	//InterFrame->pts += DestIncrement;

		//	InterFrame.TimeArrive = true;
		//}
		//else
		//{
		//	InterFrame->pts = Dest.GetLastPts( );
		//	InterFrame.TimeArrive = false;
		//}


		//here
		//InterFrame->pts = ( sFrame->pts * Ratio.num ) / Ratio.den;


		//Need To Fix if SourcePts Duration Larger than Dest pts double or more
		//InterFrame->pts = pts;
		//pts += 1024;



		//InterFrame->pts = Dest.GetLastPts( ) + DestIncrement;
		//InterFrame->pts = sFrame->pts;
		//InterFrame.TimeArrive = true;
		TranslateNotify( InterFrame );
	}

}
