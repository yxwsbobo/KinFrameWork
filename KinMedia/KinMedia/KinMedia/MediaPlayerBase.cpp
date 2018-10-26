//
// Created by Kin on 2017/11/4 0004.
//

#include "MediaPlayerBase.h"
#include <thread>
#include <KinBase/RunningTC.h>

KinMedia::MediaPlayerBase::MediaPlayerBase( uint32_t BufferSize )
	:FrameBuffer{ BufferSize }
{
}


void KinMedia::MediaPlayerBase::Init( const AVCodecParameters *Para, int64_t sTime, AVRational bTime )
{
	InitFun( Para );
	FrameStartTime = sTime;
	FrameBaseTime = bTime;
	RunningFlag = true;
	CurrentDuration = 0;
	FixDuration = std::chrono::milliseconds(0);
	std::thread( std::bind( &MediaPlayerBase::PlayThread, this ) ).detach();
}

void KinMedia::MediaPlayerBase::SetStartPlayTime( std::chrono::time_point<std::chrono::system_clock> StartTime )
{
	PlayStartTime = StartTime;
}

void KinMedia::MediaPlayerBase::Play( const KinMedia::MediaFrame &Frame )
{
	auto InnerFrame = Enqueue( Frame );
	if (InnerFrame)
	{
		std::lock_guard<std::mutex> lck(BufferMutex);
		FrameBuffer.push_back(InnerFrame);
	}
}

void KinMedia::MediaPlayerBase::Stop()
{
	RunningFlag = false;
	while( RunningCount != 0 )
	{
		std::this_thread::yield();
	}
	clear();
}

void KinMedia::MediaPlayerBase::Pause()
{
	PauseOver = false;
	PauseFlag = 1;
	while (!PauseOver)
	{
		std::this_thread::yield();
	}
}

void KinMedia::MediaPlayerBase::Resume()
{
	PauseFlag = 0;
}

bool KinMedia::MediaPlayerBase::isRunning()
{
	return RunningFlag;
}

uint32_t KinMedia::MediaPlayerBase::size()
{
	return (uint32_t)FrameBuffer.size();
}

uint32_t KinMedia::MediaPlayerBase::capacity()
{
	return (uint32_t)FrameBuffer.capacity();
}

uint32_t KinMedia::MediaPlayerBase::residue()
{
	return capacity() - size();
}

void KinMedia::MediaPlayerBase::clear()
{
	std::lock_guard<std::mutex> lck(BufferMutex);
	FrameBuffer.clear();
}

void KinMedia::MediaPlayerBase::resize( uint32_t size )
{
	std::lock_guard<std::mutex> lck(BufferMutex);
	FrameBuffer.resize( size );
}


void KinMedia::MediaPlayerBase::PlayThread()
{
	RunningTC c( RunningCount );

	while (isRunning())
	{

		if (PauseFlag)
		{
			PauseOver = true;
			auto sPause = std::chrono::system_clock::now();
			while (PauseFlag)
			{
				if (!isRunning())
				{
					break;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			FixDuration += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - sPause);
			continue;
		}

		std::shared_ptr<MediaFrame> InnerFrame = nullptr;
		{
			std::lock_guard<std::mutex> lck(BufferMutex);
			if (size() > 0)
			{
				InnerFrame = FrameBuffer.front();
				FrameBuffer.pop_front();
			}
		}

		if (!InnerFrame)
		{
			std::this_thread::yield();
			continue;
		}

		if (FrameBaseTime.den != 0 || FrameBaseTime.num != 0)
		{
			auto Duration = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now() - PlayStartTime);
			auto FrameDuration =
				(((*InnerFrame)->pts - FrameStartTime) * FrameBaseTime.num * 1000) /
				FrameBaseTime.den;
			CurrentDuration = FrameDuration;
			auto SleepDuration = std::chrono::milliseconds(FrameDuration) + FixDuration - Duration;
			std::chrono::milliseconds Early(3);
			if (SleepDuration.count() > 0)
			{
				std::this_thread::sleep_for(SleepDuration - Early);
			}
		}

		while (!ReadyToPlay())
		{
			std::this_thread::yield();
		}
		ShowFrame(InnerFrame);
		

	}
}

bool KinMedia::MediaPlayerBase::ReadyToPlay()
{
	return true;
}