#include "MediaFrameTranslater.h"
#include <KinBase/KinBase.h>
#include "MediaOutputStream.h"

void KinMedia::MediaFrameTranslater::ConnectTo( MediaOutputStream & outStream )
{
	TranslateNotify.Add( std::bind( &MediaOutputStream::EncodeFrame, &outStream, std::placeholders::_1 ) );
}

KinMedia::MediaFrameTranslater::MediaFrameTranslater( const MediaInputStream&sStream, const MediaOutputStream& dStream )
	: Source( sStream ), Dest( dStream )
{
	InterFrame = Dest.AllocFrame();
}