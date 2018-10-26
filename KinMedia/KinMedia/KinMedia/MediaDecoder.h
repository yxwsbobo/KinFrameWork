#pragma once
#include "FFmpegInclude.h"
#include "MediaCodeCtx.h"

namespace KinMedia
{

	using MediaInputStream = class MediaInputStream;
	class MediaDecoder : public MediaCodeCtx
	{
	public:
		friend MediaInputStream;
		MediaDecoder() = default;
		~MediaDecoder() = default;

		void Init( const AVStream& stream );
		void Init( const AVCodecParameters& codePara );

	private:
		void VideoInit( const AVStream& stream );
		void AudioInit( const AVStream& stream );

		void VideoInit( const  AVCodecParameters& codePara );
		void AudioInit( const  AVCodecParameters& codePara );
	};
}

