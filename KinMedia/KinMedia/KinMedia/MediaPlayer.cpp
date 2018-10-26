#include "MediaPlayer.h"
#include <thread>
#include <KinBase/RunningTC.h>
#include <chrono>


KinMedia::MediaPlayer::MediaPlayer( std::shared_ptr<MediaPlayerBase> vPlayer, std::shared_ptr<MediaPlayerBase> aPlayer )
	:VideoPlayer{ vPlayer }, AudioPlayer{ aPlayer }
{

}

KinMedia::MediaPlayer::~MediaPlayer()
{
	Stop();
}

void KinMedia::MediaPlayer::Play( std::string url )
{
	Stop();
	Path = url;
	RunFlag = 1;

	InFile = std::make_shared<MediaInputFile>(Path.c_str());
	InVideoStream = std::make_shared<MediaInputStream>(InFile->GetVideoStream());
	InAudioStream = std::make_shared<MediaInputStream>(InFile->GetAudioStream());
	InFile->ConnectTo(*InVideoStream);
	InFile->ConnectTo(*InAudioStream);

	InVideoStream->DecodeNotify.Add(std::bind(&MediaPlayer::PushVideoFrame, this, std::placeholders::_1));
	InAudioStream->DecodeNotify.Add(std::bind(&MediaPlayer::PushAudioFrame, this, std::placeholders::_1));

	VideoPlayer->Init((*InVideoStream)->codecpar, (*InVideoStream)->start_time, (*InVideoStream)->time_base);
	AudioPlayer->Init((*InAudioStream)->codecpar, (*InAudioStream)->start_time, (*InAudioStream)->time_base);

	std::thread( std::bind( &MediaPlayer::PlayUrlVideoThread, this ) ).detach();
}

void KinMedia::MediaPlayer::Stop()
{
	RunFlag = 0;
	while( RunningCont != 0 )
	{
		std::this_thread::yield();
	}
	AudioPlayer->Stop();
	VideoPlayer->Stop();
}

void KinMedia::MediaPlayer::Pause()
{
	PauseFlag = 1;
	VideoPlayer->Pause();
	AudioPlayer->Pause();

}

void KinMedia::MediaPlayer::Resume()
{
	PauseFlag = 0;
	VideoPlayer->Resume();
	AudioPlayer->Resume();
}

int64_t KinMedia::MediaPlayer::GetTotalTime()
{
	if (!isPlaying())
	{
		return 0;
	}
	return InFile->GetTotalDuration();
}

int KinMedia::MediaPlayer::GetCurrentPercent()
{
	auto Total = GetTotalTime();
	if (!Total)
	{
		return 0;
	}
	auto CurrentDuration = VideoPlayer->GetCurrentDuration() *1000;

	int Result = (CurrentDuration) / Total;
	Result = std::min(Result, 1000);
	return Result;
}



void KinMedia::MediaPlayer::Seek(int64_t Time)
{
	if (!isPlaying())
	{
		return;
	}
	NeedSeek = 1;
	SeekTimePoint = Time;
	VideoPlayer->CurrentDuration = Time;
}

bool KinMedia::MediaPlayer::isPlaying()
{
	return ( bool ) RunFlag;
}

void KinMedia::MediaPlayer::PushVideoFrame( const MediaFrame & Frame )
{
	if (!SeekOverFlag)
	{
		VideoPlayer->Play(Frame);
	}
}

void KinMedia::MediaPlayer::PushAudioFrame( const MediaFrame & Frame )
{
	if (!SeekOverFlag)
	{
		AudioPlayer->Play(Frame);
	}
	else
	{
		auto vtb = (*InAudioStream)->time_base;
		auto fTime = (SeekTimePoint * vtb.den) / (vtb.num * 1000);
		if ((Frame->pts - (*InAudioStream)->start_time) >= fTime)
		{
			auto StartTime = std::chrono::system_clock::now() - std::chrono::milliseconds(SeekTimePoint);
			VideoPlayer->SetStartPlayTime(StartTime);
			AudioPlayer->SetStartPlayTime(StartTime);
			SeekOverFlag = 0;
		}
	}
}

void KinMedia::MediaPlayer::PushVideoPacket( const AVPacket * pkt )
{
	if( avcodec_send_packet( VideoDecoder->Get(), pkt ) != 0 )
		return;

	while( avcodec_receive_frame( VideoDecoder->Get(), VideoFrame.Get() ) == 0 )
	{
		if (FirstPlayFlag)
		{
			if (VideoFrame->key_frame)
			{
				FirstPlayFlag = 0;
			}
		}

		if (!FirstPlayFlag)
		{
			PushVideoFrame(VideoFrame);
		}
		
	}
}

void KinMedia::MediaPlayer::PushAudioPacket( const AVPacket * pkt )
{
	if( avcodec_send_packet( AudioDecoder->Get(), pkt ) != 0 )
		return;

	while( avcodec_receive_frame( AudioDecoder->Get(), AudioFrame.Get() ) == 0 )
	{
		if (!FirstPlayFlag)
		{
			PushAudioFrame(AudioFrame);
		}
	
	}
}

void KinMedia::MediaPlayer::InitVideoPlayer( AVCodecParameters * Para )
{
	VideoDecoder = std::make_shared<MediaDecoder>();
	VideoDecoder->Init( *Para );

	FirstPlayFlag = 1;
	VideoPlayer->Init( Para, 0, { 0,0 } );
}

void KinMedia::MediaPlayer::InitAudioPlayer( AVCodecParameters * Para )
{
	AudioDecoder = std::make_shared<MediaDecoder>();
	AudioDecoder->Init( *Para );

	AudioPlayer->Init( Para, 0, { 0,0 } );
}

void KinMedia::MediaPlayer::SetVideoWindow( void * nWindow )
{
	VideoPlayer->SetInPara( nWindow );
}

void KinMedia::MediaPlayer::PlayUrlVideoThread()
{
	RunningTC c( RunningCont );

	auto StartTime = std::chrono::system_clock::now();
	VideoPlayer->SetStartPlayTime( StartTime );
	AudioPlayer->SetStartPlayTime( StartTime );

	while (isPlaying())
	{
		if (PauseFlag)
		{
			while (PauseFlag)
			{
				if (!isPlaying())
				{
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			continue;
		}
		if (NeedSeek)
		{
			auto vtb = (*InVideoStream)->time_base;
			auto sTime = (SeekTimePoint * vtb.den) / (vtb.num * 1000);
			auto Context = InFile->Get();
			av_seek_frame(Context.get(), (*InVideoStream)->index, sTime + (*InVideoStream)->start_time, AVSEEK_FLAG_BACKWARD);
			avcodec_flush_buffers((*InVideoStream).GetDecoder().Get());
			avcodec_flush_buffers((*InAudioStream).GetDecoder().Get());
			VideoPlayer->clear();
			AudioPlayer->clear();
			VideoPlayer->FixDuration = std::chrono::milliseconds(0);
			AudioPlayer->FixDuration = std::chrono::milliseconds(0);
			NeedSeek = 0;
			SeekOverFlag = 1;
		}

		if (VideoPlayer->residue() && AudioPlayer->residue())
		{
			if (!InFile->TravelPacket())
			{
				while (isPlaying())
				{
					if (VideoPlayer->size() > 0 || AudioPlayer->size() > 0)
					{
						std::this_thread::yield();
						continue;
					}
					else
					{
						break;
					}
				}
				RunFlag = 0;
				break;
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	AudioPlayer->Stop();
	VideoPlayer->Stop();
}
