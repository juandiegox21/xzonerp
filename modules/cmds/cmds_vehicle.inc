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

		// Vehicle is not a rental
		if (!Vehicle_IsRental(vehicleid)) {
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No estas en un vehiculo que se pueda rentar.");
		}

		// Vehicle is already rented by someone else
		if (Vehicle_IsRented(vehicleid) && !Vehicle_RentalBelongsToPlayer(playerid, vehicleid))
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Esta bici ya esta siendo rentada por otro jugador");

		// Vehicle is rented to current player, which means player can extend this rental time
		if (Vehicle_IsRented(vehicleid) && Vehicle_RentalBelongsToPlayer(playerid, vehicleid)) {
			inline DialogExtendRentedBike(id, dialogid, response, listitem, string: inputtext[]) {
				#pragma unused id, dialogid, listitem, inputtext
				if (!response) {
					return 1;
				}

				new 
					index = Vehicle_GetRentalIndexById(vehicleid),
					rentTime = 30; //30m

				if (Vehicle_GetRentingTimeLeft(index) > 5) {
					new string[160];

					format(string, sizeof string, "[RENTAS] Solo puedes extender el tiempo de renta 5 minutos antes de expirarse. (Tiempo restante: %dm)", Vehicle_GetRentingTimeLeft(index));
					return SendClientMessage(playerid, COLOR_RED, string);
				}

				Vehicle_ExtendRentalTime(index, rentTime);

				SendClientMessage(playerid, COLOR_ORANGE, "[RENTAS]: "COLOR_WHITE_INLINE" Has extendido la renta de esta bici por 30 minutos mas");
				
				// @TODO extending the rent of a bike should remove money from the player.
			}

			new 
				string[170],
				index = Vehicle_GetRentalIndexById(vehicleid);

			new rentingTimeLeft = Vehicle_GetRentingTimeLeft(index);

			format(string, sizeof string, ""COLOR_WHITE_INLINE"Quieres extender la renta de esta bici por 30 minutos mas? ("COLOR_ORANGE_INLINE"tiempo restante: %dm"COLOR_WHITE_INLINE") el precio es de "COLOR_GREEN_INLINE"$100.", rentingTimeLeft);

			Dialog_ShowCallback(playerid, using inline DialogExtendRentedBike, DIALOG_STYLE_MSGBOX, "Extender renta de bici", string, "Extender", "Cancelar");
			return 1;
		}

		new RentedVehiclePlayerIndex = Vehicle_GetRentalIndexById(Account_GetRentedVehicleId(playerid));

		if  (
				Account_GetRentedVehicleId(playerid) > 0 &&
				!Vehicle_IsRented(vehicleid) &&
				vehicleid != Account_GetRentedVehicleId(playerid) &&
				Vehicle_GetRenterSqlId(RentedVehiclePlayerIndex) == Account_GetSqlId(playerid)
			) {

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

				SendClientMessage(playerid, COLOR_ORANGE, "[RENTAS]: "COLOR_WHITE_INLINE" Has desalquilado tu bici anterior, ya puedes alquilar una nueva");

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
		
			SendClientMessage(playerid, COLOR_ORANGE, "[RENTAS]: "COLOR_WHITE_INLINE" Has alquilado esta bici por 30 minutos");
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