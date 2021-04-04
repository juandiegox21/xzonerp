CMD:motor(playerid,params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No estas en un vehiculo como conductor.");
	new vehicleid = GetPlayerVehicleID(playerid);
	if(Vehicle_IsABike(vehicleid)) return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Este vehiculo no tiene motor.");
    
	Vehicle_AttemptToTurnEngineOn(playerid, vehicleid);
	return 1;
}

CMD:chimbo(playerid, params[])
{
	Vehicle_GetAllRentals();
	return 1;
}


// @TODO refactor command, this is imposible to read.
CMD:rentar(playerid,params[])
{    
    if(sscanf(params, "s[32]", params)) {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /rentar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" bici");
		return 1;
	}

    if(!strcmp(params, "bici", true, 4)) {
		new vehicleid = GetPlayerVehicleID(playerid);

		if (vehicleid == 0 || !Vehicle_IsRental(vehicleid)) {
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No estas en un vehiculo que se pueda rentar.");
		}

		// Vehicle is already rented but by someone else
		if (Vehicle_IsRented(vehicleid) && !Vehicle_PlayerCanUseRental(playerid, vehicleid))
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Esta bici ya esta siendo rentada por otro jugador");

		else if (Vehicle_IsRented(vehicleid) && Vehicle_PlayerCanUseRental(playerid, vehicleid)) {
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Extender?");
		}

		else if (!Vehicle_IsRented(vehicleid) && !Vehicle_PlayerCanUseRental(playerid, vehicleid) && Account_GetRentedVehicleId(playerid) > 0 ) {
			inline DialogUnrentBike(id, dialogid, response, listitem, string: inputtext[]) {
				#pragma unused id, dialogid, listitem, inputtext
				if (!response) {
					RemovePlayerFromVehicle(playerid);
					return 1;
				}

				new 
					RentedVehicleId = Account_GetRentedVehicleId(playerid),
					RentedVehicleIndex = Vehicle_GetRentalIndexById(RentedVehicleId);

				Account_SetRentedVehicleId(playerid, 0);
				Vehicle_SetRenterSqlId(RentedVehicleIndex, -1);

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

			Account_SetRentedVehicleId(playerid, vehicleid);

			new RentedVehicleIndex = Vehicle_GetRentalIndexById(vehicleid);
			Vehicle_SetRenterSqlId(RentedVehicleIndex, Account_GetSqlId(playerid));
		
			SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION]: "COLOR_WHITE_INLINE" Has alquilado esta bici por 30 minutos");
			SendClientMessage(playerid, COLOR_ORANGE,
				"[RENTAS]: "COLOR_WHITE_INLINE" Para extender la renta "COLOR_ORANGE_INLINE" /rentar bici"\
				""COLOR_WHITE_INLINE" el precio es de "COLOR_GREEN_INLINE"$100"COLOR_WHITE_INLINE" por 30 minutos."
			);

			// The bike will now move
			new alarm,doors,bonnet,boot,objective;
			SetVehicleParamsEx(vehicleid, 1, 0, alarm, doors, bonnet, boot, objective);

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