#include <YSI_Coding\y_hooks>

#include <vehicles_factions>
#include <vehicles_rentals>

hook OnGameModeInit()
{
	Vehicle_CreateFactions();
	Vehicle_CreateRentals();
    
    return 1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate)
{
	// Player entered a vehicle as a driver
    if(oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER) {
        new vehicleid = GetPlayerVehicleID(playerid);

		// Public vehicle is from a faction
		if (Vehicle_IsPublic(vehicleid) && Vehicle_IsFaction(vehicleid)) {
			if (!Vehicle_PlayerCanUseFaction(playerid, vehicleid)) {
				return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No tienes las llaves de este vehiculo");
			}
		}

		// public vehicle is from a job
		else if (Vehicle_IsJob(vehicleid)) {
			// Add conditions for job vehicles
		}
		
		// public vehicle is rental
		else if (Vehicle_IsRental(vehicleid)) {
			if (Vehicle_IsRented(vehicleid) && !Vehicle_RentalBelongsToPlayer(playerid, vehicleid)) {
				RemovePlayerFromVehicle(playerid);
				return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Esta bici ya esta siendo rentada por otro jugador");
			}

			if (Vehicle_IsRented(vehicleid) && Vehicle_RentalBelongsToPlayer(playerid, vehicleid)) {
				// @TODO
				SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION]: "COLOR_WHITE_INLINE" Estas rentando esta bici, tiempo restante de renta: "COLOR_ORANGE_INLINE" 25m");
				SendClientMessage(playerid, COLOR_ORANGE,
					"[RENTAS]: "COLOR_WHITE_INLINE" Para extender la renta "COLOR_ORANGE_INLINE" /rentar bici"\
					""COLOR_WHITE_INLINE" el precio es de "COLOR_GREEN_INLINE"$100"COLOR_WHITE_INLINE" por 30 minutos."
				);

				return 1;
			}

			SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION]: "COLOR_WHITE_INLINE" Esta bici esta a la renta "COLOR_ORANGE_INLINE" /rentar bici "COLOR_WHITE_INLINE" para rentarla");
			SendClientMessage(playerid, COLOR_ORANGE, "[RENTAS]: "COLOR_WHITE_INLINE" El precio es de "COLOR_GREEN_INLINE"$100"COLOR_WHITE_INLINE" por 30 minutos.");
			return 1;
		}
    }
    return 1;
}

stock Vehicle_IsABike(vehicleid)
{
	new VehicleModel = GetVehicleModel(vehicleid);

	if(VehicleModel == 510 || VehicleModel == 481 || VehicleModel == 509)
		return true;

	return false;
}

stock Vehicle_IsPublic(vehicleid)
{
    return vehicleid <= MAX_VEHICLE_PUBLIC;
}

// @TODO
stock Vehicle_IsJob(vehicleid)
{
	printf("%d", vehicleid);
    return false;
}

// @TODO when we add personal vehicles add functionality to verify access to the vehicle
forward Vehicle_AttemptToTurnEngineOn(playerid, vehicleid);
public Vehicle_AttemptToTurnEngineOn(playerid, vehicleid)
{
	if (Vehicle_IsPublic(vehicleid) && Vehicle_IsFaction(vehicleid)) {
		if (!Vehicle_PlayerCanUseFaction(playerid, vehicleid)) {
			return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No tienes las llaves de este vehiculo");
		}
	}

	else if (Vehicle_IsJob(vehicleid)) {
		// Add conditions for job vehicles
	}

	else {
    	// Add conditions for personal vehicles
	}

	new 
        engine,
        lights,
        alarm,
        doors,
        bonnet,
        boot,
        objective,
        Float:VehHealth,
        string[128];

	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

    // If the vehicle is on turn it off
	if (engine == 1) {
		format(string, sizeof(string), "* %s apaga el motor de su vehiculo.", GetPlayerNameRP(playerid));
		ProxDetector(15.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		SetVehicleParamsEx(vehicleid, 0, lights, alarm, doors, bonnet, boot, objective);
		return 1;
	}

	GetVehicleHealth(vehicleid, VehHealth);
    
    if (VehHealth < MIN_VEHICLE_HEALTH)
        return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION]: "COLOR_WHITE_INLINE" No puedes encender el vehiculo, el motor esta demasiado dañado.");
    
    else if (GetPVarInt(playerid, "TurningEngineOn") != 0)
        return 1;

	format(string, sizeof(string), "* %s mete la llave en el contacto y la gira.", GetPlayerNameRP(playerid));
	ProxDetector(15.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

	SetPVarInt(playerid, "TurningEngineOn", 1);
	defer Vehicle_TurnEngineOn(playerid, vehicleid);
	return 1;
}

/* ------------- TIMERS ------------------ */

// VEHICLES 2 Seconds
timer Vehicle_TurnEngineOn[2000](playerid, vehicleid)
{
    new string[128];
	format(string, sizeof(string), "* %s enciende el motor de su vehiculo.", GetPlayerNameRP(playerid));
	ProxDetector(20.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

	new 
        engine,
        lights,
        alarm,
        doors,
        bonnet,
        boot,
        objective;

	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, 1, lights, alarm, doors, bonnet, boot, objective);

    DeletePVar(playerid, "TurningEngineOn");

    return 1;
}