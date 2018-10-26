#include "MediaPacket.h"
#include <KinBase/KinBase.h>


KinMedia::MediaPacket::MediaPacket()
{
	auto Packet = av_packet_alloc();
	Maybe( Packet, "Media Packet Alloc Error" );
	Pointer.reset( Packet, Deleter );
}

KinMedia::MediaPacket::MediaPacket( int size )
	: MediaPacket()
{
	Init( size );
}

void KinMedia::MediaPacket::Init( int size )
{
	av_packet_unref( Get() );
	Maybe( av_new_packet( Get(), size ) == 0, "MediaPacket Alloc Buffer Error" );
}

void KinMedia::MediaPacket::Deleter( AVPacket *& pkt )
{
	av_packet_unref( pkt );
	av_packet_free( &pkt );
}

