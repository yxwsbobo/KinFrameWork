#pragma once
#include "FFmpegInclude.h"
#include <mutex>
#include "MediaFrame.h"
#include "MediaPicConvert.h"
#include "MediaStruct.h"

namespace KinMedia
{

	using MediaInputStream = class MediaInputStream;
	class MediaStreamBlend
	{
	public:
		MediaStreamBlend( MediaInputStream& Source, MediaInputStream& Dest, const StreamBlendSetting& Settings );
		~MediaStreamBlend() = default;

		void SetFrame( const MediaFrame& frame );
		void PutFrame( const MediaFrame& frame );

		void SetStartPosition( int X, int Y );
	private:
		std::mutex mtx;

		MediaFrame InterFrame;

		MediaPicConvert ConvertSave;
		MediaPicConvert ConvertPut;

		StreamBlendSetting BlendSets;

		int MaxX;
		int MaxY;
	};
}

