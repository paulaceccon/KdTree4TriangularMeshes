/*
 * Timer.h
 *
 *  Created on: Nov 26, 2013
 *      Author: jduartejr
 */

#ifndef TIMER_H_
#define TIMER_H_

class Timer
{
public:



    Timer( )
    {
        clock_gettime( CLOCK_REALTIME, &_startTime );
    }



    inline void restart( )
    {
        clock_gettime( CLOCK_REALTIME, &_startTime );
    }



    inline double elapsed( ) const
    {
        timespec endTime;
        clock_gettime( CLOCK_REALTIME, &endTime );
        return (endTime.tv_sec - _startTime.tv_sec) + (double) (endTime.tv_nsec - _startTime.tv_nsec) / 1000000000.0;
    }



    inline void printTime( const char* timerString ) const
    {
        printf( "%s: %lf\n", timerString, elapsed( ) );
    }

private:
    timespec _startTime;
};

#endif /* TIMER_H_ */