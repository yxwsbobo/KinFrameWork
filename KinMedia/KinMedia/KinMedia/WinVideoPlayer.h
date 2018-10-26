#pragma once
#include "MediaPlayerBase.h"
#include "MediaPicConvert.h"
#include <SDL/SDL.h>
#include <mutex>

namespace KinMedia
{
	class WinVideoPlayer :public MediaPlayerBase
	{
	public:
		WinVideoPlayer();
		~WinVideoPlayer();

		virtual void SetInPara(void* Para) override;

	protected:
		void InitFun(const AVCodecParameters *Para) override;
		void ShowFrame(std::shared_ptr<MediaFrame> Frame) override;
		std::shared_ptr<MediaFrame> Enqueue(const MediaFrame &Frame) override;

	private:
		SDL_Window * screen = 0;
		SDL_Renderer* sdlRenderer = 0;
		SDL_Texture* sdlTexture = 0;

		std::mutex ResizeMutex;
		void* WindowId = nullptr;
		SDL_Rect DisplayRect;
		std::atomic<int> ThreadFlag = 0;

		MediaPicConvert PicConvert;
		MediaPicInfo DisplayPicInfo;

	private:
		static void Run(WinVideoPlayer* player, std::atomic<int>& flag);
		void InitWindow();
		void ComputeDisplayRect();
	};
}

