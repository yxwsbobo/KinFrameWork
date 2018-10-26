#pragma once
#include "MediaFrameTranslater.h"
#include "MediaPicConvert.h"

namespace KinMedia
{

	class VideoFrameTranslater :
		public MediaFrameTranslater
	{
	public:
		virtual ~VideoFrameTranslater() = default;

		VideoFrameTranslater( const MediaInputStream&sStream, const MediaOutputStream& dStream );

	private:

		MediaPicConvert PicConvert;

		virtual void Translate( const MediaFrame & sFrame ) override;
	};
}

