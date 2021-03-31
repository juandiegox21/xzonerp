CMD:motor(playerid,params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No estas en un vehiculo como conductor.");
	new vehicleid = GetPlayerVehicleID(playerid);
	if(IsABike(vehicleid)) return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Este vehiculo no tiene motor.");
    
	AttemptToTurnVehicleEngineOn(playerid, vehicleid);
	return 1;
}

CMD:chimbo(playerid, params[])
{
	GetAllRentalVehicles();
	return 1;
}

CMD:rentar(playerid,params[])
{    
    if(sscanf(params, "s[32]", params)) {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /rentar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" bici");
		return 1;
	}

    if(!strcmp(params, "bici", true, 4)) {
		new vehicleid = GetPlayerVehicleID(playerid);

		if (vehicleid == 0 || !IsVehicleRental(vehicleid)) {
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No estas en un vehiculo que se pueda rentar.");
		}

		// Vehicle is already rented but by someone else
		if (IsVehicleRented(vehicleid) && !PlayerCanUseVehicleRental(playerid, vehicleid))
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Esta bici ya esta siendo rentada por otro jugador");

		else if (IsVehicleRented(vehicleid) && PlayerCanUseVehicleRental(playerid, vehicleid)) {
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Extender?");
		}

		else if (!IsVehicleRented(vehicleid) && !PlayerCanUseVehicleRental(playerid, vehicleid) && GetPlayerRentedVehicleId(playerid) > 0 ) {
			inline DialogUnrentBike(id, dialogid, response, listitem, string: inputtext[]) {
				#pragma unused id, dialogid, listitem, inputtext
				if (!response) {
					RemovePlayerFromVehicle(playerid);
					return 1;
				}

				new RentedVehicleId = GetPlayerRentedVehicleId(playerid);

				SetPlayerRentedVehicleId(playerid, 0);
				SetVehicleRenterSqlId(RentedVehicleId, -1);

				SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION]: "COLOR_WHITE_INLINE" Has desalquilado tu bici anterior, ya puedes alquilar una nueva");

				new string[32];
				format (string, sizeof string, "bici");
				return cmd_rentar(playerid, string);
			}

			// Give the user the option to unrent a previous rented vehicle.
			Dialog_ShowCallback(playerid, using inline DialogUnrentBike, DIALOG_STYLE_MSGBOX, "Quieres desalquilar tu bici anterior?", "Ya estas rentando una bici, sin embargo puedes desalquilar tu bici anterior para alquilar esta nueva.", "Desalquilar", "Cancelar");
			return 1;
		}
		
		inline DialogRentBike(id, dialogid, response, listitem, string: inputtext[]) {
			#pragma unused id, dialogid, listitem, inputtext
			if (!response) {
				RemovePlayerFromVehicle(playerid);
				return 1;
			}

			SetPlayerRentedVehicleId(playerid, vehicleid);
			SetVehicleRenterSqlId(vehicleid, GetPlayerSqlId(playerid));
		
			SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION]: "COLOR_WHITE_INLINE" Has alquilado esta bici por 30 minutos");
			SendClientMessage(playerid, COLOR_ORANGE,
				"[RENTAS]: "COLOR_WHITE_INLINE" Para extender la renta "COLOR_ORANGE_INLINE" /rentar bici"\
				""COLOR_WHITE_INLINE" el precio es de "COLOR_GREEN_INLINE"$100"COLOR_WHITE_INLINE" por 30 minutos."
			);
			// @TODO renting a bike should remove money from the player.
		}

		Dialog_ShowCallback(playerid, using inline DialogRentBike, DIALOG_STYLE_MSGBOX, "Bici en alquiler", "Quieres rentar esta bici por 30 minutos? el precio es de "COLOR_GREEN_INLINE"$100.", "Alquilar", "Cancelar");
		return 1;
	}

	else {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /rentar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" bici");
    }

	return 1;
}	