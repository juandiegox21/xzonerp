// @TODO We should add more bikes for rental, and also the rental vehicles should expire when they are not being rented anymore

#define RENTAL_EXPIRATION_TIME_IN_MINS 1 // 30m

enum e_VehicleRentalData
{
	e_VehicleId,
    e_VehicleRenterSqlId,
    e_VehicleRentalExpiration,
	e_VehicleModel,
	Float:e_VehiclePos[4], // X, Y, Z, Angle
	e_VehicleColor[2], // Color 1, Color 2
};

static const g_arrVehicleRentalData[][e_VehicleRentalData] =
{
    {
        -1, -1, 0, 509, {1731.1541, -1862.8352, 13.0953, 0.00}, {126, 126}
    },
    {
        -1, -1, 0, 509, {1729.3250, -1862.9210, 13.0953, 0.00}, {126, 126}
    },
    {
        -1, -1, 0, 509, {1727.6307, -1862.8716, 13.0953, 0.00}, {126, 126}
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

    repeat CheckRentalExpirationTimer();
    return 1;
}

stock Vehicle_IsRental(vehicleid)
{
	new IsVehRental = false;

	for (new i = 0; i < sizeof(g_arrVehicleRentalData); i++) {
		if 	(g_arrVehicleRentalData[i][e_VehicleId] == vehicleid) {
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
                g_arrVehicleRentalData[i][e_VehicleRenterSqlId] > 0
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

stock Vehicle_RentalBelongsToPlayer(playerid, vehicleid)
{
    if (Account_GetRentedVehicleId(playerid) == vehicleid)
        return true;

    return false;
}

forward Vehicle_SetRenterSqlId(index, RenterSqlId);
public Vehicle_SetRenterSqlId(index, RenterSqlId)
{
    // Renter
    g_arrVehicleRentalData[index][e_VehicleRenterSqlId] = RenterSqlId;
    
    // If the renter is invalid
    if (RenterSqlId < 1) {
        // Respawn vehicle
        SetVehicleToRespawn(g_arrVehicleRentalData[index][e_VehicleId]);
        // Reset expiration time
        g_arrVehicleRentalData[index][e_VehicleRentalExpiration] = 0;
        return 1;
    }

    // Set Expiration time
    g_arrVehicleRentalData[index][e_VehicleRentalExpiration] = RENTAL_EXPIRATION_TIME_IN_MINS;

    return 1;
}

forward Vehicle_ExtendRentalTime(index, RentTime);
public Vehicle_ExtendRentalTime(index, RentTime)
{
    g_arrVehicleRentalData[index][e_VehicleRentalExpiration] += RentTime;
}

stock Vehicle_GetRenterSqlId(index)
{
    return  g_arrVehicleRentalData[index][e_VehicleRenterSqlId];
}

stock Vehicle_GetRentingTimeLeft(index)
{
    return  g_arrVehicleRentalData[index][e_VehicleRentalExpiration];
}

stock Vehicle_GetAllRentals()
{
    new 
        string[500];
    for (new i = 0; i < sizeof(g_arrVehicleRentalData); i++) {
        new 
            playerid = Account_GetPlayerIdBySqlId(g_arrVehicleRentalData[i][e_VehicleRenterSqlId]);

        format(string, sizeof string, "VehicleId: %d | Renter sqlId: %d | Renter playerid: %d| Expiration Time: %d", g_arrVehicleRentalData[i][e_VehicleId], g_arrVehicleRentalData[i][e_VehicleRenterSqlId], playerid, g_arrVehicleRentalData[i][e_VehicleRentalExpiration]);
		SendClientMessageToAll(-1, string);
	}
}

/* ------------- TIMERS ------------------ */
// @TODO  refactor this is a mess
// Runs every 1 minute
timer CheckRentalExpirationTimer[60000]()
{
    for (new i = 0; i < sizeof g_arrVehicleRentalData; i++) {
        // Vehicle is not being rented anymore
        if (g_arrVehicleRentalData[i][e_VehicleRenterSqlId] < 1 && g_arrVehicleRentalData[i][e_VehicleRentalExpiration] > 0) {
            // Reset vehicle rental expiration
            Vehicle_SetRenterSqlId(i, -1);
        }

        // Vehicle is being rented and rented time is up
        if  (g_arrVehicleRentalData[i][e_VehicleRenterSqlId] > 0 && g_arrVehicleRentalData[i][e_VehicleRentalExpiration] < 1) {
            new playerid = Account_GetPlayerIdBySqlId(g_arrVehicleRentalData[i][e_VehicleRenterSqlId]);

            if (playerid > -1) {
                SendClientMessage(playerid, COLOR_ORANGE, "[RENTAS] "COLOR_WHITE_INLINE"El tiempo de renta de tu bici ha expirado");
            }
            
            Vehicle_SetRenterSqlId(i, -1);
        }

        // Vehicle is being rented and the rented time is not up yet
        if  (g_arrVehicleRentalData[i][e_VehicleRenterSqlId] > 0 && g_arrVehicleRentalData[i][e_VehicleRentalExpiration] > 0) {
            new playerid = Account_GetPlayerIdBySqlId(g_arrVehicleRentalData[i][e_VehicleRenterSqlId]);
            // 300 seconds = 5 minutes
            // 240 seconds = 4 mins
            // When the expiration is less than 5 mins but more than 4 mins, advise the player to extend their rent.
            if  (
                    playerid > -1 &&
                    g_arrVehicleRentalData[i][e_VehicleRentalExpiration] <= 5 &&
                    g_arrVehicleRentalData[i][e_VehicleRentalExpiration] > 4
                ) {
                SendClientMessage(playerid, COLOR_ORANGE, "[RENTAS] "COLOR_WHITE_INLINE"El tiempo de renta de tu bici expirara en 5 minutos");
            }

            // Substract 1 min
            g_arrVehicleRentalData[i][e_VehicleRentalExpiration] -= 1;
        }
    }
}