#include "MediaEncoder.h"
#include <KinBase/KinBase.h>


void KinMedia::MediaEncoder::Init( const AVStream& stream, const VideoStreamInfo& info )
{
	auto Encoder = avcodec_find_encoder( info.CodeId );
	Maybe( Encoder, "Can't find Encoder" );
	auto Ctx = avcodec_alloc_context3( Encoder );
	Maybe( Ctx, "Can't Alloc CodecCtx" );

	if( info.FileFormat->flags & AVFMT_GLOBALHEADER )
	{
		Ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}

	Ctx->time_base = av_inv_q( info.FrameRate );

	Ctx->width = info.Width;
	Ctx->height = info.Height;
	Ctx->width >>= 1;
	Ctx->width <<= 1;
	Ctx->height >>= 1;
	Ctx->height <<= 1;
	Ctx->codec_type = info.CodecType;
	Ctx->pix_fmt = info.PixFmt;
	Ctx->bit_rate = info.BitRate;
	Ctx->bit_rate_tolerance = ( int ) ( Ctx->bit_rate / 2 );

	Ctx->gop_size = info.GopSize;

	Ctx->qmin = info.qMin;
	Ctx->qmax = info.qMax;

	Ctx->max_b_frames = info.MaxBFrame;

	if( Ctx->codec_id == AV_CODEC_ID_MPEG1VIDEO )
	{
		Ctx->mb_decision = 2;
	}
	AVDictionary *param = 0;
	if( Ctx->codec_id == AV_CODEC_ID_H264 )
	{
		av_dict_set( &param, "preset", "slow", 0 );
		av_dict_set( &param, "tune", "zerolatency", 0 );
	}
	if( Ctx->codec_id == AV_CODEC_ID_H265 )
	{
		av_dict_set( &param, "preset", "ultrafast", 0 );
		av_dict_set( &param, "tune", "zero-latency", 0 );
	}
	if( Ctx->codec_id == AV_CODEC_ID_VP9 )
	{
		av_dict_set( &param, "qscale", "3", 0 );
		av_dict_set( &param, "speed", "1", 0 );
	}

	Maybe( avcodec_open2( Ctx, Encoder, &param ) == 0, "Can't Open Avcodec" );
	av_dict_free( &param );

	Pointer.reset( Ctx, Deleter );


	Maybe( avcodec_parameters_from_context( stream.codecpar, Ctx ) >= 0, "avcodec_parameters_from_context error" );
#pragma warning(suppress:4996)
	Maybe( avcodec_parameters_to_context( stream.codec, stream.codecpar ) >= 0, "avcodec_parameters_to_context error" );
}

void KinMedia::MediaEncoder::Init( const AVStream& stream, const AudioStreamInfo& info )
{
	auto Encoder = avcodec_find_encoder( info.CodeId );
	Maybe( Encoder, "Can't find Encoder" );
	auto Ctx = avcodec_alloc_context3( Encoder );
	Maybe( Ctx, "Can't Alloc CodecCtx" );

	if( info.FileFormat->flags & AVFMT_GLOBALHEADER )
	{
		Ctx->flags |= CODEC_FLAG_GLOBAL_HEADER;
	}

	//Maby Need Check Fmt And Others
	Ctx->sample_fmt = Encoder->sample_fmts[ 0 ];
	Ctx->sample_rate = info.SampleRate;
	Ctx->channels = info.Channels;
	Ctx->channel_layout = av_get_default_channel_layout( Ctx->channels );
	Ctx->bit_rate = info.BitRate;

	Ctx->time_base = { 1, Ctx->sample_rate };
	Ctx->strict_std_compliance = FF_COMPLIANCE_EXPERIMENTAL;

	Maybe( avcodec_open2( Ctx, Encoder, NULL ) == 0, "Can't Open Avcodec" );

	Pointer.reset( Ctx, Deleter );

	Maybe( avcodec_parameters_from_context( stream.codecpar, Ctx ) >= 0, "avcodec_parameters_from_context error" );
}
