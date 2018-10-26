#include "MediaOutputFile.h"
#include <KinBase/KinBase.h>
#include "MediaPacket.h"
#include "MediaOutputStream.h"
//#include "MediaStrogGdi.h"

KinMedia::MediaOutputFile::MediaOutputFile( const char * FileName )
{
	AVFormatContext* Result = NULL;

	Maybe( avformat_alloc_output_context2( &Result, 0, 0, FileName ) >= 0, "avformat_alloc_output_context2 Error" );
	Maybe( avio_open( &Result->pb, FileName, AVIO_FLAG_READ_WRITE ) >= 0, "Failed To Open OutPut File" );

	Pointer.reset( Result, Deleter );
}

KinMedia::MediaOutputFile::MediaOutputFile( std::function<int( uint8_t*, int )> WriteFun, int BufferSize, const char* Name )
	:HandleWrite{ WriteFun }
{
	Buffer = ( unsigned char* ) av_malloc( BufferSize );
	AVFormatContext* Result = NULL;
	auto fmt = av_guess_format( Name, 0, NULL );
	Maybe( avformat_alloc_output_context2( &Result, fmt, 0, 0 ) >= 0, "avformat_alloc_output_context2 Error" );
	AVIOContext *avio = avio_alloc_context( Buffer, BufferSize, 1, this, NULL, write_buffer, seek_buffer );
	Maybe( avio, "avio_alloc_context Failed" );

	Result->pb = avio;
	Result->flags = AVFMT_FLAG_CUSTOM_IO;

	Pointer.reset( Result, Deleter2 );
}

KinMedia::MediaOutputFile::~MediaOutputFile()
{
	av_freep( &Buffer );
}

KinMedia::MediaOutputStream KinMedia::MediaOutputFile::AllocStream( VideoStreamInfo info )
{
	info.FileFormat = Pointer->oformat;
	return MediaOutputStream( *this, info );
}

KinMedia::MediaOutputStream KinMedia::MediaOutputFile::AllocStream( AudioStreamInfo info )
{
	info.FileFormat = Pointer->oformat;
	return MediaOutputStream( *this, info );
}

void KinMedia::MediaOutputFile::WritePacket( MediaPacket & pkt )
{
	WriteOrgPacket(pkt.Get());
}

void KinMedia::MediaOutputFile::WriteOrgPacket(AVPacket * pkt)
{
	std::lock_guard<std::mutex> lck(WriteMutex);
	av_write_frame(Pointer.get(), pkt);
}

void KinMedia::MediaOutputFile::WriteHeader()
{
	Must( avformat_write_header( Pointer.get(), NULL ) == 0, "Write_Header Failed" );
}

void KinMedia::MediaOutputFile::WriteTrailer()
{
	av_write_trailer( Pointer.get() );
}

void KinMedia::MediaOutputFile::SetSeekFun( std::function<int64_t( int64_t, int )> Fun )
{
	Seek_Handle = Fun;
}

void KinMedia::MediaOutputFile::Deleter( AVFormatContext *& formatCtx )
{
	avio_closep( &formatCtx->pb );
	avformat_free_context( formatCtx );
}

void KinMedia::MediaOutputFile::Deleter2( AVFormatContext *& formatCtx )
{
	avformat_free_context( formatCtx );
}

int KinMedia::MediaOutputFile::write_buffer( void * opaque, uint8_t * buf, int buf_size )
{
	auto me = ( MediaOutputFile* ) opaque;

	return me->HandleWrite( buf, buf_size );
}

int64_t KinMedia::MediaOutputFile::seek_buffer( void * opaque, int64_t offset, int whence )
{
	auto me = ( MediaOutputFile* ) opaque;

	if( me->Seek_Handle )
	{
		return me->Seek_Handle( offset, whence );
	}
	return -1;
}


