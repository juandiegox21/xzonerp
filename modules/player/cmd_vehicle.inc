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

		// The player is already renting a vehicle but it's not this vehicle
		if (GetPlayerRentedVehicleId(playerid) > 0 && !PlayerCanUseVehicleRental(playerid, vehicleid)) {
			inline DialogUnrentBike(id, dialogid, response, listitem, string: inputtext[]) {
				#pragma unused id, dialogid, listitem, inputtext
				if (!response)
					return 1;

				new rentedVehicleId = GetPlayerRentedVehicleId(playerid);

				SetPlayerRentedVehicleId(playerid, 0);
				SetVehicleRentedStatus(rentedVehicleId, false);

				SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION]: "COLOR_WHITE_INLINE" Has desalquilado tu bici anterior, ya puedes alquilar una nueva");
				return cmd_rentar(playerid,params);
			}

			// Give the user the option to unrent a previous rented vehicle.
			Dialog_ShowCallback(playerid, using inline DialogUnrentBike, DIALOG_STYLE_MSGBOX, "Quieres desalquilar tu bici anterior?", "Ya estas rentando una bici, sin embargo puedes desalquilar tu bici anterior para alquilar esta nueva.", "Desalquilar", "Cancelar");
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