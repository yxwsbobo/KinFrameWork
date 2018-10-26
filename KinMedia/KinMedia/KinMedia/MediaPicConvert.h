#pragma once
#include "FFmpegInclude.h"
#include <memory>
#include "MediaStruct.h"
namespace KinMedia
{

	class MediaPicConvert
	{
	public:
		MediaPicConvert() = default;
		MediaPicConvert( MediaPicInfo& Source, MediaPicInfo& Dest, int flags = SWS_FAST_BILINEAR );
		~MediaPicConvert() = default;

		void Init( MediaPicInfo& Source, MediaPicInfo& Dest, int flags = SWS_FAST_BILINEAR );

		int Convert( const uint8_t *const sBuffer[ ], const int sStride[ ], int sSliceY, int srcSliceH, uint8_t *const dBuffer[ ], const int dStride[ ] );
	private:
		std::shared_ptr<SwsContext> Sws;

		static void Deleter( SwsContext*& sws );
	};
}

