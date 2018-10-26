#pragma once
#include <memory>
#include <vector>
#include "FFmpegInclude.h"
#include "MediaPacket.h"
#include <KinBase/CallBackFunction.h>

namespace KinMedia
{

	using MediaInputStream = class MediaInputStream;
	class MediaInputFile
	{
	public:
		explicit MediaInputFile( const char* FileName, const char* TypeName = NULL );
		explicit MediaInputFile( const char* FileName, const char* TypeName, AVDictionary** Diction );
		//explicit MediaInputFile( void* hWndId );
		MediaInputFile( std::function<int( uint8_t*, int )> ReadFun, int BufferSize );
		MediaInputFile( const char* x, const char* y, const char* wh );
		~MediaInputFile();

		KinMedia::MediaInputStream GetVideoStream( int Index = 0 );
		KinMedia::MediaInputStream GetAudioStream( int Index = 0 );

		const MediaPacket& GetSinglePacket();

		bool TravelPacket();
		void ConnectTo( MediaInputStream& inStream );
		int64_t GetTotalFrame();
		int64_t GetTotalDuration();

		const AVFormatContext* operator->() const noexcept
		{
			return Pointer.get();
		}
		AVFormatContext* operator->() noexcept
		{
			return Pointer.get();
		}

		std::shared_ptr<AVFormatContext> Get()
		{
			return Pointer;
		}

	public:
		CallBackFunctions<void( const MediaPacket& )> RecivePacketNotify;

	private:
		void Init( const char* FileName, const char* TypeName = NULL, AVDictionary** param = NULL );

	private:
		KinMedia::MediaInputStream GetStream( int Index, AVMediaType type );
		static void Deleter( AVFormatContext* &formatCtx );
		std::shared_ptr<AVFormatContext> Pointer;

		MediaPacket InterPacket;

		unsigned char* Buffer = NULL;

		std::function<int( uint8_t*, int )> HandleRead;

		static int read_buffer( void *opaque, uint8_t *buf, int buf_size );


	};
}

