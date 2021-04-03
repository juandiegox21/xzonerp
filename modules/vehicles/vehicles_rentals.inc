// @TODO We should add more bikes for rental, and also the rental vehicles should expire when they are not being rented anymore

#define RENTAL_EXPIRATION_TIME_IN_SECONDS 20 // 30m

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

static
	Timer:RentalExpirationTimer[sizeof g_arrVehicleRentalData];

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

forward Vehicle_SetRenterSqlId(vehicleid, RenterSqlId);
public Vehicle_SetRenterSqlId(vehicleid, RenterSqlId)
{
    new RentalVehicleIndex = Vehicle_GetRentalIndexById(vehicleid);
    // Renter

    g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRenterSqlId] = RenterSqlId;
    
    // If the renter is invalid
    if (RenterSqlId == -1) {
        // Respawn vehicle
        SetVehicleToRespawn(g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleId]);
        // Reset expiration time
        g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRentalExpiration] = 0;
        return 1;
    }

    // Set Expiration time
    g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRentalExpiration] = RENTAL_EXPIRATION_TIME_IN_SECONDS;
    // Timer to check if expiration time is up
    RentalExpirationTimer[RentalVehicleIndex] = repeat CheckRentalExpirationTimer(RentalVehicleIndex);    

    return 1;
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
// Runs every second
timer CheckRentalExpirationTimer[1000](RentalVehicleIndex)
{
    // Vehicle is not being rented anymore
    if (g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRenterSqlId] == -1) {
        // Reset vehicle rental expiration
        g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRentalExpiration] = 0;

        // Stop this timer
        stop RentalExpirationTimer[RentalVehicleIndex];
    }

    // Vehicle is being rented and rented time is up
    if  (
            g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRenterSqlId] > 0 && 
            g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRentalExpiration] < 1
        ) {
            new playerid = Account_GetPlayerIdBySqlId(g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRenterSqlId]);

            if (playerid > -1) {
                SendClientMessage(playerid, COLOR_ORANGE, "[RENTAS] "COLOR_WHITE_INLINE"El tiempo de renta de tu bici ha expirado");
            }

            // Reset vehicle rental expiration
            g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRentalExpiration] = 0;
            // Set vehicle renter to invalid
            g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRenterSqlId] = -1;

            // Respawn vehicle
            SetVehicleToRespawn(g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleId]);

            // Stop this timer
            stop RentalExpirationTimer[RentalVehicleIndex];
    }

    // Vehicle is being rented and the rented time is not up yet
    if  (
            g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRenterSqlId] > 0 && 
            g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRentalExpiration] > 0
        ) {
            new playerid = Account_GetPlayerIdBySqlId(g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRenterSqlId]);

            if (playerid > -1 && g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRentalExpiration] == 5) {
                SendClientMessage(playerid, COLOR_ORANGE, "[RENTAS] "COLOR_WHITE_INLINE"El tiempo de renta de tu bici expirara en 5 minutos, vuelve a tu bici para extender el tiempo de renta.");
            }

            // Substract 1 second
            g_arrVehicleRentalData[RentalVehicleIndex][e_VehicleRentalExpiration] -= 1;
    }
}