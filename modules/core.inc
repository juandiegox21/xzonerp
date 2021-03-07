#include <YSI_Coding\y_hooks>

#define PlayerToPoint(%1,%2,%3,%4,%5)   IsPlayerInRangeOfPoint(%2,%1,%3,%4,%5)
//----------- COLORS ----------------//

#define COLOR_ORANGE	0xff8c00FF
#define COLOR_GREY		0xAFAFAFFF
#define COLOR_RED		0xFF0000FF
// Inline colors can be used inside a string
#define COLOR_WHITE_INLINE	"{FFFFFF}"
#define COLOR_ORANGE_INLINE	"{ff8c00}"

hook OnGameModeInit() 
{
    AntiDeAMX(); //Evita desencriptar a partir del .AMX
    AddPlayerClass(2,1958.3783,1343.1572,1100.3746,269.1425,-1,-1,-1,-1,-1,-1); //Sistema de spawn
    AllowInteriorWeapons(1); //Permite armas en interiores
    DisableInteriorEnterExits(); //Desactiva entradas a locales
    EnableStuntBonusForAll(0); //Desactiva bonus por saltos y acrobacias
    ManualVehicleEngineAndLights(); //Sistema arranque de vehiculos
	
	/// enable all protections
    Protection_SetParamInt(PROTECTION_ALL, PROTECTION_ENABLED, 1);
    /// redefine name
    Protection_SetParamString(PROTECTION_TELEPORT, PROTECTION_NAME, "Teleport");
}

hook OnGameModeExit()
{
	// save all player data before closing connection
	for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) // GetPlayerPoolSize function was added in 0.3.7 version and gets the highest playerid currently in use on the server
	{
		if (IsPlayerConnected(i))
		{
            Kick(i);
		}
	}

	return 1;
}

// Avoid people clicking the spawn button
hook OnPlayerRequestClass(playerid, classid)
{
	return 0;
}

//Encriptador
static AntiDeAMX()
{
	new a[][] =
	{
		"Unarmed (Fist)",
		"Brass K"
	};
	#pragma unused a
}

// Delay kick
forward _KickPlayerDelayed(playerid);
public _KickPlayerDelayed(playerid)
{
	Kick(playerid);
	return 1;
}

DelayedKick(playerid, time = 500)
{
	SetTimerEx("_KickPlayerDelayed", time, false, "d", playerid);
	return 1;
}