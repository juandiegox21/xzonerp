#include <cmds_general>
#include <cmds_chat>
#include <cmds_faction>
#include <cmds_job>
#include <cmds_vehicle>
#include <cmds_admin>

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
    new string[128];
	format(string, sizeof(string), "[CMD] [%s]: %s", GetPlayerNameRP(playerid), cmdtext);
	Logger_Log(string);

    if(!success) {
		return SendClientMessage(playerid, COLOR_RED, "Comando desconocido, usa /ayuda para ver los comandos.");
	}

	return 1;
}

public OnPlayerCommandReceived(playerid,cmdtext[])
{
	if(!IsPlayerLoggedIn(playerid)) {
		SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
		return 0;
	}

	return 1;
}