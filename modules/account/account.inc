#include <YSI_Coding\y_hooks>

// how many seconds until it kicks the player for taking too long to login
#define	 SECONDS_TO_LOGIN 		60

// default spawn point: Las Venturas (The High Roller)
#define 	DEFAULT_POS_X 		1715.6425
#define 	DEFAULT_POS_Y 		-1931.9774
#define 	DEFAULT_POS_Z 		13.7526
#define 	DEFAULT_POS_A 		0.0

// Player Account
enum E_PLAYERS
{
	ORM: pORM_ID,

	pId,
	pName[MAX_PLAYER_NAME],
	pPassword[65], // the output of SHA256_PassHash function (which was added in 0.3.7 R1 version) is always 256 bytes in length, or the equivalent of 64 Pawn cells
	pSalt[17],
	pFactionId,
	pFactionRankElevation,
	pJobId,
	Float: pPos_X,
	Float: pPos_Y,
	Float: pPos_Z,
	Float: pPos_Angle,
	pPos_Interior,

	bool: pIsLoggedIn,
	pLoginAttempts,
	pLoginTimer,

	pVehicleRentedId,
};
new Player[MAX_PLAYERS][E_PLAYERS];

new g_MysqlRaceCheck[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
	SetPlayerColor(playerid, COLOR_WHITE_TRANSPARENT);

	g_MysqlRaceCheck[playerid]++;

    // reset player data
	static const empty_player[E_PLAYERS];
	Player[playerid] = empty_player;

    GetPlayerName(playerid, Player[playerid][pName], MAX_PLAYER_NAME);

	// create orm instance and register all needed variables
	new ORM: ormid = Player[playerid][pORM_ID] = orm_create("players", MySQL_GetHandle());

	orm_addvar_int(ormid, Player[playerid][pId], "id");
	orm_addvar_string(ormid, Player[playerid][pName], MAX_PLAYER_NAME, "username");
	orm_addvar_string(ormid, Player[playerid][pPassword], 65, "password");
	orm_addvar_string(ormid, Player[playerid][pSalt], 17, "salt");
	orm_addvar_int(ormid, Player[playerid][pFactionId], "faction_id");
	orm_addvar_int(ormid, Player[playerid][pFactionRankElevation], "faction_rank_elevation");
	orm_addvar_int(ormid, Player[playerid][pJobId], "job_id");
	orm_addvar_float(ormid, Player[playerid][pPos_X], "pos_x");
	orm_addvar_float(ormid, Player[playerid][pPos_Y], "pos_y");
	orm_addvar_float(ormid, Player[playerid][pPos_Z], "pos_z");
	orm_addvar_float(ormid, Player[playerid][pPos_Angle], "pos_angle");
	orm_addvar_int(ormid, Player[playerid][pPos_Interior], "pos_interior");

	orm_addvar_int(ormid, Player[playerid][pVehicleRentedId], "vehicle_rented_id");

	orm_setkey(ormid, "username");

    // tell the orm system to load all data, assign it to our variables and call our callback when ready
	orm_load(ormid, "OnPlayerDataLoaded", "dd", playerid, g_MysqlRaceCheck[playerid]);

    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    g_MysqlRaceCheck[playerid]++;

	UpdatePlayerData(playerid, reason);

    // if the player was kicked before the time expires, kill the timer
	if (Player[playerid][pLoginTimer])
	{
		KillTimer(Player[playerid][pLoginTimer]);
		Player[playerid][pLoginTimer] = 0;
	}

	// sets "IsLoggedIn" to false when the player disconnects, it prevents from saving the player data twice when "gmx" is used
	Player[playerid][pIsLoggedIn] = false;

    return 1;
}

hook OnPlayerSpawn(playerid)
{
    // spawn the player to their last saved position
	SetPlayerInterior(playerid, Player[playerid][pPos_Interior]);
	SetPlayerPos(playerid, Player[playerid][pPos_X], Player[playerid][pPos_Y], Player[playerid][pPos_Z]);
	SetPlayerFacingAngle(playerid, Player[playerid][pPos_Angle]);

	SetCameraBehindPlayer(playerid);

	return 1;
}

static UpdatePlayerData(playerid, reason)
{
	if (Player[playerid][pIsLoggedIn] == false) return 0;

	// if the client crashed, it's not possible to get the player's position in OnPlayerDisconnect callback
	// so we will use the last saved position (in case of a player who registered and crashed/kicked, the position will be the default spawn point)
	if (reason == 1)
	{
		GetPlayerPos(playerid, Player[playerid][pPos_X], Player[playerid][pPos_Y], Player[playerid][pPos_Z]);
		GetPlayerFacingAngle(playerid, Player[playerid][pPos_Angle]);
	}

	// it is important to store everything in the variables registered in ORM instance
	Player[playerid][pPos_Interior] = GetPlayerInterior(playerid);

	// orm_save sends an UPDATE query
	orm_save(Player[playerid][pORM_ID]);
	orm_destroy(Player[playerid][pORM_ID]);
	return 1;
}

forward OnPlayerDataLoaded(playerid, race_check);
public OnPlayerDataLoaded(playerid, race_check)
{
	/*	race condition check:
		player A connects -> SELECT query is fired -> this query takes very long
		while the query is still processing, player A with playerid 2 disconnects
		player B joins now with playerid 2 -> our laggy SELECT query is finally finished, but for the wrong player
		what do we do against it?
		we create a connection count for each playerid and increase it everytime the playerid connects or disconnects
		we also pass the current value of the connection count to our OnPlayerDataLoaded callback
		then we check if current connection count is the same as connection count we passed to the callback
		if yes, everything is okay, if not, we just kick the player
	*/
	if (race_check != g_MysqlRaceCheck[playerid]) return Kick(playerid);

	orm_setkey(Player[playerid][pORM_ID], "id");

	new string[115];
	switch (orm_errno(Player[playerid][pORM_ID]))
	{
		case ERROR_OK:
		{
            inline DialogLogin(id, dialogid, response, listitem, string: inputtext[]) {
                #pragma unused id, dialogid, listitem
                
                if (!response) return Kick(playerid);
                
                new hashed_pass[65];
                SHA256_PassHash(inputtext, Player[playerid][pSalt], hashed_pass, 65);

                // If the password provided is invalid
                if (strcmp(hashed_pass, Player[playerid][pPassword]) == 0)
                {
                    //correct password, spawn the player
                    return AuthenticatePlayer(playerid);
                } else {
                    Player[playerid][pLoginAttempts]++;

                    if (Player[playerid][pLoginAttempts] >= 3)
                    {
                        Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Ingresa a tu cuenta", "Has fallado en ingresar una contraseña correcta (3 veces).", "Ok", "");
                        DelayedKick(playerid);
                    }
                    else {
                        Dialog_ShowCallback(playerid, using inline DialogLogin, DIALOG_STYLE_PASSWORD, "Ingresa a tu cuenta", "Contraseña incorrecta!\nIngresa la contraseña correcta", "Ingresar", "Cancelar");
                        return 1;
                    }
                }
            }

			format(string, sizeof string, "Esta cuenta (%s) ya esta registrada. Por favor ingresa tu contraseña:", Player[playerid][pName]);
            Dialog_ShowCallback(playerid, using inline DialogLogin, DIALOG_STYLE_PASSWORD, "Ingresa en tu cuenta", string, "Ingresar", "Cancelar");

            // from now on, the player has a limited time to login
			Player[playerid][pLoginTimer] = SetTimerEx("OnLoginTimeout", SECONDS_TO_LOGIN * 1000, false, "d", playerid);
		}
		case ERROR_NO_DATA:
		{
            inline DialogRegister(id, dialogid, response, listitem, string: inputtext[]) {
                #pragma unused id, dialogid, listitem, inputtext

                if (!response) return Kick(playerid);

                if (strlen(inputtext) <= 5) {
                    Dialog_ShowCallback(playerid, using inline DialogRegister, DIALOG_STYLE_PASSWORD, "Registro", "Tu contraseña debe ser de al menos 5 caracteres!\nPor favor intenta de nuevo", "Registrar", "Cancelar");

                    return 1;
                }

                // 16 random characters from 33 to 126 (in ASCII) for the salt
                for (new i = 0; i < 16; i++) Player[playerid][pSalt][i] = random(94) + 33;
                SHA256_PassHash(inputtext, Player[playerid][pSalt], Player[playerid][pPassword], 65);
                
                // Get the DateTime to keep a record of when the account was created
                orm_addvar_string(Player[playerid][pORM_ID], getDateTime(), 32, "created_at");
                // sends an INSERT query
                orm_save(Player[playerid][pORM_ID], "OnPlayerRegister", "d", playerid);
            }

			format(string, sizeof string, "Bienvenido %s, para registrar esta cuenta, por favor ingresa una contraseña:", Player[playerid][pName]);
            Dialog_ShowCallback(playerid, using inline DialogRegister, DIALOG_STYLE_PASSWORD, "Registro", string, "Registrar", "Cancelar");
		}
	}

	return 1;
}

forward OnPlayerRegister(playerid);
public OnPlayerRegister(playerid)
{
	SendClientMessage(playerid, COLOR_ORANGE, "Tu cuenta ha sido registrada exitosamente, bienvenido!");

	Player[playerid][pIsLoggedIn] = true;

	Player[playerid][pPos_X] = DEFAULT_POS_X;
	Player[playerid][pPos_Y] = DEFAULT_POS_Y;
	Player[playerid][pPos_Z] = DEFAULT_POS_Z;
	Player[playerid][pPos_Angle] = DEFAULT_POS_A;

	SetSpawnInfo(playerid, NO_TEAM, 0, Player[playerid][pPos_X], Player[playerid][pPos_Y], Player[playerid][pPos_Z], Player[playerid][pPos_Angle], 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	
	return 1;
}

static AuthenticatePlayer(playerid)
{
    SendClientMessage(playerid, COLOR_ORANGE, "Bienvenido de vuelta!");

    KillTimer(Player[playerid][pLoginTimer]);
    Player[playerid][pLoginTimer] = 0;
    Player[playerid][pIsLoggedIn] = true;

    // spawn the player to their last saved position after login
    SetSpawnInfo(playerid, NO_TEAM, 0, Player[playerid][pPos_X], Player[playerid][pPos_Y], Player[playerid][pPos_Z], Player[playerid][pPos_Angle], 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);

    return 1;
}

forward OnLoginTimeout(playerid);
public OnLoginTimeout(playerid)
{
    // reset the variable that stores the timerid
	Player[playerid][pLoginTimer] = 0;

	Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Ingresa en tu cuenta", "Has sido expulsado por tardarte mucho en ingresar una contraseña, vuelve a entrar para intentar de nuevo.", "Ok", "");
	DelayedKick(playerid);
	return 1;
}

stock Account_GetSqlId(playerid) 
{
	return Player[playerid][pId];
}

stock Account_GetJobId(playerid) {
	return Player[playerid][pJobId];
}

//@TODO
stock Account_GetJobName(playerid)
{
	return Player[playerid][pJobId];
}

stock Account_GetFactionId(playerid)
{
	return Player[playerid][pFactionId];
}

stock Account_GetFactionRankElevation(playerid)
{
	return Player[playerid][pFactionRankElevation];
}

forward Account_SetFactionId(playerid, FactionId);
public Account_SetFactionId(playerid, FactionId)
{
	if (!Faction_IsValid(FactionId)) return SendClientMessage(playerid, COLOR_RED, "[ATENCION] esa faccion no existe");

	Player[playerid][pFactionId] = FactionId;

	// Reset rank if faction was set to civil
	if (Faction_IsCivil(FactionId)) {
		Player[playerid][pFactionRankElevation] = 0;
	}

	orm_save(Player[playerid][pORM_ID]);

	return 1;
}

forward Account_SetFactionRankElevation(playerid, Elevation);
public Account_SetFactionRankElevation(playerid, Elevation)
{
	Player[playerid][pFactionRankElevation] = Elevation;
	orm_save(Player[playerid][pORM_ID]);
	return true;
}

stock Account_Faction_GetName(playerid)
{
	new PlayerFactionId = Account_GetFactionId(playerid);
	new FactionNameString[42];

	FactionNameString = Faction_GetName(PlayerFactionId);
	return FactionNameString;
}

stock Account_IsFactionLeader(playerid)
{
	return Account_GetFactionRankElevation(playerid) == 4;
}

stock Account_GetRentedVehicleId(playerid)
{
	return Player[playerid][pVehicleRentedId];
}

forward Account_SetRentedVehicleId(playerid, vehicleid);
public Account_SetRentedVehicleId(playerid, vehicleid)
{
	Player[playerid][pVehicleRentedId] = vehicleid;
	orm_save(Player[playerid][pORM_ID]);

	return 1;
}

stock IsPlayerLoggedIn(playerid)
{
	return Player[playerid][pIsLoggedIn];
}