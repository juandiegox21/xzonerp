enum e_VehicleJobData
{
	e_VehicleId,
	e_VehicleJobId,
	e_VehicleModel,
	Float:e_VehiclePos[4], // X, Y, Z, Angle
	e_VehicleColor[2], // Color 1, Color 2
};

static const g_arrVehicleJobData[][e_VehicleJobData] =
{
    // ================== TRUCKERS ==================
    {
        -1, JOB_TRUCKER, 499, {2455.14282, -2113.93774, 13.65593, 90.00}, {0, 1}
    }
};

forward Vehicle_CreateJobs();
public Vehicle_CreateJobs()
{
	for (new i = 0; i < sizeof(g_arrVehicleJobData); i++) {
		g_arrVehicleJobData[i][e_VehicleId] = 	AddStaticVehicleEx(
                                                    g_arrVehicleJobData[i][e_VehicleModel], 
                                                    g_arrVehicleJobData[i][e_VehiclePos][0], 
                                                    g_arrVehicleJobData[i][e_VehiclePos][1], 
                                                    g_arrVehicleJobData[i][e_VehiclePos][2], 
                                                    g_arrVehicleJobData[i][e_VehiclePos][3], 
                                                    g_arrVehicleJobData[i][e_VehicleColor][0], 
                                                    g_arrVehicleJobData[i][e_VehicleColor][1],
                                                    VEHICLE_RESPAWN_DELAY
                                                );
	}
    return 1;
}

stock Vehicle_IsJob(vehicleid)
{
    new IsVehJob = false;

	for (new i = 0; i < sizeof(g_arrVehicleJobData); i++) {
		if 	(
				g_arrVehicleJobData[i][e_VehicleId] == vehicleid
			) {

			IsVehJob = true;
			break;
		}
	}
	
    return IsVehJob;
}

stock Vehicle_IsFromJob(vehicleid, jobId)
{
	new isVehFromJob = false;

	for (new i = 0; i < sizeof(g_arrVehicleJobData); i++) {
		if 	(
				g_arrVehicleJobData[i][e_VehicleId] == vehicleid && 
				g_arrVehicleJobData[i][e_VehicleJobId] == jobId
			) {

			isVehFromJob = true;
			break;
		}
	}

	return isVehFromJob;
}

stock Vehicle_PlayerCanUseJob(playerid, vehicleid)
{
	// TRUCKER
	if	(
			Vehicle_IsFromJob(vehicleid, JOB_TRUCKER) && 
			Account_GetJobId(playerid) == JOB_TRUCKER
		)

        return true;

    return false;
}