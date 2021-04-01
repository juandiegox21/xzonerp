// @TODO We should add more bikes for rental, and also the rental vehicles should expire when they are not being rented anymore

enum e_VehicleRentalData
{
	e_VehicleId,
    e_VehicleRenterSqlId,
	e_VehicleModel,
	Float:e_VehiclePos[4], // X, Y, Z, Angle
	e_VehicleColor[2], // Color 1, Color 2
};

static const g_arrVehicleRentalData[][e_VehicleRentalData] =
{
    {
        -1, -1, 509, {1731.1541, -1862.8352, 13.0953, 0.00}, {126, 126}
    },
    {
        -1, -1, 509, {1729.3250, -1862.9210, 13.0953, 0.00}, {126, 126}
    },
    {
        -1, -1, 509, {1727.6307, -1862.8716, 13.0953, 0.00}, {126, 126}
    }
};

forward Vehicle_CreateRentals();
public Vehicle_CreateRentals()
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

stock Vehicle_IsRental(vehicleid)
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

stock Vehicle_IsRented(vehicleid)
{
    new IsVehRented = false;
	for (new i = 0; i < sizeof(g_arrVehicleRentalData); i++) {
		if 	(
                g_arrVehicleRentalData[i][e_VehicleId] == vehicleid &&
                g_arrVehicleRentalData[i][e_VehicleRenterSqlId] > -1
            ) {
			IsVehRented = true;
			break;
		}
	}
	
    return IsVehRented;
}

stock Vehicle_GetRentalIndexById(vehicleid)
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

stock Vehicle_PlayerCanUseRental(playerid, vehicleid)
{
    if (Account_GetRentedVehicleId(playerid) == vehicleid)
        return true;

    return false;
}

forward Vehicle_SetRenterSqlId(vehicleid, RenterId);
public Vehicle_SetRenterSqlId(vehicleid, RenterId)
{
    new RentalVehicleIndex = Vehicle_GetRentalIndexById(vehicleid);

    g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRenterSqlId] = RenterId;
    return 1;
}

stock Vehicle_GetAllRentals()
{
    new string[120];
    for (new i = 0; i < sizeof(g_arrVehicleRentalData); i++) {
        format(string, sizeof string, "VehicleId: %d | Renter Id: %d", g_arrVehicleRentalData[i][e_VehicleId], g_arrVehicleRentalData[i][e_VehicleRenterSqlId]);
		SendClientMessageToAll(-1, string);
	}
}