#pragma once

#include <memory>
#include "FFmpegInclude.h"

namespace KinMedia
{
	class MediaCodeCtx
	{
	public:
		const AVCodecContext* operator->() const noexcept
		{
			return Pointer.get();
		}
		AVCodecContext* operator->() noexcept
		{
			return Pointer.get();
		}

		const AVCodecContext* Get() const noexcept
		{
			return Pointer.get();
		}
		AVCodecContext* Get() noexcept
		{
			return Pointer.get();
		}

	public:
		MediaCodeCtx() = default;
		~MediaCodeCtx() = default;

	protected:
		static void Deleter( AVCodecContext*& ctx );
		std::shared_ptr<AVCodecContext> Pointer;
	};
}

