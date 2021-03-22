#include <a_samp>

#undef 		MAX_PLAYERS
#define 	MAX_PLAYERS 		100

#include <env>
#include <logger>
#include <fixes>
#include <crashdetect>
#include <streamer>

#include <a_mysql>
#include <sscanf2>
#include <PawnPlus>
#include <eSelection>

#define YSI_NO_HEAP_MALLOC
#define FOREACH_NO_BOTS
#define FOREACH_NO_STREAMED

#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_timers>
#include <YSI_Visual\y_dialog>
#include <YSI_Coding\y_inline>

#include <getspawninfo>
#include <weaponskill>
#include <protection>

#include <izcmd>

// Server
#include "../modules/server/time.inc"
#include "../modules/server/core.inc"
#include "../modules/server/cmd_process.inc"
#include "../modules/server/mysql.inc" // MySQL connection

// Player 
#include "../modules/player/account.inc"

// Factions
#include "../modules/factions.inc"

// Player commands
#include "../modules/player/commands.inc" // Chat 

// Admin commands
#include "../modules/admin/commands.inc"

// World
#include "../modules/world/maps.inc"

// Vehicles
#include "../modules/vehicles.inc"

//Gamemode
main()
{
	print(" ");
	print("XZone Roleplay... Bienvenido");
}