CMD:motor(playerid,params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No estas en un vehiculo como conductor.");
	new vehicleid = GetPlayerVehicleID(playerid);
	if(IsABike(vehicleid)) return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Este vehiculo no tiene motor.");
    
	AttemptToTurnVehicleEngineOn(playerid, vehicleid);
	return 1;
}

CMD:rentar(playerid,params[])
{
	if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    if(sscanf(params, "s[32]", params)) {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /rentar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" bici");
		return 1;
	}

    if(!strcmp(params, "bici", true, 4)) {
		new vehicleid = GetPlayerVehicleID(playerid);

		// Vehicle is already rented
		if (IsVehicleRented(playerid)) {
			if (!PlayerCanUseVehicleRental(playerid, vehicleid)) {
				return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Esta bici ya esta siendo rentada por otro jugador");
			}

			// @TODO Player is already renting this vehicle extend?
			return 1;
		}

		// The player is already renting a vehicle but it's not this one
		if (GetPlayerRentedVehicleId(playerid) > 0 && !PlayerCanUseVehicleRental(playerid, vehicleid)) {
			// @TODO You are already renting a vehicle but it is not this one, unrent it so you can rent this one?
			// Dialog_ShowCallback(playerid, using inline DialogUnrentBike, DIALOG_STYLE_MSGBOX, "Ya estas rentando una bici", "", "Desalquilar", "Cancelar");
			
			return 1;
		}

		// @TODO you have rented the vehicle
		return 1;
	}

	else {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /rentar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" bici");
    }

	return 1;
}	