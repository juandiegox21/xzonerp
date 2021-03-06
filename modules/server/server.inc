#include <YSI_Coding\y_hooks>

#include <server_constants>
#include <server_mysql>
#include <server_time>

hook OnGameModeInit() 
{
	MySQL_CreateConnection();

    AntiDeAMX(); //Evita desencriptar a partir del .AMX
    AddPlayerClass(2,1958.3783,1343.1572,1100.3746,269.1425,-1,-1,-1,-1,-1,-1); //Sistema de spawn
    AllowInteriorWeapons(1); //Permite armas en interiores
    DisableInteriorEnterExits(); //Desactiva entradas a locales
    EnableStuntBonusForAll(0); //Desactiva bonus por saltos y acrobacias
    ManualVehicleEngineAndLights(); //Sistema arranque de vehiculos
	
	/// enable all protections
    Protection_SetParamInt(PROTECTION_ALL, PROTECTION_ENABLED, 1);
    /// redefine name
    Protection_SetParamString(PROTECTION_TELEPORT, PROTECTION_NAME, "Teleport");

	return 1;
}

hook OnGameModeExit()
{
	// save all player data before closing connection
	for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) // GetPlayerPoolSize function was added in 0.3.7 version and gets the highest playerid currently in use on the server
	{
		if (IsPlayerConnected(i))
		{
            Kick(i);
		}
	}

	mysql_close(dbHandle);

	return 1;
}

// Avoid people clicking the spawn button
hook OnPlayerRequestClass(playerid, classid)
{
	return 0;
}

// Encrypt
static AntiDeAMX()
{
	new a[][] =
	{
		"Unarmed (Fist)",
		"Brass K"
	};
	#pragma unused a
}

// Delay kick
forward _KickPlayerDelayed(playerid);
public _KickPlayerDelayed(playerid)
{
	Kick(playerid);
	return 1;
}

DelayedKick(playerid, time = 500)
{
	SetTimerEx("_KickPlayerDelayed", time, false, "d", playerid);
	return 1;
}

forward ProxDetector(Float:radi, playerid, const string[],col1,col2,col3,col4,col5);
public ProxDetector(Float:radi, playerid, const string[],col1,col2,col3,col4,col5)
{
	new Float:posx, Float:posy, Float:posz;
	new Float:oldposx, Float:oldposy, Float:oldposz;
	new Float:tempposx, Float:tempposy, Float:tempposz;
	GetPlayerPos(playerid, oldposx, oldposy, oldposz);
	//radi = 2.0; //Trigger Radius
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(IsPlayerConnected(i) && (GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(i)))
		{
			GetPlayerPos(i, posx, posy, posz);
			tempposx = (oldposx -posx);
			tempposy = (oldposy -posy);
			tempposz = (oldposz -posz);
			//printf("DEBUG: X:%f Y:%f Z:%f",posx,posy,posz);
			if(((tempposx < radi/16) && (tempposx > -radi/16)) && ((tempposy < radi/16) && (tempposy > -radi/16)) && ((tempposz < radi/16) && (tempposz > -radi/16)))
			{

				SendClientMessage(i, col1, string);
			}
			else if(((tempposx < radi/8) && (tempposx > -radi/8)) && ((tempposy < radi/8) && (tempposy > -radi/8)) && ((tempposz < radi/8) && (tempposz > -radi/8)))
			{

				SendClientMessage(i, col2, string);
			}
			else if(((tempposx < radi/4) && (tempposx > -radi/4)) && ((tempposy < radi/4) && (tempposy > -radi/4)) && ((tempposz < radi/4) && (tempposz > -radi/4)))
			{

				SendClientMessage(i, col3, string);
			}
			else if(((tempposx < radi/2) && (tempposx > -radi/2)) && ((tempposy < radi/2) && (tempposy > -radi/2)) && ((tempposz < radi/2) && (tempposz > -radi/2)))
			{

				SendClientMessage(i, col4, string);
			}
			else if(((tempposx < radi) && (tempposx > -radi)) && ((tempposy < radi) && (tempposy > -radi)) && ((tempposz < radi) && (tempposz > -radi)))
			{

				SendClientMessage(i, col5, string);
			}
		}
	}
	return 1;
}

forward GetPlayersProx(Float:radi, playerid, targetid);
public GetPlayersProx(Float:radi, playerid, targetid)
{
	new Float:posx, Float:posy, Float:posz;
	new Float:oldposx, Float:oldposy, Float:oldposz;
	new Float:tempposx, Float:tempposy, Float:tempposz;
	GetPlayerPos(playerid, oldposx, oldposy, oldposz);
	GetPlayerPos(targetid, posx, posy, posz);
	tempposx = (oldposx -posx);
	tempposy = (oldposy -posy);
	tempposz = (oldposz -posz);
	if(((tempposx < radi) && (tempposx > -radi)) && ((tempposy < radi) && (tempposy > -radi)) && ((tempposz < radi) && (tempposz > -radi)))
	{
		return 1;
	}
	return 0;
}

forward GetPlayerNameRP(playerid);
stock GetPlayerNameRP(playerid)
{
	new NameRP[MAX_PLAYER_NAME];
	if(IsPlayerConnected(playerid))
	{
		GetPlayerName(playerid, NameRP, MAX_PLAYER_NAME);
	}
	
	for(new name = 0; name < MAX_PLAYER_NAME; name++) if(NameRP[name] == '_') NameRP[name] = ' ';
	return NameRP;
}

public OnPlayerText(playerid, text[])
{
	new string[128];
	format(string, sizeof(string), "%s dice: %s", GetPlayerNameRP(playerid), text);
	ProxDetector(15.0, playerid, string, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5);

	return 0;
}

// Disable showing player names
public OnPlayerStreamIn(playerid, forplayerid)
{
	ShowPlayerNameTagForPlayer(playerid, forplayerid, 0);
	return 1;
}