#pragma once
#include <memory>
#include "FFmpegInclude.h"
#include <KinBase/CallBackFunction.h>
#include "MediaFrame.h"

namespace KinMedia
{

	using MediaOutputStream = class MediaOutputStream;
	using MediaInputStream = class MediaInputStream;
	class MediaFrameTranslater
	{
	public:
		virtual void Translate( const MediaFrame& ) = 0;
		void ConnectTo( MediaOutputStream& outStream );

		CallBackFunctions<void( const MediaFrame& )> TranslateNotify;

		virtual ~MediaFrameTranslater() = default;
	protected:
		MediaFrameTranslater( const MediaInputStream&sStream, const MediaOutputStream& dStream );

		MediaFrame InterFrame;
		const MediaInputStream& Source;
		const MediaOutputStream& Dest;

		AVRational Ratio;
		int HalfDestIncrement;
		int64_t DestIncrement;
	};
}

