#pragma once
#include "MediaFrame.h"
#include "FFmpegInclude.h"
#include <memory>
#include <atomic>
#include <chrono>
#include<boost/circular_buffer.hpp>
#include <mutex>

namespace KinMedia
{

	class MediaPlayerBase
	{
	public:
		MediaPlayerBase( uint32_t BufferSize );
		virtual ~MediaPlayerBase() = default;

		void Init( const AVCodecParameters* Para, int64_t sTime, AVRational bTime );
		void SetStartPlayTime( std::chrono::time_point<std::chrono::system_clock> StartTime );
		void Play( const MediaFrame& Frame );
		void Stop();
		void Pause();
		void Resume();
		uint64_t GetCurrentDuration()
		{
			return CurrentDuration;
		}
		uint64_t CurrentDuration = 0;
		std::chrono::milliseconds FixDuration{ 0 };
		virtual void SetInPara( void* Para )
		{
		}

		bool isRunning();
		uint32_t size();
		uint32_t capacity();
		uint32_t residue();
		void clear();
		void resize( uint32_t size );

	protected:
		virtual void InitFun( const AVCodecParameters* Para ) = 0;
		virtual void ShowFrame( std::shared_ptr<MediaFrame> Frame ) = 0;
		virtual std::shared_ptr<MediaFrame> Enqueue( const MediaFrame& Frame ) = 0;
		virtual bool ReadyToPlay();

	private:
		void PlayThread();


	private:
		int64_t FrameStartTime;
		AVRational FrameBaseTime;
		std::chrono::time_point<std::chrono::system_clock> PlayStartTime;
		std::atomic_char32_t PauseFlag{ 0 };
		std::atomic_bool PauseOver;
		std::atomic_bool RunningFlag{ false };
		std::atomic_char32_t RunningCount{ 0 };

		boost::circular_buffer<std::shared_ptr<MediaFrame>> FrameBuffer;
		std::mutex BufferMutex;
		
	};
}

