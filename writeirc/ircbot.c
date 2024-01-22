
#include "ignotalib/src/ig_net/ign_inetcli.h"
#include "ignotalib/src/ig_net/ign_strtoport.h"
#include "ignotalib/src/ig_event/igev_signals.h"
#include "ignotalib/src/ig_file/igf_read.h"
#include "ignotalib/src/ig_file/igf_write.h"
#include "ignotalib/src/ig_file/igf_opt.h"

#include <signal.h>
#include <unistd.h>

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFSIZE 8192

// basic error log function
void fail( const char *const estr )  {

  perror( estr );
  exit( EXIT_FAILURE );

}


// connect and set after that socket nonblocking
int setcon( const char *const ip, const long int port )  {

  int confd = ign_inettcpcli( ip, port );
  if( confd == -1 )  return -1;
  
  if( igf_nonblock( confd ) == -1 )  return -1;
  
  return confd;

}

// read stuff from fd to buff
// then try to find message
// if yes save it to buff_msg return 1
// 0 if no message found
// error: -1
// FIN on socket or EOF on file: -1 + errno set to 0
// log error if buff filled but no msgend, discard data
int reader( 
    const int fd,
    char *const buff, 
    char **buff_freepos,
    size_t *buff_leftsize,
    char *const buff_msg,
    size_t *const buff_msg_size,
    const char *const msgend,
    const size_t msgend_size
    )   {

  // first try to read stuff
  ssize_t readret = igf_read_nb( fd, *buff_freepos, *buff_leftsize );
    
  // blocking is handled inside function as eintr  
  // so potentialy more serious stuff here
  if( readret == -1 )  return -1;
  if( readret == 0 )  { // we have FIN EOF
  
    errno = 0; // reset errno so we know it's FIN EOF not error
    return -1;

  }

  // update buff free pos and left size
  *buff_freepos += readret;
  *buff_leftsize -= readret;

  // look for message end
  ptrdiff_t msgsize = *buff_freepos - buff;
  // read is smaller than message end, we can leave
  if( msgsize < msgend_size )  return 0;

  // we can't go further becasue we could leave buff memory in some cases
  size_t msgmaxsize = msgsize - msgend_size; // note <= in loop
  for( size_t i = 0; i <= msgmaxsize; i++ )  {

    // There is a full message
    if( ! memcmp( &buff[i], msgend, msgend_size ) )  {

      // all we need to do with buff_msg part
      *buff_msg_size = i; // set it's new size
      memcpy( buff, buff_msg, *buff_msg_size ); // copy msg

      // update buff leftsize and freepos
      *buff_leftsize += *buff_msg_size + msgend_size;
      *buff_freepos -= *buff_msg_size + msgend_size;

      // move data in buff
      size_t curdata_size = *buff_freepos - buff;
      memmove( &buff[ i + msgend_size ], buff, curdata_size );
      return 1;  // msg passed to buff_msg and discarded from buff

    }

  }

  return 0; // we never found message

}

int main( const int argc, const char *const argv[] )  {

  // check args and set with pointer variables
  if( argc != 4 )  fail( "We need IP, port and irc channel in argments" );
  const char *const ip = argv[1];
  const char *const port = argv[2];
  const char *const chan = argv[3];

  // error log passed arguments and set port
  fprintf( stderr, "%s %s %s\n", ip, port, chan );
  long int port_num = ign_strtoport( port );
  if( port_num == -1 )  fail( "Port number is invalid" );

  // turnoff sigpipe, nonblock on stdin
  if( igev_sigign( SIGPIPE ) == -1 )
    fail( "Could not ignore SIGPIPE" );
  int stdin_fd = fileno( stdin );
  if( igf_nonblock( stdin_fd ) == -1 )
    fail( "Failed setting stdin nonblock" );

  // set up connection socket
  int confd = setcon( ip, port_num );
  if( confd == -1 ) fail( "Failed on seting up conection socket" );

  // buffs program uses
  char buffirc[ BUFFSIZE ];
  char buffstdin[ BUFFSIZE ];
  char buff_msg[ BUFFSIZE ];

  size_t buffirc_leftsize = BUFFSIZE;
  char *buffirc_freepos = buffirc;
  size_t buffstdin_leftsize = BUFFSIZE;
  char *buffstdin_freepos = buffstdin;
  size_t buff_msg_size = 0;


/*
  // startup loop
  for(;;)  {



  }

  // main program loop
  for(;;)  {


  }
*/
 error_cleanup:
  close( confd );
  return -1;

}
