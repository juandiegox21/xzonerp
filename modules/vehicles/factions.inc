enum e_VehicleFactionData
{
	e_VehicleId,
	e_VehicleFactionId,
	e_VehicleModel,
	Float:e_VehiclePos[4], // X, Y, Z, Angle
	e_VehicleColor[2], // Color 1, Color 2
};

static const g_arrVehicleFactionData[][e_VehicleFactionData] =
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

forward CreateFactionVehicles();
public CreateFactionVehicles()
{
	for (new i = 0; i < sizeof(g_arrVehicleFactionData); i++) {
		g_arrVehicleFactionData[i][e_VehicleId] = 	AddStaticVehicleEx(
                                                        g_arrVehicleFactionData[i][e_VehicleModel], 
                                                        g_arrVehicleFactionData[i][e_VehiclePos][0], 
                                                        g_arrVehicleFactionData[i][e_VehiclePos][1], 
                                                        g_arrVehicleFactionData[i][e_VehiclePos][2], 
                                                        g_arrVehicleFactionData[i][e_VehiclePos][3], 
                                                        g_arrVehicleFactionData[i][e_VehicleColor][0], 
                                                        g_arrVehicleFactionData[i][e_VehicleColor][1],
                                                        VEHICLE_RESPAWN_DELAY
                                                    );
	}
    return 1;
}

forward IsVehicleFaction(vehicleid);
stock isVehicleFaction(vehicleid)
{
	new IsVehFaction = false;
	for (new i = 0; i < sizeof(g_arrVehicleFactionData); i++) {
		if 	(
				g_arrVehicleFactionData[i][e_VehicleId] == vehicleid && 
				g_arrVehicleFactionData[i][e_VehicleFactionId] > 0
			) {

			IsVehFaction = true;
			break;
		}
	}
	
    return IsVehFaction;
}

forward IsVehicleFromFaction(vehicleid, FactionId);
stock IsVehicleFromFaction(vehicleid, FactionId)
{
	new isVehFromFaction = false;

	for (new i = 0; i < sizeof(g_arrVehicleFactionData); i++) {
		if 	(
				g_arrVehicleFactionData[i][e_VehicleId] == vehicleid && 
				g_arrVehicleFactionData[i][e_VehicleFactionId] == FactionId
			) {

			isVehFromFaction = true;
			break;
		}
	}

	return isVehFromFaction;
}

forward PlayerCanUseVehicleFaction(playerid, vehicleid);
stock PlayerCanUseVehicleFaction(playerid, vehicleid)
{
	// SAPD
	if	(
			IsVehicleFromFaction(vehicleid, FACTION_SAPD) && 
			GetPlayerFactionId(playerid) == FACTION_SAPD
		)
        return true;

	// MEPD
	if	(
			IsVehicleFromFaction(vehicleid, FACTION_MEPD) && 
			GetPlayerFactionId(playerid) == FACTION_MEPD
		)
        return true;
    
    return false;
}