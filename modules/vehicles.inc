#include <YSI_Coding\y_hooks>

#define VEHICLE_RESPAWN_DELAY 3600 // 1h

enum e_VehicleData
{
	e_VehicleId,
	e_VehicleFactionId,
	e_VehicleModel,
	Float:e_VehiclePos[4], // X, Y, Z, Angle
	e_VehicleColor[2], // Color 1, Color 2
};

static const g_arrVehicleData[][e_VehicleData] =
{
	// ================== SAPD ==================
		// Police Car
		{
			-1, FACTION_SAPD, 596, {1545.5494, -1651.0291, 5.7106, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1545.5494, -1655.0783, 5.7106, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1545.5494, -1659.0273, 5.7106, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1545.5494, -1663.0397, 5.7106, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1545.5494, -1667.8980, 5.7106, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1545.5494, -1671.9782, 5.7106, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1545.5494, -1676.0792, 5.7106, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1545.5494, -1684.3708, 5.7401, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1545.5494, -1680.2238, 5.7401, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1526.4957, -1643.9830, 6.2757, 180.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1530.4660, -1643.9830, 6.2757, 180.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1534.69177246, -1643.9830, 6.2757, 180.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1538.7166, -1643.9830, 6.2757, 180.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1528.7050, -1688.0760, 5.6593, 270.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 596, {1528.7050, -1683.9211, 5.6593, 270.00}, {0, 1}
		},
		// Police Ranger
		{
			-1, FACTION_SAPD, 599, {1602.6211, -1691.9659, 6.0790, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 599, {1602.6211, -1687.8544, 6.1430, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 599, {1602.6211, -1683.9187, 6.1060, 90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 599, {1602.6211, -1696.0380, 6.1060, 90.00}, {0, 1}
		},
		// Tow Truck
		{
			-1, FACTION_SAPD, 525, {1591.4323, -1711.0713, 5.7688, 0.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 525, {1595.4498, -1711.0713, 5.7680, 0.00}, {0, 1}
		},
		// HPV1000
		{
			-1, FACTION_SAPD, 523, {1557.4336, -1710.5233, 5.4525, 0.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 523, {1559.8743, -1710.5233, 5.4525, 0.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 523, {1562.3150, -1710.5233, 5.4525, 0.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 523, {1564.7557, -1710.5233, 5.4525, 0.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 523, {1567.1964, -1710.5233, 5.4525, 0.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 523, {1569.6371, -1710.5233, 5.4525, 0.00}, {0, 1}
		},
		// Police Maverick
		{
			-1, FACTION_SAPD, 497, {1566.5000, -1708.0600, 28.7000,	90.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 497, {1569.6999, -1652.8000, 28.7000, 0.00}, {0, 1}
		},
		{
			-1, FACTION_SAPD, 497, {1566.4906, -1694.0710, 28.6254, 90.00}, {0, 1}
		},
		{
			-1, FACTION_MEPD, 416, {1576.6371, -1710.5233, 5.4525, 0.00}, {0, 1}
		}
};

hook OnGameModeInit()
{
	createVehicles();
    
    return 1;
}

forward createVehicles();
public createVehicles()
{
	for (new i = 0; i < sizeof(g_arrVehicleData); i++) {
		g_arrVehicleData[i][e_VehicleId] = 	AddStaticVehicleEx(
												g_arrVehicleData[i][e_VehicleModel], 
												g_arrVehicleData[i][e_VehiclePos][0], 
												g_arrVehicleData[i][e_VehiclePos][1], 
												g_arrVehicleData[i][e_VehiclePos][2], 
												g_arrVehicleData[i][e_VehiclePos][3], 
												g_arrVehicleData[i][e_VehicleColor][0], 
												g_arrVehicleData[i][e_VehicleColor][1],
												VEHICLE_RESPAWN_DELAY
											);
	}
	return 1;
}

// @TODO when we add personal vehicles add functionality to verify access to the vehicle
forward AttemptToTurnVehicleEngineOn(playerid, vehicleid);
stock AttemptToTurnVehicleEngineOn(playerid, vehicleid)
{
	if (IsVehiclePublic(vehicleid) && isVehicleFaction(vehicleid)) {
		if (!PlayerCanUseVehicleFaction(playerid, vehicleid)) {
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No tienes las llaves de este vehiculo");
		}
	}

	else if (IsVehicleJob(vehicleid)) {
		// Add conditions for job vehicles
	}

	else {
    	// Add conditions for personal vehicles
	}

	new 
        engine,
        lights,
        alarm,
        doors,
        bonnet,
        boot,
        objective,
        Float:VehHealth,
        string[128];

	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

    // If the vehicle is on turn it off
	if (engine == 1) {
		format(string, sizeof(string), "* %s apaga el motor de su vehiculo.", GetPlayerNameRP(playerid));
		ProxDetector(15.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		SetVehicleParamsEx(vehicleid, 0, lights, alarm, doors, bonnet, boot, objective);
		return 1;
	}

	GetVehicleHealth(vehicleid, VehHealth);
    
    if (VehHealth < MIN_VEHICLE_HEALTH)
        return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION]: "COLOR_WHITE_INLINE" No puedes encender el vehiculo, el motor esta demasiado dañado.");
    
    else if (GetPVarInt(playerid, "TurningEngineOn") != 0)
        return 1;

	format(string, sizeof(string), "* %s mete la llave en el contacto y lo gira.", GetPlayerNameRP(playerid));
	ProxDetector(15.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

	SetPVarInt(playerid, "TurningEngineOn", 1);
	defer TurnVehicleEngineOn(playerid, vehicleid);
	return 1;
}

forward IsVehiclePublic(vehicleid);
stock IsVehiclePublic(vehicleid)
{
    return vehicleid <= MAX_VEHICLE_PUBLIC;
}

// @TODO
forward IsVehicleJob(vehicleid);
stock IsVehicleJob(vehicleid)
{
	printf("%d", vehicleid);
    return false;
}


forward IsVehicleFaction(vehicleid);
stock isVehicleFaction(vehicleid)
{
	if(IsVehSapd(vehicleid))
        return true;
    
    return false;
}

forward PlayerCanUseVehicleFaction(playerid, vehicleid);
stock PlayerCanUseVehicleFaction(playerid, vehicleid)
{
	if(IsVehSapd(vehicleid) && GetPlayerFactionId(playerid) == FACTION_SAPD)
        return true;
    
    return false;
}

forward IsVehSapd(vehicleid);
stock IsVehSapd(vehicleid)
{
	new isVehicleIdSapd = false;

	for (new i = 0; i < sizeof(g_arrVehicleData); i++) {
		if (g_arrVehicleData[i][e_VehicleId] == vehicleid && g_arrVehicleData[i][e_VehicleFactionId] == FACTION_SAPD) {
			isVehicleIdSapd = true;
			break;
		}
	}

	return isVehicleIdSapd;
}

/* ------------- TIMERS ------------------ */

// VEHICLES 2 Seconds
timer TurnVehicleEngineOn[2000](playerid, vehicleid)
{
    new string[128];
	format(string, sizeof(string), "* %s enciende el motor de su vehiculo.", GetPlayerNameRP(playerid));
	ProxDetector(20.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

	new 
        engine,
        lights,
        alarm,
        doors,
        bonnet,
        boot,
        objective;

	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, 1, lights, alarm, doors, bonnet, boot, objective);

    DeletePVar(playerid, "TurningEngineOn");

    return 1;
}

forward IsABike(vehicleid);
stock IsABike(vehicleid)
{
	new VehicleModel = GetVehicleModel(vehicleid);

	if(VehicleModel == 510 || VehicleModel == 481 || VehicleModel == 509)
		return true;

	return false;
}