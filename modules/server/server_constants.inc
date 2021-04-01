#define PlayerToPoint(%1,%2,%3,%4,%5)   IsPlayerInRangeOfPoint(%2,%1,%3,%4,%5)
//----------- COLORS ----------------//
#define COLOR_WHITE				0xFFFFFFFF
#define COLOR_WHITE_TRANSPARENT 0xFFFFFF00
#define COLOR_ORANGE			0xff8c00FF
#define COLOR_GREY				0xAFAFAFFF
#define COLOR_RED				0xFF0000FF
#define COLOR_PURPLE			0xC2A2DAFF
#define COLOR_GREEN				0x1eb032FF
#define COLOR_YELLOW			0xfce80cFF
#define COLOR_YELLOW_LIGHT		0xfdfe8bFF
#define COLOR_DODGER_BLUE		0x1e90ffFF
// Inline colors can be used inside a string
#define COLOR_WHITE_INLINE			"{FFFFFF}"
#define COLOR_ORANGE_INLINE			"{ff8c00}"
#define COLOR_DODGER_BLUE_INLINE	"{1e90ff}"
#define COLOR_GREEN_INLINE 			"{1eb032}"

// for ProxDetector
#define COLOR_FADE1 		0xE6E6E6E6
#define COLOR_FADE2 		0xC8C8C8C8
#define COLOR_FADE3 		0xAAAAAAAA
#define COLOR_FADE4 		0x8C8C8C8C
#define COLOR_FADE5 		0x6E6E6E6E

// VEHICLES
#define MIN_VEHICLE_HEALTH 350 // Less than 350 of health for a car will set it up on fire, with this we can avoid cars from setting up on fire.
#define MAX_VEHICLE_PUBLIC 73 // Maximum of public cars such as jobs, and factions. 
#define VEHICLE_RESPAWN_DELAY 3600 // 1h