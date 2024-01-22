
#include "ignotalib/src/ig_net/ign_inetcli.h"
#include "ignotalib/src/ig_net/ign_strtoport.h"
#include "ignotalib/src/ig_event/igev_signals.h"
#include "ignotalib/src/ig_file/igf_read.h"
#include "ignotalib/src/ig_file/igf_write.h"
#include "ignotalib/src/ig_file/igf_opt.h"
#include "ignotalib/src/ig_time/igt_sleep.h"

#include <signal.h>
#include <unistd.h>

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
    size_t *buff_leftsize
    )   {

  assert( fd >= 0 );
  assert( buff != NULL );
  assert( buff_freepos != NULL );
  assert( *buff_freepos != NULL );
  assert( buff_leftsize != NULL );

  if( *buff_leftsize == 0 )  return 0;

  // first try to read stuff
  ssize_t readret = igf_read_nb( fd, *buff_freepos, *buff_leftsize );
    
  // blocking is handled inside function so is eintr  
  // so potentialy more serious stuff here
  if( readret == -1 )  return -1;
  if( readret == 0 )  { // we have FIN EOF
  
    if( errno == EAGAIN )  return 0;
    return -1;

  }

  // update buff free pos and left size
  *buff_freepos += readret;
  *buff_leftsize -= readret;
  return 1;

}


int get_msg( 
    char *const buff, 
    char **buff_freepos,
    size_t *buff_leftsize,
    char *const buff_msg,
    size_t *const buff_msg_size,
    const char *const msgend,
    const size_t msgend_size,
    char **scanpos
    )   {

  assert( buff != NULL );
  assert( buff_freepos != NULL );
  assert( *buff_freepos != NULL );
  assert( buff_leftsize != NULL );
  assert( buff_msg != NULL );
  assert( buff_msg_size != NULL );
  assert( msgend != NULL );
  assert( scanpos != NULL );
  assert( *scanpos != NULL );


  // scanpos should never pass freepos
  if( *buff_freepos - *scanpos < msgend_size  )
    return 0;  // we have nothing to do

  // look for message end
  ptrdiff_t msgsize = *buff_freepos - buff;
  assert( msgsize >= 0 );
  // read is smaller than message end, we can leave
  if( ( size_t )msgsize < msgend_size )  return 0;

  // we can't go further becasue we could leave buff memory in some cases
  size_t msgmaxsize = msgsize - msgend_size; // note <= in loop
  for( size_t i = *scanpos - buff; i <= msgmaxsize; i++ )  {

    // There is a full message
    if( ! memcmp( &buff[i], msgend, msgend_size ) )  {

      // because we will move all not copied data
      // at the buff start so does the scanpos go on start
      *scanpos = buff;

      // all we need to do with buff_msg part
      *buff_msg_size = i; // set it's new size
      memcpy( buff_msg, buff, *buff_msg_size ); // copy msg

      // update buff leftsize and freepos
      *buff_leftsize += *buff_msg_size + msgend_size;
      *buff_freepos -= *buff_msg_size + msgend_size;

      // move data in buff
      size_t curdata_size = *buff_freepos - buff;
      memmove( buff, &buff[ i + msgend_size ], curdata_size );
      return 1;  // msg passed to buff_msg and discarded from buff

    }

  }

  // We never found message but if buffor is full
  // we are going to discard this data simply
  // It is lost for ever
  if( *buff_leftsize == 0 )  {

    *buff_leftsize += *buff_freepos - buff;
    *buff_freepos = buff;
    *scanpos = buff;  

  }  else  {

    *scanpos = *buff_freepos + 1 - msgend_size;

  }

  return 0;

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
  int stdinfd = fileno( stdin );
  if( stdinfd == -1 )  fail( " Failed to get stdin fd" );
  if( igf_nonblock( stdinfd ) == -1 )
    fail( "Failed setting stdin nonblock" );

  // buffs program uses

  char buffirc[ BUFFSIZE ];
  size_t buffirc_leftsize;
  char *buffirc_freepos;
  char buffirc_msg[ BUFFSIZE ] = {0};
  size_t buffirc_msg_size;
  char *irc_scanpos;

  char buffstdin[ BUFFSIZE ];
  size_t buffstdin_leftsize;
  char *buffstdin_freepos;
  char buffstdin_msg[ BUFFSIZE ];
  size_t buffstdin_msg_size;
  char *stdin_scanpos;

  //msg endings

  const char *const stdin_end = "\n";
  const size_t stdin_end_size = strlen( stdin_end );

  const char *const irc_end = "\r\n";
  const size_t irc_end_size = strlen( irc_end );

  // stuff from stdio never is restarted - stdio part errors
  // make program finish with error allways
  buffstdin_leftsize = BUFFSIZE;
  buffstdin_freepos = buffstdin;
  buffstdin_msg_size = 0;
  stdin_scanpos = buffstdin;
  int stdindata_stat = 0;
 
  // start program loop
  for(;;)  {
  
    // on the other hand irc fail can sometimes get fixed
    // but we end up reseting irc buffs 
    buffirc_leftsize = BUFFSIZE;
    buffirc_freepos = buffirc;
    buffirc_msg_size = 0;
    irc_scanpos = buffirc;
    int ircdata_stat = 0;

    // set up connection socket
    int confd = setcon( ip, port_num );
    if( confd == -1 ) fail( "Failed on seting up conection socket" );

    for( int readstat ;;)  {

      // perform reading

      readstat = reader( confd, buffirc, &buffirc_freepos,
          &buffirc_leftsize );

      if( readstat == -1 )  {

        close( confd );
        switch( errno )  {

          case 0:
          case ECONNRESET:
            break;  // restart

          default:
            fail( "Program failed on reader function from irc data" );

        }

      }

      readstat = reader( stdinfd, buffstdin, &buffstdin_freepos,
          &buffstdin_leftsize );

      if( readstat == -1 )  {

        close( confd );  
        fail( "Program failed on reader function from stdio data" );

      }

      // try to read data

      if(  ircdata_stat != 1  )  { 

        ircdata_stat = get_msg( buffirc, &buffirc_freepos,
           &buffirc_leftsize, buffirc_msg, &buffirc_msg_size,
           irc_end, irc_end_size, &irc_scanpos );

      }
      if(  stdindata_stat != 1  )  {

        stdindata_stat = get_msg( buffstdin, &buffstdin_freepos,
            &buffstdin_leftsize, buffstdin_msg, &buffstdin_msg_size,
            stdin_end, stdin_end_size, &stdin_scanpos );

      }
      // use data

      if( ircdata_stat == 1 )  {

        for( int i = 0; i < buffirc_msg_size; i++ )
          putchar( buffirc_msg[i] );
        ircdata_stat = 0;
        puts("");

      }

      igt_sleepmsec( 200 );
  
    }

  }

  // miricle, this should never leave loop !
  return -1;

}
