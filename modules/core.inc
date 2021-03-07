#include <YSI_Coding\y_hooks>

#define PlayerToPoint(%1,%2,%3,%4,%5)   IsPlayerInRangeOfPoint(%2,%1,%3,%4,%5)
//----------- COLORS ----------------//
#define COLOR_ORANGE	0xb57c01FF

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