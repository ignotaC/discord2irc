
#include "discord.h"
#include "log.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CONF "../../data/config.json"

u64snowflake guild_id;

void fail( const char *const errstr )  {

  perror( errstr );
  exit( EXIT_FAILURE );

}


void dc_ready( struct discord *dcli, const struct discord_ready *ev )  {

  ( void )dcli;
  log_info( "Connected to server, %s %s",
      ev->user->username, ev->user->discriminator );

}
 
u64snowflake  get_guildid( struct discord *dcli,
      const char *const guild_name )  {

    struct discord_guilds guilds = { 0 };
    struct discord_ret_guilds ret = { .sync = &guilds };
    CCORDcode code;
    code = discord_get_current_user_guilds( dcli, &ret );

    assert( guilds.size != 0 );
    assert( CCORD_OK == code );
 
    size_t guildname_len = strlen( guild_name );
    int pos = -1;
    for( int i = 0; i < guilds.size; i++ )  {

      if( ! strncmp( guild_name, guilds.array[i].name, guildname_len ) )  {

        pos = i;
        break;

      }

    }

    assert( pos != -1 );

    u64snowflake guild_id = guilds.array[pos].id;
    discord_guilds_cleanup( &guilds );
    return guild_id;

}
 

void new_msg( struct discord *dcli, const struct discord_message *ev) {
  
    if( ev->guild_id != guild_id )  return;

    struct discord_channels chan = { 0 };
    struct discord_ret_channels ret = { .sync = &chan };
    CCORDcode code = discord_get_guild_channels(
        dcli,
        ev->guild_id,
        &ret
      );
    assert( code == CCORD_OK );

    char *chan_name = NULL;
    for( int i = 0; i < chan.size; i++ )   {

      if( chan.array[i].id == ev->channel_id )  {

        chan_name = chan.array[i].name;
        break;

      }

    }

    if( chan_name == NULL )  {

      perror( "Error on getting channel name" );
      goto finish;

    }

    log_info( "[MSG]#%s|%s: %s",
        chan_name,
        ev->author->username,
        ev->content );

finish:
    discord_channels_cleanup( &chan );

}


int main( const int argc, const char *const argv[] )  {

    if( argc != 2 )
      fail( "This program needs discord servername we are going to log" );

    ccord_global_init();
    struct discord *dcli = discord_config_init( CONF );
    if( dcli == NULL ) fail( "Couldn't initialize client" );
 
    discord_set_on_ready( dcli, dc_ready );
  
    guild_id = get_guildid( dcli, argv[1] );

    discord_add_intents( dcli, DISCORD_GATEWAY_MESSAGE_CONTENT );
    discord_set_on_message_create( dcli, new_msg );
 
    discord_run( dcli );

    discord_cleanup( dcli );
    ccord_global_cleanup();

    return 0; // this never returns so what ever happens it's a bug

}

