#include "MediaCodeCtx.h"
#include <KinBase/KinBase.h>


//( *this )->framerate = av_div_q( av_inv_q( Timebase ), fRate );

void KinMedia::MediaCodeCtx::Deleter( AVCodecContext *& ctx )
{
	avcodec_free_context( &ctx );
}
