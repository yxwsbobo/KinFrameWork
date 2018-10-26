#pragma once
#include <memory>
#include "FFmpegInclude.h"
#include "MediaDecoder.h"
#include "MediaStream.h"
#include <KinBase/CallBackFunction.h>
#include "MediaFrame.h"
#include <vector>
#include "MediaStruct.h"

namespace KinMedia
{

	using MediaFrameTranslater = class MediaFrameTranslater;
	using MediaInputFile = class MediaInputFile;
	using MediaStreamBlend = class MediaStreamBlend;
	using MediaOutputStream = class MediaOutputStream;
	class MediaInputStream : public MediaStream
	{

	public:
		void DecodePacket( const MediaPacket& pkt );
		MediaDecoder& GetDecoder() noexcept;

		std::shared_ptr<MediaFrameTranslater> GetFrameTranslate( const MediaOutputStream& stream );

		void ConnectTo( std::shared_ptr<MediaFrameTranslater> & Dest );
		void ConnectTo( MediaInputStream & Dest, StreamBlendSetting Settings = StreamBlendSetting() );

		~MediaInputStream() = default;
	public:
		CallBackFunctions<void( const MediaFrame& )> DecodeNotify;

	private:
		friend MediaInputFile;
		MediaInputStream() = default;
		MediaInputStream( AVStream* Stream );


	private:
		MediaDecoder Decoder;
		MediaFrame InterFrame;
		std::vector<std::shared_ptr<MediaStreamBlend>> StreamBlend;

	private:
		static void Deleter( AVStream* stream );
	};
}

