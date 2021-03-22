#include <YSI_Coding\y_hooks>

hook OnGameModeInit()
{
    /* ------------------ LSPD ------------------*/
	AddStaticVehicleEx(596,1545.54943848,-1651.02917480,5.71062469,90.00000000,0,0,3600); //Police Car (LSPD) 15
	AddStaticVehicleEx(596,1545.54943848,-1655.07836914,5.71062469,90.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1545.54943848,-1659.02734375,5.71062469,90.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1545.54943848,-1663.03979492,5.71062469,90.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1545.54943848,-1667.89807129,5.71062469,90.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1545.54943848,-1671.97827148,5.71062469,90.00000000,0,1,3600); //Police Car (LSPD) 20
	AddStaticVehicleEx(596,1545.54943848,-1676.07922363,5.71062469,90.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1545.54943848,-1684.37084961,5.74014664,90.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1545.54943848,-1680.22387695,5.74014664,90.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(523,1557.4336,-1710.5233,5.4525,0.00000000,0,1,3600); //HPV1000
	AddStaticVehicleEx(523,1559.8743,-1710.5233,5.4525,0.00000000,0,1,3600); //HPV1000 25
	AddStaticVehicleEx(523,1562.3150,-1710.5233,5.4525,0.00000000,0,1,3600); //HPV1000
	AddStaticVehicleEx(523,1564.7557,-1710.5233,5.4525,0.00000000,0,1,3600); //HPV1000
	AddStaticVehicleEx(523,1567.1964,-1710.5233,5.4525,0.00000000,0,1,3600); //HPV1000
	AddStaticVehicleEx(523,1569.6371,-1710.5233,5.4525,0.00000000,0,1,3600); //HPV1000
	AddStaticVehicleEx(497,1566.50000000,-1708.06005859,28.70000076,90.00000000,0,1,3600); //Police Maverick 30
	AddStaticVehicleEx(497,1569.69995117,-1652.80004883,28.70000076,0.00000000,0,1,3600); //Police Maverick
	AddStaticVehicleEx(487,1566.4906,-1694.0710,28.6254,90.00000000,0,0,3600); //Maverick
	AddStaticVehicleEx(596,1526.49572754,-1643.98303223,6.27575159,180.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1530.46606445,-1643.98303223,6.27575159,180.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1534.69177246,-1643.98303223,6.27575159,180.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1538.71667480,-1643.98303223,6.27575159,180.00000000,0,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(525,1591.4323,-1711.0713,5.7688,0.00000000,0,1,3600); //Tow Truck
	AddStaticVehicleEx(525,1595.4498,-1711.0713,5.7680,0.00000000,0,1,3600); //Tow Truck
	AddStaticVehicleEx(599,1602.6211,-1691.9659,6.0790,90.00000000,0,1,3600); //Police Ranger 45
	AddStaticVehicleEx(599,1602.6211,-1687.8544,6.1430,90.00000000,0,1,3600); //Police Ranger
	AddStaticVehicleEx(599,1602.6211,-1683.9187,6.1060,90.00000000,0,1,3600); //Police Ranger
	AddStaticVehicleEx(599,1602.6211,-1696.0380,6.1060,90.00000000,0,1,3600); //Police Ranger
	AddStaticVehicleEx(596,1528.7050,-1688.0760,5.6593,270.00000000,1,1,3600); //Police Car (LSPD)
	AddStaticVehicleEx(596,1528.7050,-1683.9211,5.6593,270.00000000,1,1,3600); //Police Car (LSPD)
    
    return 1;
}

// @TODO when we add personal vehicles add functionality to verify access to the car
forward AttemptToTurnVehicleEngineOn(playerid, CarId);
stock AttemptToTurnVehicleEngineOn(playerid, CarId)
{
	new 
        engine,
        lights,
        alarm,
        doors,
        bonnet,
        boot,
        objective,
        Float:CarHealth,
        string[128];

	GetVehicleParamsEx(CarId, engine, lights, alarm, doors, bonnet, boot, objective);

    // If the vehicle is on turn it off
	if (engine == 1) {
		format(string, sizeof(string), "* %s apaga el motor de su vehiculo.", GetPlayerNameRP(playerid));
		ProxDetector(15.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
		SetVehicleParamsEx(CarId, 0, lights, alarm, doors, bonnet, boot, objective);
		return 1;
	}

	GetVehicleHealth(CarId, CarHealth);
    
    if (CarHealth < MIN_CAR_HEALTH)
        return SendClientMessage(playerid, COLOR_WHITE, "[ATENCION]: No puedes encender el vehiculo, el motor esta demasiado dañado.");
    
    else if (GetPVarInt(playerid, "TurningEngineOn") != 0)
        return 1;

    else if (IsVehiclePublic(CarId))
    {
        if (!PlayerCanUseVehicleFromFaction(playerid, CarId))
            return 1;

        format(string, sizeof(string), "* %s mete la llave en el contacto y lo gira.", GetPlayerNameRP(playerid));
        ProxDetector(15.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

        SetPVarInt(playerid, "TurningEngineOn", 1);
        defer TurnVehicleEngineOn(playerid, CarId);
        return 1;
    }
    
    // Add stuff for personal vehicles

	return 1;
}

forward IsVehiclePublic(CarId);
stock IsVehiclePublic(CarId)
{
    return CarId <= MAX_CAR_PUBLIC;
}

forward PlayerCanUseVehicleFromFaction(playerid, vehicleid);
stock PlayerCanUseVehicleFromFaction(playerid, vehicleid)
{
	if(IsCarSapd(vehicleid) && GetPlayerFactionId(playerid) == FACTION_SAPD)
        return true;
    
    return false;
}

forward IsCarSapd(carid);
stock IsCarSapd(carid)
{
	if(carid >= 1 && carid <= 50)
        return 1;

	return 0;
}

/* ------------- TIMERS ------------------ */

// VEHICLES 2 Seconds
timer TurnVehicleEngineOn[2000](playerid, CarId)
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

	GetVehicleParamsEx(CarId, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(CarId, 1, lights, alarm, doors, bonnet, boot, objective);

    DeletePVar(playerid, "TurningEngineOn");

    return 1;
}

forward IsABike(CarId);
stock IsABike(CarId)
{
	new VehicleModel = GetVehicleModel(CarId);

	if(VehicleModel == 510 || VehicleModel == 481 || VehicleModel == 509)
		return true;

	return false;
}