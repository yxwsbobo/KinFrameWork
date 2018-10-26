#pragma once
#include "MediaStream.h"
#include "MediaEncoder.h"
#include "MediaPacket.h"
#include <KinBase/CallBackFunction.h>
#include <atomic>
namespace KinMedia
{

	using MediaOutputFile = class MediaOutputFile;
	class MediaOutputStream : public MediaStream
	{
	public:
		void EncodeFrame( const MediaFrame& frame );
		const MediaEncoder& GetEncoder() const noexcept;

		void ConnectTo( MediaOutputFile& outFile );

		~MediaOutputStream() = default;
		void SetNextKeyFrame();
	public:
		CallBackFunctions<void( MediaPacket& )> EncodeNotify;

	private:
		friend MediaOutputFile;
		MediaOutputStream() = default;
		MediaOutputStream( MediaOutputFile& vFile, VideoStreamInfo& info );
		MediaOutputStream( MediaOutputFile& vFile, AudioStreamInfo& info );

	private:
		MediaEncoder Encoder;
		MediaPacket InterPacket;
		int KeyFrame = 0;

	private:
		static void Deleter( AVStream* stream );
	};
}

