#pragma once
#include "MediaCodeCtx.h"
#include "MediaStruct.h"

namespace KinMedia
{
	using MediaOutputStream = class MediaOutputStream;
	class MediaEncoder : public MediaCodeCtx
	{
	private:
		friend MediaOutputStream;
		MediaEncoder() = default;
		~MediaEncoder() = default;

		void Init( const AVStream& stream, const VideoStreamInfo& info );
		void Init( const AVStream& stream, const AudioStreamInfo& info );
	};
}

