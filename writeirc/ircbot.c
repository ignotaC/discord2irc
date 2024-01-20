
#include "ignotalib/src/ig_net/ign_inetcli.c"
#include "ignotalib/src/ig_file/igf_read.c"
#include "ignotalib/src/ig_file/igf_write.c"

#include <stdio.h>

int main( void )  {

  igf_write( fileno( stdout ), "zyd\n", 4 );


  return 0;

}
