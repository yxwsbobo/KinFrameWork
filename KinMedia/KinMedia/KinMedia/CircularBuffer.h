#pragma once
#include <mutex>
namespace KinMedia
{

	class CircularBuffer
	{
	public:
		CircularBuffer( size_t capacity );
		~CircularBuffer( );

		size_t size( ) const
		{
			return size_;
		}
		size_t capacity( ) const
		{
			return capacity_;
		}
		// Return number of bytes written.
		size_t write( const char *data, size_t bytes );
		// Return number of bytes read.
		size_t read( char *data, size_t bytes );

		char* GetPointer( )
		{
			return data_;
		}

		size_t  end_index_;
	private:
		size_t beg_index_, size_, capacity_;
		char *data_;
		std::mutex ioMutex;
	};
}

