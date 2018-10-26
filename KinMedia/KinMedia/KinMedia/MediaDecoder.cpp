#include "MediaDecoder.h"
#include <KinBase/KinBase.h>

void KinMedia::MediaDecoder::Init( const AVStream & stream )
{
	if( stream.codecpar->codec_type == AVMEDIA_TYPE_VIDEO )
	{
		VideoInit( stream );
	}
	else if( stream.codecpar->codec_type == AVMEDIA_TYPE_AUDIO )
	{
		AudioInit( stream );
	}
	else
	{
		Must( 0, "Unknown Code type in MediaDecoder " );
	}
}

void KinMedia::MediaDecoder::Init( const AVCodecParameters& codePara )
{
	if( codePara.codec_type == AVMEDIA_TYPE_VIDEO )
	{
		VideoInit( codePara );
	}
	else if( codePara.codec_type == AVMEDIA_TYPE_AUDIO )
	{
		AudioInit( codePara );
	}
	else
	{
		Must( 0, "Unknown Code type in MediaDecoder " );
	}
}

void KinMedia::MediaDecoder::VideoInit( const AVStream & stream )
{
	auto &para = stream.codecpar;
	auto Decoder = avcodec_find_decoder( para->codec_id );
	Maybe( Decoder, "Can't find Decoder" );
	auto Ctx = avcodec_alloc_context3( Decoder );
	Maybe( Ctx, "Can't Alloc CodecCtx" );

#pragma warning(suppress:4996)
	auto &SourceCtx = stream.codec;

	Ctx->time_base = SourceCtx->time_base;
	Ctx->gop_size = SourceCtx->gop_size;
	Ctx->qmax = SourceCtx->qmax;
	Ctx->qmin = SourceCtx->qmin;
	Ctx->max_b_frames = SourceCtx->max_b_frames;
	Ctx->framerate = SourceCtx->framerate;
	Ctx->frame_size = SourceCtx->frame_size;

	Maybe( avcodec_parameters_to_context( Ctx, para ) >= 0, "Can't Get context frome parameters" );

	Maybe( avcodec_open2( Ctx, Decoder, 0 ) == 0, "Couldn't open Codec " );

	Maybe( avcodec_parameters_from_context( para, Ctx ) >= 0, "Can't Get context frome parameters" );
	Pointer.reset( Ctx, Deleter );

}

void KinMedia::MediaDecoder::AudioInit( const AVStream & stream )
{
	auto &para = stream.codecpar;
	//#pragma warning(suppress:4996)
	//auto &SourceCtx = stream.codec;
	//SourceCtx->frame_size = 22050;
	//应真( avcodec_open2( SourceCtx, avcodec_find_decoder(SourceCtx->codec_id), 0 ) == 0, "Couldn't open Codec " );
	//Pointer.reset( SourceCtx, Deleter );
	//应真( avcodec_parameters_from_context( para, SourceCtx ) >= 0, "Can't Get context frome parameters" );
	//return;


	//para->codec_id = AV_CODEC_ID_AAC;
	auto Decoder = avcodec_find_decoder( para->codec_id );
	Maybe( Decoder, "Can't find Decoder" );
	auto Ctx = avcodec_alloc_context3( Decoder );
	Maybe( Ctx, "Can't Alloc CodecCtx" );
	//#pragma warning(suppress:4996)
	//	auto &SourceCtx = stream.codec;
	//
	//	Ctx->flags = SourceCtx->flags;
	//	Ctx->sample_fmt = SourceCtx->sample_fmt;
	//	Ctx->sample_rate = SourceCtx->sample_rate;
	//	Ctx->bit_rate = SourceCtx->bit_rate;
	//
	//	Ctx->channels = SourceCtx->channels;
	//	Ctx->time_base = SourceCtx->time_base;
	//	Ctx->strict_std_compliance = SourceCtx->strict_std_compliance;

	//para->channel_layout = av_get_default_channel_layout( para->channels );
	//para->channel_layout = av_get_default_channel_layout( Ctx->channels );
	//para->channel_layout = SourceCtx->channel_layout;
	//para->channels = SourceCtx->channels;
	if( para->frame_size < 100 )
	{
		para->frame_size = 40000;
	}

	Maybe( avcodec_parameters_to_context( Ctx, para ) >= 0, "Can't Get context frome parameters" );



	Maybe( avcodec_open2( Ctx, Decoder, 0 ) == 0, "Couldn't open Codec " );

	Maybe( avcodec_parameters_from_context( para, Ctx ) >= 0, "Can't Get context frome parameters" );
	Pointer.reset( Ctx, Deleter );

}

void KinMedia::MediaDecoder::VideoInit( const AVCodecParameters & codePara )
{
	AVCodecParameters para = codePara;

	auto Decoder = avcodec_find_decoder( para.codec_id );
	Maybe( Decoder, "Can't find Decoder" );
	auto Ctx = avcodec_alloc_context3( Decoder );
	Maybe( Ctx, "Can't Alloc CodecCtx" );

	auto Code = avcodec_parameters_to_context( Ctx, &para );
	Maybe( Code >= 0, "Can't Get context frome parameters" );

	Maybe( avcodec_open2( Ctx, Decoder, 0 ) == 0, "Couldn't open Codec " );

	Pointer.reset( Ctx, Deleter );
}

void KinMedia::MediaDecoder::AudioInit( const AVCodecParameters & codePara )
{
	AVCodecParameters para = codePara;
	auto Decoder = avcodec_find_decoder( para.codec_id );
	Maybe( Decoder, "Can't find Decoder" );
	auto Ctx = avcodec_alloc_context3( Decoder );
	Maybe( Ctx, "Can't Alloc CodecCtx" );

	if( para.frame_size < 100 )
	{
		para.frame_size = 40000;
	}

	Maybe( avcodec_parameters_to_context( Ctx, &para ) >= 0, "Can't Get context frome parameters" );
	Maybe( avcodec_open2( Ctx, Decoder, 0 ) == 0, "Couldn't open Codec " );
	Pointer.reset( Ctx, Deleter );
}
