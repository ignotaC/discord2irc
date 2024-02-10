

#include "writeirc/ignotalib/src/ig_time/igt_sleep.h"
#include "writeirc/ignotalib/src/ig_file/igf_read.h"
#include "writeirc/ignotalib/src/ig_file/igf_write.h"

#include <sys/wait.h>

#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void fail( const char *const estr )  {

  perror( estr );
  exit( EXIT_FAILURE );

}

#define IRCPATH "writeirc"
#define IRCBIN "runirc"

#define DISCPATH "scandiscord"
#define DISCBIN "rundisc"



pid_t discproc( char *const argv[] )  {

  // fork here
  pid_t ret_pid = fork();
  if( ret_pid == -1 )  return -1;
  if( ret_pid != 0 )  return ret_pid; // parent leaves

  // child 
  fclose( stdin );
  if( chdir( DISCPATH ) == -1 )  return -1;
  execve( DISCBIN, &argv[0], NULL );
  
  // if we are here obviously there was error on execve
  perror( "Execve failed" );
  exit( EXIT_FAILURE );

  // parent will catch this on wait

}

pid_t ircproc( char *const argv[]  )  {

  // fork here
  pid_t ret_pid = fork();
  if( ret_pid == -1 )  return -1;
  if( ret_pid != 0 )  return ret_pid; // parent leaves

  // child
  fclose( stdout );
  if( chdir( IRCPATH ) == -1 )  return -1;
  execve( IRCBIN, &argv[0], NULL );
  
  // if we are here obviously there was error on execve
  perror( "Execve failed" );
  exit( EXIT_FAILURE );

  // parent will catch this on wait

}

#define WAIT_ANY ( ( pid_t )-1 )

int main( const int argc, char *const argv[] )  {

  ( void )argc; // we don't use it

  setsid(); // we create out own session. 
	    // so it's easy to run from term &

  // If it fails on first start it's better to stop for now
  pid_t disc_pid = discproc( argv );  
  if( disc_pid == -1 ) fail( "Could not start discord" );

  pid_t irc_pid = ircproc( argv );  
  if( irc_pid == -1 ) fail( "Could not start irc" );

  // program buff
  const size_t buff_size = 8192;
  char buff[ buff_size ];

  // reading vars
  ssize_t ret_io;
  const int stdin_fd = fileno( stdin );
  const int stdout_fd = fileno( stdout );

  for( pid_t retwait = -1;;)  {

    ret_io = igf_read( stdout_fd, buff, buff_size );
    if( ret_io >= 0 )  fail( "Fail on read" );

    ret_io = igf_write( stdin_fd, buff, ret_io );
    if( ret_io >= 0 )  fail( "Fail on read" );
    
    sleep( 1 ); // lazy data fflush

    // children simply interact and we just check wait.
    // they should work endlessly, if one falls
    // we check which pid and resurect it.
    retwait = waitpid( WAIT_ANY, NULL, WNOHANG );
    if( retwait == 0 )  continue;  // nothing is happening

    // check errors
    if( retwait == -1 )   {

      if( errno == EINTR )  continue; // not serous
      fail( "Fail on wait" );

    }

    // at this point one of our processess terminated
    // which we don't expect to happen, so we will resurect it back

    if( retwait == disc_pid )  {

      for(;;)  {

	// wait on fail
	fprintf( stderr, "Restarting discord loger" );
        sleep( 5 );
	disc_pid = discproc( argv );
	if( disc_pid != -1 )  break; // all fine

      }

    }  else  {  // we only have two.

      for(;;)  {

	// wait on fail
        sleep( 5 );
	fprintf( stderr, "Restarting irc bot" );
        irc_pid = ircproc( argv );  
	if( disc_pid != -1 )  break; // all fine

      }

    }

  }

  return 0;

}






