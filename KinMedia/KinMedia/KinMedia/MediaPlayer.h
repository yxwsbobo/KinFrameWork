#pragma once
#include "FFmpegInclude.h"
#include "MediaDecoder.h"
#include "MediaPacket.h"
#include "MediaFrame.h"
#include "MediaPlayerBase.h"
#include <memory>
#include <string>
#include <atomic>
#include "MediaInputFile.h"
#include "MediaInputStream.h"

namespace KinMedia
{

	class MediaPlayer
	{
	public:
		MediaPlayer(std::shared_ptr<MediaPlayerBase> vPlayer, std::shared_ptr<MediaPlayerBase> aPlayer );
		~MediaPlayer();

		void Play( std::string url );
		void Stop();
		void Pause();
		void Resume();
		int64_t GetTotalTime();
		int GetCurrentPercent();
		void Seek(int64_t Time);
		bool isPlaying();

		void PushVideoFrame( const MediaFrame & Frame );
		void PushAudioFrame( const MediaFrame & Frame );
		void PushVideoPacket( const AVPacket * pkt );
		void PushAudioPacket( const AVPacket * pkt );

		void InitVideoPlayer( AVCodecParameters* Para );
		void InitAudioPlayer( AVCodecParameters* Para );
		void SetVideoWindow( void* nWindow );

	public:
		std::shared_ptr<MediaPlayerBase> AudioPlayer;
		std::shared_ptr<MediaPlayerBase> VideoPlayer;

	private:
		void PlayUrlVideoThread();

	private:
		std::atomic_char32_t PauseFlag{ 0 };
		std::atomic_char32_t RunFlag{ 0 };
		std::atomic_char32_t RunningCont{ 0 };
		std::atomic_char32_t FirstPlayFlag{ 0 };
		std::string Path;
		//int64_t VideoFileSeekTime{ -1 };
		//std::atomic_int64_t 
		std::atomic_char32_t NeedSeek{ 0 };
		std::atomic_char32_t SeekOverFlag{ 0 };
		int64_t SeekTimePoint{ 0 };


		std::shared_ptr<MediaDecoder> VideoDecoder;
		std::shared_ptr<MediaDecoder> AudioDecoder;

		MediaFrame VideoFrame;
		MediaFrame AudioFrame;

	private:
		std::shared_ptr<MediaInputFile> InFile;
		std::shared_ptr<MediaInputStream> InVideoStream;
		std::shared_ptr<MediaInputStream> InAudioStream;
	};
}
