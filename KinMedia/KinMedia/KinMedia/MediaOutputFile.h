#pragma once
#include <KinBase/CallBackFunction.h>
#include <memory>
#include "FFmpegInclude.h"
#include "MediaStruct.h"
#include <mutex>

namespace KinMedia
{

	using MediaOutputStream = class MediaOutputStream;
	using MediaPacket = class MediaPacket;
	class MediaOutputFile
	{
	public:
		MediaOutputFile( const char* FileName );
		MediaOutputFile( std::function<int( uint8_t*, int )> WriteFun, int BufferSize, const char* Name = "HEVC" );
		~MediaOutputFile();

		MediaOutputStream AllocStream( VideoStreamInfo info = VideoStreamInfo() );
		MediaOutputStream AllocStream( AudioStreamInfo info = AudioStreamInfo() );

		void WritePacket( MediaPacket&pkt );
		void WriteOrgPacket(AVPacket* pkt);

		void WriteHeader();
		void WriteTrailer();

		const AVFormatContext* operator->() const noexcept
		{
			return Pointer.get();
		}
		AVFormatContext* operator->() noexcept
		{
			return Pointer.get();
		}

		const AVFormatContext* Get() const noexcept
		{
			return Pointer.get();
		}
		AVFormatContext* Get() noexcept
		{
			return Pointer.get();
		}
		void SetSeekFun( std::function<int64_t( int64_t, int )> Fun );

	private:
		static void Deleter( AVFormatContext* &formatCtx );
		static void Deleter2( AVFormatContext* &formatCtx );

		std::shared_ptr<AVFormatContext> Pointer;
		std::mutex WriteMutex;
		unsigned char* Buffer = NULL;

		std::function<int( uint8_t*, int )> HandleWrite;
		std::function<int64_t( int64_t, int )> Seek_Handle;

		static int write_buffer( void *opaque, uint8_t *buf, int buf_size );
		static int64_t seek_buffer( void *opaque, int64_t offset, int whence );
	};
}

