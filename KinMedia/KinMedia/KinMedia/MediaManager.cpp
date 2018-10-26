#include "MediaManager.h"
#include "MediaInclude.h"
#include <thread>
#include <mutex>
//#include "MediaStrogGdi.h"
#include "FFmpegInclude.h"
//#include <SDL/SDL.h>

//extern void( *av_log_callback )( void*, int, const char*, va_list );
std::vector<KinMedia::MediaManager::DevicesName> VideoDevices;
std::vector<KinMedia::MediaManager::DevicesName> AudioDevices;

static constexpr char BeginVideoInfo[ ] = u8"DirectShow video devices (some may be both video and audio devices)";
static constexpr char BeginAudioInfo[ ] = u8"DirectShow audio devices\n";
static constexpr char AlternativeNameInfo[ ] = u8"Alternative name";

enum class CurrentDeivceTypeStruct
{
	Video,
	Audio,
	Unknown
}CurrentDeivceType;

static void FilterLog( void*, int, const char* Info, va_list li )
{
	static KinMedia::MediaManager::DevicesName Temp;
	if( strncmp( Info, BeginVideoInfo, sizeof( BeginVideoInfo ) - 1 ) == 0 )
	{
		CurrentDeivceType = CurrentDeivceTypeStruct::Video;
		return;
	}
	else 	if( strncmp( Info, BeginAudioInfo, sizeof( BeginAudioInfo ) - 1 ) == 0 )
	{
		CurrentDeivceType = CurrentDeivceTypeStruct::Audio;
		return;
	}

	std::vector<KinMedia::MediaManager::DevicesName>* CurrentVec;
	switch( CurrentDeivceType )
	{
	case CurrentDeivceTypeStruct::Video:
		CurrentVec = &VideoDevices;
		break;
	case CurrentDeivceTypeStruct::Audio:
		CurrentVec = &AudioDevices;
		break;
	case CurrentDeivceTypeStruct::Unknown:
	default:
		return;
	}

	if( strstr( Info, "%s" ) == NULL )
	{
		return;
	}

	//maby Need check
	if( strstr( Info, AlternativeNameInfo ) == NULL )
	{
		Temp.FriendName = std::string( va_arg( li, char* ) );
	}
	else
	{
		Temp.MonikerName = std::string( va_arg( li, char* ) );
		CurrentVec->push_back( Temp );
	}
}

KinMedia::MediaManager::MediaManager()
{
}

KinMedia::MediaManager::~MediaManager()
{
}

const std::vector<KinMedia::MediaManager::DevicesName>& KinMedia::MediaManager::GetAudioDevices()
{
	return AudioDevices;
}

const std::vector<KinMedia::MediaManager::DevicesName>& KinMedia::MediaManager::GetVideoDevices()
{
	return VideoDevices;
}

void KinMedia::MediaManager::UpdateDevicesList()
{
	VideoDevices.clear();
	AudioDevices.clear();
	CurrentDeivceType = CurrentDeivceTypeStruct::Unknown;
	av_log_set_callback( FilterLog );

	AVFormatContext *pFormatCtx = avformat_alloc_context();
	AVDictionary* options = NULL;
	av_dict_set( &options, "list_devices", "true", 0 );
	AVInputFormat *iformat = av_find_input_format( "dshow" );
	avformat_open_input( &pFormatCtx, "dummy", iformat, &options );

	av_log_set_callback( NULL );

	av_dict_free( &options );
	avformat_close_input( &pFormatCtx );
}

KinMedia::FFmpegInitClass::FFmpegInitClass()
{
//#ifndef _MSC_VER
	av_register_all();
//#endif // _MSC_VER

	avdevice_register_all();
	avformat_network_init();
	//MediaStrogGdi::Init( );
	//SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER );
}
KinMedia::FFmpegInitClass::~FFmpegInitClass()
{
	//SDL_Quit( );
}

KinMedia::FFmpegInitClass FFmpegInitMember;

//
//static void AudioVideo( KinMedia::MediaInputFile& iFile, int a )
//{
//	int i = 0;
//	while( flag )
//	{
//		if( a == 1 )
//		{
//			printf( "Vide :%d\n", ++i );
//		}
//		else
//		{
//			printf( "Audio :%d\n", ++i );
//		}
//
//		if( iFile.TravelPacket( ) == false )
//		{
//			break;
//		}
//	}
//
//}

//void KinMedia::MediaManager::Run( )
//{
////int FrameCnt = 0;
//MediaInputFile CameraVideoFile( u8"video=e2eSoft VCam", "dshow" );

////MediaInputFile iFile2( "D:\\Kin\\Downloads\\Video\\ok.mp4" );
////MediaInputFile iFile( "D:\\Kin\\Downloads\\Video\\123.flv" );
////MediaInputFile iFile( "d:\\Media.mp4" );
//MediaInputFile VideoFile( (void*)0X1C07F0 );
//MediaOutputFile OutFile( "d:\\Media2.mp4" );

//av_register_all( );
//avdevice_register_all( );
//avformat_network_init( );
//MediaStrogGdi::Init( );

////Video
//auto ivStream = VideoFile.GetVideoStream( );
//auto &ivcodepar = ivStream->codecpar;
//VideoStreamInfo ps;
//ps.BitRate = 800000;
//ps.StreamTimeBase = { 1,90000 };
//ps.FrameRate = { 30,1 };
//ps.Width = 800;
//ps.Height = 600;
//auto ovStream = OutFile.AllocStream( ps );
//auto VideoTranslate = ivStream.GetFrameTranslate( ovStream );

//VideoFile.ConnectTo( ivStream );
//ivStream.ConnectTo( VideoTranslate );

//VideoTranslate->ConnectTo( ovStream );
//ovStream.ConnectTo( OutFile );

//StreamBlendSetting BlendSettings;
//BlendSettings.StartX = 300;
//BlendSettings.StartY = 200;

////VideoCame
//auto ivStreamCamera = CameraVideoFile.GetVideoStream( );
//CameraVideoFile.ConnectTo( ivStreamCamera );
//ivStreamCamera.ConnectTo( ivStream, BlendSettings );

////Audio
//auto iAudioStream = AudioFile.GetAudioStream( );
//auto Decoder = iAudioStream.GetDecoder( ).Get( );

//AudioStreamInfo is;
//is.SampleRate = Decoder->sample_rate;
//auto oAudioStream = OutFile.AllocStream( is );
//auto AudioTranslate = iAudioStream.GetFrameTranslate( oAudioStream );

//AudioFile.ConnectTo( iAudioStream );
//iAudioStream.ConnectTo( AudioTranslate );
//AudioTranslate->ConnectTo( oAudioStream );
//oAudioStream.ConnectTo( OutFile );


//OutFile.WriteHeader( );

//std::thread t1( AudioVideo,VideoFile,1 );
//t1.detach( );
//std::thread t2( AudioVideo, AudioFile, 0 );
//t2.detach( );
//std::thread t3( AudioVideo,CameraVideoFile, 1 );
//t3.detach( );
//getchar( );
//flag = false;
//getchar( );

//OutFile.WriteTrailer( );
//}

