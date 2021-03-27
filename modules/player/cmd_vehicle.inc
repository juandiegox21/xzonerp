CMD:motor(playerid,params[])
{
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No estas en un vehiculo como conductor.");
	new vehicleid = GetPlayerVehicleID(playerid);
	if(IsABike(vehicleid)) return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Este vehiculo no tiene motor.");
    
	AttemptToTurnVehicleEngineOn(playerid, vehicleid);
	return 1;
}
