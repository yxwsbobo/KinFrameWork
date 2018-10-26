#pragma once
#include <memory>
#include "FFmpegInclude.h"
#include "MediaStruct.h"

namespace KinMedia
{

	class MediaFrame
	{
	public:
		MediaFrame();
		MediaFrame( int Width, int Height, AVPixelFormat PixFmt );
		MediaFrame( const MediaPicInfo& info );
		MediaFrame( uint64_t ChannelLayout, int Rate, int NumSamples, AVSampleFormat Fmt );
		~MediaFrame() = default;

		void Init( int Width, int Height, AVPixelFormat PixFmt );
		void Init( uint64_t ChannelLayout, int Rate, int NumSamples, AVSampleFormat Fmt );
		void Init( const MediaPicInfo& info );

		int64_t GetBestPts() const noexcept;

		const AVFrame* operator->() const noexcept
		{
			return Pointer.get();
		}
		AVFrame* operator->() noexcept
		{
			return Pointer.get();
		}

		const AVFrame* Get() const noexcept
		{
			return Pointer.get();
		}
		AVFrame* Get() noexcept
		{
			return Pointer.get();
		}

		bool TimeArrive = false;
	private:
		static void Deleter( AVFrame* &Frame );
		std::shared_ptr<AVFrame> Pointer;
	};
}

