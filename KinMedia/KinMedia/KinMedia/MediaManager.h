#pragma once
#include <vector>
#include <string>

namespace KinMedia
{

	class FFmpegInitClass
	{
	public:
		FFmpegInitClass();
		~FFmpegInitClass();
	};
	extern FFmpegInitClass FFmpegInitMember;

	class MediaManager
	{
	public:
		MediaManager();
		~MediaManager();

		struct DevicesName
		{
			std::string FriendName;
			std::string MonikerName;
		};

		static const std::vector<DevicesName>& GetAudioDevices();
		static const std::vector<DevicesName>& GetVideoDevices();

		static void UpdateDevicesList();
	};
}

