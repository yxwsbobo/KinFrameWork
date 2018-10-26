//
// Created by Kin on 2017/11/4 0004.
//

#include "WinVideoPlayer.h"
#include <KinBase/KinBase.h>

KinMedia::WinVideoPlayer::WinVideoPlayer()
	:MediaPlayerBase{ 180 }
{
	std::thread(Run, this, std::ref(ThreadFlag)).detach();
}

KinMedia::WinVideoPlayer::~WinVideoPlayer()
{
	ThreadFlag = 0;
	if (sdlRenderer != NULL)
	{
		SDL_DestroyRenderer(sdlRenderer);
		sdlRenderer = NULL;
	}

	if (sdlTexture != NULL)
	{
		SDL_DestroyTexture(sdlTexture);
		sdlTexture = NULL;
	}
}

void KinMedia::WinVideoPlayer::SetInPara(void * Para)
{
	WindowId = Para;
}

void KinMedia::WinVideoPlayer::InitFun(const AVCodecParameters * Para)
{
	std::lock_guard<std::mutex> lck(ResizeMutex);

	MediaPicInfo Source;
	Source.Width = Para->width;
	Source.Height = Para->height;
	Source.Format = (AVPixelFormat)Para->format;

	DisplayPicInfo.Width = Source.Width;
	DisplayPicInfo.Height = Source.Height;
	DisplayPicInfo.Format = AV_PIX_FMT_YUV420P;

	PicConvert.Init(Source, DisplayPicInfo, SWS_BILINEAR);

	InitWindow();
}

void KinMedia::WinVideoPlayer::ShowFrame(std::shared_ptr<MediaFrame> Frame)
{
	std::lock_guard<std::mutex> lck(ResizeMutex);
	SDL_UpdateTexture(sdlTexture, NULL, (*Frame)->data[0], (*Frame)->linesize[0]);
	av_freep(&(*Frame)->data[0]);
	SDL_RenderClear(sdlRenderer);
	SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &DisplayRect);
	SDL_RenderPresent(sdlRenderer);
}

std::shared_ptr<KinMedia::MediaFrame> KinMedia::WinVideoPlayer::Enqueue(const MediaFrame & Frame)
{
	std::shared_ptr<MediaFrame> TempFrame = std::make_shared<MediaFrame>();
	av_image_alloc((*TempFrame)->data, (*TempFrame)->linesize, DisplayPicInfo.Width, DisplayPicInfo.Height, DisplayPicInfo.Format, 1);

	PicConvert.Convert(Frame->data, Frame->linesize, 0, Frame->height, (*TempFrame)->data,
		(*TempFrame)->linesize);
	TempFrame->Get()->pts = Frame->pts;
	TempFrame->Get()->pkt_dts = Frame->pkt_dts;

	return TempFrame;
}



void KinMedia::WinVideoPlayer::Run(WinVideoPlayer * player, std::atomic<int>& flag)
{
	SDL_Event Sevent;
	flag = 1;
	while (flag)
	{
		SDL_WaitEvent(&Sevent);
		switch (Sevent.type)
		{
		case SDL_WINDOWEVENT:
		{
			if (Sevent.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				if (player->isRunning())
				{
					player->InitWindow();
				}
			}
			break;
		}
		default:
			break;
		}
	}
}

void KinMedia::WinVideoPlayer::InitWindow()
{

	if (screen == NULL)
	{
		if (WindowId == nullptr)
		{
			screen = SDL_CreateWindow("OnLineSchool", 50, 50, 800, 600, SDL_WINDOW_OPENGL);
		}
		else
		{
			Must(screen = SDL_CreateWindowFrom(WindowId), "SDL_CreateWindow Error");
		}
	}

	if (sdlRenderer != NULL)
	{
		SDL_DestroyRenderer(sdlRenderer);
		sdlRenderer = NULL;
	}

	if (sdlTexture != NULL)
	{
		SDL_DestroyTexture(sdlTexture);
		sdlTexture = NULL;
	}

	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
	sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,
		DisplayPicInfo.Width, DisplayPicInfo.Height);
	ComputeDisplayRect();
}

void KinMedia::WinVideoPlayer::ComputeDisplayRect()
{
	auto whRatio = (float)DisplayPicInfo.Width / (float)DisplayPicInfo.Height;
	int Width, Height;

	int wWindow, hWindow;
	SDL_GetWindowSize(screen, &wWindow, &hWindow);

	Height = hWindow;
	Width = lrint(Height * whRatio) & ~1;
	if (Width >wWindow)
	{
		Width = wWindow;
		Height = lrint(Width / whRatio) & ~1;
	}

	DisplayRect.x = (wWindow - Width) / 2;
	DisplayRect.y = (hWindow - Height) / 2;
	DisplayRect.w = std::max(Width, 1);
	DisplayRect.h = std::max(Height, 1);
}
