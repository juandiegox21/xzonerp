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

/* ===  Gamemode modules === */

// Server
#include <server>

// Account
#include <account> 

// Factions
#include <factions>

// Jobs
#include <jobs>

// Commands
#include <cmds>

// World
#include <maps>

// Vehicles
#include <vehicles>

//Gamemode
main()
{
	print(" ");
	print("XZone Roleplay... Bienvenido");
}