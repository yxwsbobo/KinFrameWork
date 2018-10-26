#pragma once
#include <memory>
#include "FFmpegInclude.h"

namespace KinMedia
{

	using MediaPacket = class MediaPacket;
	using MediaFrame = class MediaFrame;
	using MediaCodeCtx = class MediaCodeCtx;
	class MediaStream
	{
	public:
		MediaPacket AllocPacket() const;
		MediaFrame AllocFrame() const;

		const AVStream* operator->() const noexcept
		{
			return Pointer.get();
		}
		AVStream* operator->() noexcept
		{
			return Pointer.get();
		}

		int64_t GetStartPts() const noexcept;

		int64_t GetLastPts() const noexcept;

		int64_t GetLastDuration() const noexcept
		{
			return GetLastPts() - GetStartPts();
		}


		~MediaStream() = default;

	protected:
		std::shared_ptr<AVStream> Pointer;
		MediaCodeCtx* CodeCtx;
		int64_t LastPts;
	protected:
		MediaStream() = default;
	};
}

