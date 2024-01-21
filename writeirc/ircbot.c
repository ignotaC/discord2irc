
#include "ignotalib/src/ig_net/ign_inetcli.c"
#include "ignotalib/src/ig_file/igf_read.c"
#include "ignotalib/src/ig_file/igf_write.c"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fail( const char *const estr )  {

  perror( estr );
  exit( EXIT_FAILURE );

}

int main( const int argc, const char *const argv[] )  {

  if( argc != 3 )  fail( "We need IP and port in argments" );
  const char *const ip = argv[1];
  const char *const port = argv[2];



  return 0;

}
