//
// Created by Kin on 2017/11/4 0004.
//

#include "WinAudioPlayer.h"
#include <KinBase/KinBase.h>
KinMedia::WinAudioPlayer::WinAudioPlayer()
	:MediaPlayerBase{ 180 }
{
	AudioInfo.freq = 44100;
	AudioInfo.silence = 0;
	AudioInfo.size = 1024;
	AudioInfo.samples = 1024;
	AudioInfo.channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
	AudioInfo.format = AUDIO_S16SYS;

	AudioInfo.userdata = this;
	AudioInfo.callback = AudioCallBack;
	Must(SDL_OpenAudio(&AudioInfo, NULL) == 0, "OpenAudio Error");
}

KinMedia::WinAudioPlayer::~WinAudioPlayer()
{
	SDL_CloseAudio();
}

void KinMedia::WinAudioPlayer::InitFun(const AVCodecParameters * Para)
{
	AudioPara = *Para;
	AudioPara.format = AV_SAMPLE_FMT_S16;
	AudioPara.channel_layout = AV_CH_LAYOUT_STEREO;
	AudioPara.channels = av_get_channel_layout_nb_channels(AudioPara.channel_layout);

	auto SwrCtx = swr_alloc();
	SwrCtx = swr_alloc_set_opts(SwrCtx, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,
		av_get_default_channel_layout(Para->channels), (AVSampleFormat)Para->format, Para->sample_rate, 0, 0);
	swr_init(SwrCtx);
	Swr.reset(SwrCtx, SwrDeleter);

	BufferSize = av_samples_get_buffer_size(NULL, AudioInfo.channels, AudioInfo.samples, AV_SAMPLE_FMT_S16, 1);
	SDL_PauseAudio(0);
}

void KinMedia::WinAudioPlayer::ShowFrame(std::shared_ptr<MediaFrame> Frame)
{
	std::lock_guard<std::mutex> lck(WriteMutex);
	cBuffer.write((char*)Frame->Get()->data[0], BufferSize);
}

std::shared_ptr<KinMedia::MediaFrame> KinMedia::WinAudioPlayer::Enqueue(const MediaFrame & Frame)
{
	//maby TempFrameBuffer not enough
	auto TempFrame = std::make_shared<MediaFrame>(AudioPara.channel_layout, AudioPara.sample_rate, AudioPara.frame_size, (AVSampleFormat)AudioPara.format);

	swr_convert(Swr.get(), &TempFrame->Get()->data[0], TempFrame->Get()->nb_samples, (const uint8_t **)Frame->data, Frame->nb_samples);
	TempFrame->Get()->pts = Frame->pts;
	TempFrame->Get()->pkt_dts = Frame->pkt_dts;
	return TempFrame;
}

void KinMedia::WinAudioPlayer::AudioCallBack(void * userdata, Uint8 * stream, int len)
{
	char TempBuffer[MAX_AUDIO_FRAME_SIZE];
	auto me = (WinAudioPlayer*)userdata;
	SDL_memset(stream, 0, len);
	std::lock_guard<std::mutex> lck(me->WriteMutex);
	len = me->cBuffer.read(TempBuffer, len);

	SDL_MixAudio(stream, (uint8_t*)TempBuffer, len, SDL_MIX_MAXVOLUME);
}

void KinMedia::WinAudioPlayer::SwrDeleter(SwrContext *& swr)
{
	swr_free(&swr);
}
