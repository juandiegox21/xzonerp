#include <a_samp>

#undef 		MAX_PLAYERS
#define 	MAX_PLAYERS 		100

#include <fixes>
#include <crashdetect>
#include <streamer>
#include <a_mysql>
#include <sscanf2>
#include <izcmd>

#define 			function%0(%1) 							forward %0(%1); public %0(%1)
#if !defined isnull
    #define isnull(%1) \
                ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif
#define PlayerToPoint(%1,%2,%3,%4,%5)   IsPlayerInRangeOfPoint(%2,%1,%3,%4,%5)

main()
{
	print(" ");
	print("XZone Roleplay... Bienvenido");
}