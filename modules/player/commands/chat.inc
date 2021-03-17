// Chat commands
CMD:b(playerid, params[])
{
	if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    if(sscanf(params, "s[128]", params)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE"/b [Texto]");

    new string[120];
    format(string, sizeof(string), "[ID: %d] %s: (( %s ))", playerid, GetPlayerNameRP(playerid), params);
    ProxDetector(15.0, playerid, string, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4, COLOR_FADE5);
    printf("Chat OOC %s: %s", GetPlayerNameRP(playerid), params);

	return 1;
}

CMD:me(playerid, params[])
{
	if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
	if(sscanf(params, "s[128]", params)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /me [Accion]");

	new string[120];
	format(string, sizeof(string), "%s %s", GetPlayerNameRP(playerid), params);
	ProxDetector(15.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
	printf("%s", string);
	return 1;
}

CMD:do(playerid, params[])
{
	if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
	if(sscanf(params, "s[128]", params)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /do [Texto]");
	
    new string[120];
	format(string, sizeof(string), "* (%s) %s", GetPlayerNameRP(playerid), params);
	ProxDetector(15.0, playerid, string, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN, COLOR_GREEN);
	printf("(/do) %s: %s", GetPlayerNameRP(playerid), params);
	return 1;
}

CMD:g(playerid, params[])
{
	if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
	if(sscanf(params, "s[128]", params)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /(g)ritar [Texto]");
    
    new string[120];
    format(string, sizeof(string), "%s grita: %s!!", GetPlayerNameRP(playerid), params);
    ProxDetector(35.0, playerid, string, COLOR_WHITE, COLOR_FADE1, COLOR_FADE2, COLOR_FADE3, COLOR_FADE4);
    printf("%s grita: %s", GetPlayerNameRP(playerid), params);
	return 1;
}

CMD:s(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
	if(sscanf(params, "is[120]", params[0], params[1])) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /(S)usurrar [ID][Texto]");

    new playerid2 = params[0], string[120];
    if(playerid2 == playerid) {
        SendClientMessage(playerid, COLOR_RED, "[ATENCION] No te puedes susurrar a ti mismo.");
        return 1;
    }
	
    if(!IsPlayerConnected(playerid2) || playerid2 == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: ID invalida.");
    if(!GetPlayersProx(1.5, playerid, playerid2)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION]: "COLOR_WHITE_INLINE" Debes estar al lado del jugador que deseas susurrar.");

    format(string, sizeof(string), "* %s susurra algo al oido de %s", GetPlayerNameRP(playerid), GetPlayerNameRP(playerid2));
    ProxDetector(15.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

    format(string, sizeof(string), "%s te susurra al oido: %s", GetPlayerNameRP(playerid), params[1]);
    SendClientMessage(playerid2, COLOR_YELLOW_LIGHT, string);

    format(string, sizeof(string), "Susurras al oido de %s: %s", GetPlayerNameRP(playerid2), params[1]);
    SendClientMessage(playerid, COLOR_YELLOW, string);
    printf("%s susurra a %s: %s", GetPlayerNameRP(playerid), GetPlayerNameRP(playerid2), params[1]);
	return 1;
}