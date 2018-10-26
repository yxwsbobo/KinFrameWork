#include "MediaInputFile.h"
#include <KinBase/KinBase.h>
#include "MediaInputStream.h"
#include <algorithm>
#include <functional>
//#include "MediaStrogGdi.h"

//constexpr char* MyTempTitle = "TempTitle83CD1AC5-46F8-BADE-E55C7290FCE2";
//constexpr wchar_t* MyTempTitleL = L"TempTitle83CD1AC5-46F8-BADE-E55C7290FCE2";
//constexpr char* MyTempTitlePut = "title=TempTitle83CD1AC5-46F8-BADE-E55C7290FCE2";

KinMedia::MediaInputFile::MediaInputFile( const char * FileName, const char * TypeName )
{
	Init( FileName, TypeName );
}

KinMedia::MediaInputFile::MediaInputFile( const char * FileName, const char * TypeName, AVDictionary ** Diction )
{
	Init( FileName, TypeName, Diction );
}

//KinMedia::MediaInputFile::MediaInputFile( void * hWndId )
//{
//	MediaStrogGdi::HookFindWindow( hWndId, MyTempTitle );
//	MediaStrogGdi::HookFindWindow( hWndId, MyTempTitleL );
//	Init( MyTempTitlePut, "gdigrab" );
//}

KinMedia::MediaInputFile::MediaInputFile( std::function<int( uint8_t*, int )> ReadFun, int BufferSize )
	:HandleRead( ReadFun )
{
	Buffer = ( unsigned char* ) av_malloc( BufferSize );
	AVIOContext *avio = avio_alloc_context( Buffer, BufferSize, 0, this, read_buffer, NULL, NULL );
	Maybe( avio, "avio_alloc_context failed" );

	AVFormatContext* Result = avformat_alloc_context();
	Result->pb = avio;
	Result->flags = AVFMT_FLAG_CUSTOM_IO;

	Maybe( avformat_open_input( &Result, "any", NULL, NULL ) == 0, "avformat_open_input Error" );
	//avformat_open_input( &Result, "any", NULL, NULL );

	Maybe( avformat_find_stream_info( Result, NULL ) >= 0, "Couldn't find stream information." );

	Pointer.reset( Result, Deleter );
}

KinMedia::MediaInputFile::MediaInputFile( const char * x, const char * y, const char * wh )
{
	AVDictionary* options = NULL;

	av_dict_set( &options, "offset_x", x, 0 );

	av_dict_set( &options, "offset_y", y, 0 );

	av_dict_set( &options, "video_size", wh, 0 );
	Init( u8"desktop", "gdigrab", &options );
	av_dict_free( &options );
}

KinMedia::MediaInputFile::~MediaInputFile()
{
	//av_freep( &Buffer );
}


KinMedia::MediaInputStream KinMedia::MediaInputFile::GetVideoStream( int Index )
{
	return GetStream( Index, AVMEDIA_TYPE_VIDEO );
}

KinMedia::MediaInputStream KinMedia::MediaInputFile::GetAudioStream( int Index )
{
	return GetStream( Index, AVMEDIA_TYPE_AUDIO );
}

const KinMedia::MediaPacket & KinMedia::MediaInputFile::GetSinglePacket()
{
	av_packet_unref(InterPacket.Get());
	av_read_frame(Pointer.get(), InterPacket.Get());
	return InterPacket;
}

bool KinMedia::MediaInputFile::TravelPacket()
{
	av_packet_unref( InterPacket.Get() );
	auto Result = av_read_frame( Pointer.get(), InterPacket.Get() );
	if( Result == 0 )
	{
		RecivePacketNotify( InterPacket );
		return true;
	}
	return false;
}

void KinMedia::MediaInputFile::ConnectTo( MediaInputStream & inStream )
{
	RecivePacketNotify.Add( std::bind( &MediaInputStream::DecodePacket, &inStream, std::placeholders::_1 ) );
}

int64_t KinMedia::MediaInputFile::GetTotalFrame()
{
	int64_t CurrentCnt = 0;

	for( unsigned int i = 0; i < Pointer->nb_streams; i++ )
	{
		CurrentCnt += Pointer->streams[ i ]->nb_frames;
	}

	return CurrentCnt;
}

int64_t KinMedia::MediaInputFile::GetTotalDuration()
{
	if (Pointer->duration != AV_NOPTS_VALUE) {
		return Pointer->duration / (AV_TIME_BASE/1000);
	}
	return 0;
}


void KinMedia::MediaInputFile::Init( const char * FileName, const char * TypeName, AVDictionary** param )
{
	AVInputFormat* format = NULL;
	if( TypeName != NULL )
	{
		format = av_find_input_format( TypeName );
		Maybe( format, "Can't Find Input Format" );
	}

	AVFormatContext* Result = NULL;
	auto code = avformat_open_input(&Result, FileName, format, param);
	Maybe(  code == 0, "avformat_open_input Error" );

	Maybe( avformat_find_stream_info( Result, NULL ) >= 0, "Couldn't find stream information." );

	Pointer.reset( Result, Deleter );
}

KinMedia::MediaInputStream KinMedia::MediaInputFile::GetStream( int Index, AVMediaType type )
{
	auto &inCtx = Pointer;

	for( unsigned int i = Index; i < inCtx->nb_streams; i++ )
	{
		if( inCtx->streams[ i ]->codecpar->codec_type == type )
		{
			return MediaInputStream( inCtx->streams[ i ] );
		}
	}
	Must( 0, "Not FoundStream" );
	//return MediaInputStream();
}

void KinMedia::MediaInputFile::Deleter( AVFormatContext *& formatCtx )
{
	avformat_close_input( &formatCtx );
}

int KinMedia::MediaInputFile::read_buffer( void * opaque, uint8_t * buf, int buf_size )
{
	auto me = ( MediaInputFile* ) opaque;

	return me->HandleRead( buf, buf_size );
}

