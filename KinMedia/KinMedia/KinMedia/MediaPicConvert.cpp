#include "MediaPicConvert.h"
#include <KinBase/KinBase.h>
KinMedia::MediaPicConvert::MediaPicConvert( MediaPicInfo & Source, MediaPicInfo & Dest, int flags )
{
	Init( Source, Dest, flags );
}

void KinMedia::MediaPicConvert::Init( MediaPicInfo & Source, MediaPicInfo & Dest, int flags )
{
	auto Result = sws_getContext( Source.Width, Source.Height, Source.Format, Dest.Width, Dest.Height, Dest.Format, flags, NULL, NULL, NULL );
	Maybe( Result, "sws_getContext Error" );
	Sws.reset( Result, Deleter );
}

int KinMedia::MediaPicConvert::Convert( const uint8_t * const sBuffer[ ], const int sStride[ ], int sSliceY,
	int srcSliceH, uint8_t * const dBuffer[ ], const int dStride[ ] )
{
	return sws_scale( Sws.get(), sBuffer, sStride, sSliceY, srcSliceH, dBuffer, dStride );
}

void KinMedia::MediaPicConvert::Deleter( SwsContext *& sws )
{
	sws_freeContext( sws );
}
