#pragma once
#include "MediaFrameTranslater.h"

namespace KinMedia
{

	class AudioFrameTranslater : public MediaFrameTranslater
	{
	public:
		virtual ~AudioFrameTranslater();

		AudioFrameTranslater( const MediaInputStream&sStream, const MediaOutputStream& dStream );

	private:
		static void SwrDeleter( SwrContext*& swr );
		static void FFDeleter( AVAudioFifo* ff );
		std::shared_ptr<SwrContext> Swr;
		std::shared_ptr<AVAudioFifo> Fifo;
		uint8_t** ConvertedSamples;

		virtual void Translate( const MediaFrame & sFrame ) override;

		bool ConvertStoreFrame( const MediaFrame & sFrame );
		void PickFrameDispatch( const MediaFrame & sFrame );

		int cnt = 0;
	};
}

