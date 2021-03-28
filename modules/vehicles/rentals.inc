enum e_VehicleRentalData
{
	e_VehicleId,
    bool:e_VehicleRented,
	e_VehicleModel,
	Float:e_VehiclePos[4], // X, Y, Z, Angle
	e_VehicleColor[2], // Color 1, Color 2
};

static const g_arrVehicleRentalData[][e_VehicleRentalData] =
{
    {
        -1, false, 509, {1731.1541, -1862.8352, 13.0953, 0.00}, {126, 126}
    },
    {
        -1, false, 509, {1729.3250, -1862.9210, 13.0953, 0.00}, {126, 126}
    },
    {
        -1, false, 509, {1727.6307, -1862.8716, 13.0953, 0.00}, {126, 126}
    }
};

forward CreateRentalVehicles();
public CreateRentalVehicles()
{
	for (new i = 0; i < sizeof(g_arrVehicleRentalData); i++) {
		g_arrVehicleRentalData[i][e_VehicleId] = 	AddStaticVehicleEx(
                                                        g_arrVehicleRentalData[i][e_VehicleModel], 
                                                        g_arrVehicleRentalData[i][e_VehiclePos][0], 
                                                        g_arrVehicleRentalData[i][e_VehiclePos][1], 
                                                        g_arrVehicleRentalData[i][e_VehiclePos][2], 
                                                        g_arrVehicleRentalData[i][e_VehiclePos][3], 
                                                        g_arrVehicleRentalData[i][e_VehicleColor][0], 
                                                        g_arrVehicleRentalData[i][e_VehicleColor][1],
                                                        VEHICLE_RESPAWN_DELAY
                                                    );
	}
    return 1;
}

forward IsVehicleRental(vehicleid);
stock IsVehicleRental(vehicleid)
{
	new IsVehRental = false;
	for (new i = 0; i < sizeof(g_arrVehicleRentalData); i++) {
		if 	(
				g_arrVehicleRentalData[i][e_VehicleId] == vehicleid
			) {

			IsVehRental = true;
			break;
		}
	}
	
    return IsVehRental;
}

forward IsVehicleRented(vehicleid);
stock IsVehicleRented(vehicleid)
{
    new IsVehRented = false;
	for (new i = 0; i < sizeof(g_arrVehicleRentalData); i++) {
		if 	(
                g_arrVehicleRentalData[i][e_VehicleId] == vehicleid &&
                g_arrVehicleRentalData[i][e_VehicleRented] == true
            ) {
			IsVehRented = true;
			break;
		}
	}
	
    return IsVehRented;
}

forward GetVehicleRentalIndexById(vehicleid);
stock GetVehicleRentalIndexById(vehicleid)
{
    new Index = -1;

    for (new i = 0; i < sizeof(g_arrVehicleRentalData); i++) {
		if 	(g_arrVehicleRentalData[i][e_VehicleId] == vehicleid) {
			Index = i;
			break;
		}
	}

    return Index;
}

forward PlayerCanUseVehicleRental(playerid, vehicleid);
stock PlayerCanUseVehicleRental(playerid, vehicleid)
{
    if (GetPlayerRentedVehicleId(playerid) == vehicleid)
        return true;

    return false;
}

forward SetVehicleRentedStatus(vehicleid, bool:isRented);
public SetVehicleRentedStatus(vehicleid, bool:isRented)
{
    new RentalVehicleIndex = GetVehicleRentalIndexById(vehicleid);

    g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRented] = isRented;
    return 1;
}