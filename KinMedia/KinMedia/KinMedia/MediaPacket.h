#pragma once
#include <memory>
#include "FFmpegInclude.h"

namespace KinMedia
{

	class MediaPacket
	{
	public:
		MediaPacket();
		MediaPacket( int size );
		~MediaPacket() = default;

		void Init( int size );
		const AVPacket* operator->() const noexcept
		{
			return Pointer.get();
		}
		AVPacket* operator->() noexcept
		{
			return Pointer.get();
		}

		const AVPacket* Get() const noexcept
		{
			return Pointer.get();
		}
		AVPacket* Get() noexcept
		{
			return Pointer.get();
		}

	private:
		static void Deleter( AVPacket* &pkt );
		std::shared_ptr<AVPacket> Pointer;
	};
}

