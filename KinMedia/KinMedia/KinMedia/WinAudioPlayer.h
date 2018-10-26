#pragma once
#include "FFmpegInclude.h"
#include "MediaPlayerBase.h"
#include "CircularBuffer.h"
#include <SDL/SDL.h>
#define MAX_AUDIO_FRAME_SIZE 192000

namespace KinMedia
{
	class WinAudioPlayer : public MediaPlayerBase
	{
	public:
		WinAudioPlayer();
		~WinAudioPlayer();

	protected:
		void InitFun(const AVCodecParameters *Para) override;
		void ShowFrame(std::shared_ptr<MediaFrame> Frame) override;
		std::shared_ptr<MediaFrame> Enqueue(const MediaFrame &Frame) override;

	private:
		SDL_AudioSpec AudioInfo;
		CircularBuffer cBuffer{ MAX_AUDIO_FRAME_SIZE*10 };
		std::mutex WriteMutex;
		AVCodecParameters AudioPara;
		std::shared_ptr<SwrContext> Swr;

		int BufferSize;
	private:
		static void  AudioCallBack(void *userdata, Uint8 * stream, int len);
		static void SwrDeleter(SwrContext*& swr);
	};

}

