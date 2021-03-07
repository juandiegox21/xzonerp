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
	ORM: ORM_ID,

	ID,
	Name[MAX_PLAYER_NAME],
	Password[65], // the output of SHA256_PassHash function (which was added in 0.3.7 R1 version) is always 256 bytes in length, or the equivalent of 64 Pawn cells
	Salt[17],
	Float: Pos_X,
	Float: Pos_Y,
	Float: Pos_Z,
	Float: Pos_Angle,
	Pos_Interior,

	bool: IsLoggedIn,
	LoginAttempts,
	LoginTimer
};
new Player[MAX_PLAYERS][E_PLAYERS];

new g_MysqlRaceCheck[MAX_PLAYERS];

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

	return 1;
}

hook OnPlayerConnect(playerid)
{
	g_MysqlRaceCheck[playerid]++;

    // reset player data
	static const empty_player[E_PLAYERS];
	Player[playerid] = empty_player;

    GetPlayerName(playerid, Player[playerid][Name], MAX_PLAYER_NAME);

	// create orm instance and register all needed variables
	new ORM: ormid = Player[playerid][ORM_ID] = orm_create("players", MySQL_GetHandle());

	orm_addvar_int(ormid, Player[playerid][ID], "id");
	orm_addvar_string(ormid, Player[playerid][Name], MAX_PLAYER_NAME, "username");
	orm_addvar_string(ormid, Player[playerid][Password], 65, "password");
	orm_addvar_string(ormid, Player[playerid][Salt], 17, "salt");
	orm_addvar_float(ormid, Player[playerid][Pos_X], "pos_x");
	orm_addvar_float(ormid, Player[playerid][Pos_Y], "pos_y");
	orm_addvar_float(ormid, Player[playerid][Pos_Z], "pos_z");
	orm_addvar_float(ormid, Player[playerid][Pos_Angle], "pos_angle");
	orm_addvar_int(ormid, Player[playerid][Pos_Interior], "pos_interior");
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
	if (Player[playerid][LoginTimer])
	{
		KillTimer(Player[playerid][LoginTimer]);
		Player[playerid][LoginTimer] = 0;
	}

	// sets "IsLoggedIn" to false when the player disconnects, it prevents from saving the player data twice when "gmx" is used
	Player[playerid][IsLoggedIn] = false;

    return 1;
}

hook OnPlayerSpawn(playerid)
{
    // spawn the player to their last saved position
	SetPlayerInterior(playerid, Player[playerid][Pos_Interior]);
	SetPlayerPos(playerid, Player[playerid][Pos_X], Player[playerid][Pos_Y], Player[playerid][Pos_Z]);
	SetPlayerFacingAngle(playerid, Player[playerid][Pos_Angle]);

	SetCameraBehindPlayer(playerid);

	return 1;
}

UpdatePlayerData(playerid, reason)
{
	if (Player[playerid][IsLoggedIn] == false) return 0;

	// if the client crashed, it's not possible to get the player's position in OnPlayerDisconnect callback
	// so we will use the last saved position (in case of a player who registered and crashed/kicked, the position will be the default spawn point)
	if (reason == 1)
	{
		GetPlayerPos(playerid, Player[playerid][Pos_X], Player[playerid][Pos_Y], Player[playerid][Pos_Z]);
		GetPlayerFacingAngle(playerid, Player[playerid][Pos_Angle]);
	}

	// it is important to store everything in the variables registered in ORM instance
	Player[playerid][Pos_Interior] = GetPlayerInterior(playerid);

	// orm_save sends an UPDATE query
	orm_save(Player[playerid][ORM_ID]);
	orm_destroy(Player[playerid][ORM_ID]);
	return 1;
}

forward OnPlayerRegister(playerid);
public OnPlayerRegister(playerid)
{
	SendClientMessage(playerid, COLOR_ORANGE, "Tu cuenta ha sido registrada exitosamente, bienvenido!");

	Player[playerid][IsLoggedIn] = true;

	Player[playerid][Pos_X] = DEFAULT_POS_X;
	Player[playerid][Pos_Y] = DEFAULT_POS_Y;
	Player[playerid][Pos_Z] = DEFAULT_POS_Z;
	Player[playerid][Pos_Angle] = DEFAULT_POS_A;

	SetSpawnInfo(playerid, NO_TEAM, 0, Player[playerid][Pos_X], Player[playerid][Pos_Y], Player[playerid][Pos_Z], Player[playerid][Pos_Angle], 0, 0, 0, 0, 0, 0);
	SpawnPlayer(playerid);
	
	return 1;
}

forward OnLoginTimeout(playerid);
public OnLoginTimeout(playerid)
{
    // reset the variable that stores the timerid
	Player[playerid][LoginTimer] = 0;

	Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Ingresa en tu cuenta", "Has sido expulsado por tardarte mucho en ingresar una contraseña, vuelve a entrar para intentar de nuevo.", "Ok", "");
	DelayedKick(playerid);
	return 1;
}

forward _KickPlayerDelayed(playerid);
public _KickPlayerDelayed(playerid)
{
	Kick(playerid);
	return 1;
}

static DelayedKick(playerid, time = 500)
{
	SetTimerEx("_KickPlayerDelayed", time, false, "d", playerid);
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

	orm_setkey(Player[playerid][ORM_ID], "id");

	new string[115];
	switch (orm_errno(Player[playerid][ORM_ID]))
	{
		case ERROR_OK:
		{
            inline DialogLogin(id, dialogid, response, listitem, string: inputtext[]) {
                #pragma unused id, dialogid, listitem
                
                if (!response) return Kick(playerid);
                
                new hashed_pass[65];
                SHA256_PassHash(inputtext, Player[playerid][Salt], hashed_pass, 65);

                // If the password provided is invalid
                if (strcmp(hashed_pass, Player[playerid][Password]) == 0)
                {
                    //correct password, spawn the player
                    return AuthenticatePlayer(playerid);
                } else {
                    Player[playerid][LoginAttempts]++;

                    if (Player[playerid][LoginAttempts] >= 3)
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

			format(string, sizeof string, "Esta cuenta (%s) ya esta registrada. Por favor ingresa tu contraseña:", Player[playerid][Name]);
            Dialog_ShowCallback(playerid, using inline DialogLogin, DIALOG_STYLE_PASSWORD, "Ingresa en tu cuenta", string, "Ingresar", "Cancelar");

            // from now on, the player has a limited time to login
			Player[playerid][LoginTimer] = SetTimerEx("OnLoginTimeout", SECONDS_TO_LOGIN * 1000, false, "d", playerid);
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
                for (new i = 0; i < 16; i++) Player[playerid][Salt][i] = random(94) + 33;
                SHA256_PassHash(inputtext, Player[playerid][Salt], Player[playerid][Password], 65);
                
                // Get the DateTime to keep a record of when the account was created
                orm_addvar_string(Player[playerid][ORM_ID], getDateTime(), 32, "created_at");
                // sends an INSERT query
                orm_save(Player[playerid][ORM_ID], "OnPlayerRegister", "d", playerid);
            }

			format(string, sizeof string, "Bienvenido %s, para registrar esta cuenta, por favor ingresa una contraseña:", Player[playerid][Name]);
            Dialog_ShowCallback(playerid, using inline DialogRegister, DIALOG_STYLE_PASSWORD, "Registro", string, "Registrar", "Cancelar");
		}
	}

	return 1;
}

static AuthenticatePlayer(playerid)
{
    SendClientMessage(playerid, COLOR_ORANGE, "Bienvenido de vuelta!");

    KillTimer(Player[playerid][LoginTimer]);
    Player[playerid][LoginTimer] = 0;
    Player[playerid][IsLoggedIn] = true;

    // spawn the player to their last saved position after login
    SetSpawnInfo(playerid, NO_TEAM, 0, Player[playerid][Pos_X], Player[playerid][Pos_Y], Player[playerid][Pos_Z], Player[playerid][Pos_Angle], 0, 0, 0, 0, 0, 0);
    SpawnPlayer(playerid);

    return 1;
}