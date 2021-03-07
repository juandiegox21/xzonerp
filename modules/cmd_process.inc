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