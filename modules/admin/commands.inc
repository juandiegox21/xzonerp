#include <izcmd>

CMD:facciones(playerid, params[])
{
    new string[42];
    for (new i = 0; i < GetFactionsCount(); i++) {
        format(string, sizeof string, "id: %d (%d) | name: %s", i, Factions[i][fId], Factions[i][fName]);
        SendClientMessage(playerid, COLOR_WHITE, string);
    }

    return 1;
}

CMD:factionranks(playerid, params[])
{
    if(sscanf(params, "d", params[0])) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE"/factionranks [Faccion ID]");

    new 
        FactionString[520],
        FactionId = params[0];

    format(FactionString, sizeof FactionString, "%s", GetFactionRanks(FactionId));
    SendClientMessage(playerid, COLOR_WHITE, FactionString);
    
    return 1;
}

CMD:hacerlider(playerid, params[])
{
	if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    if(sscanf(params, "dd", params[0], params[1])) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE"/hacerlider [ID][Faccion ID]");
    
    new 
        string[84],
        playerid2 = params[0],
        PlayerName[MAX_PLAYER_NAME],
        FactionId = params[1],
        RankElevation = 8;

    GetPlayerName(playerid2, PlayerName, MAX_PLAYER_NAME);

    SetPlayerFactionId(playerid2, FactionId);

    format(string, sizeof string, "[ATENCION] Ahora eres miembro de la faccion: %s", GetFactionName(FactionId));
    SendClientMessage(playerid2, COLOR_DODGER_BLUE, string);

    AddFactionMember(PlayerName, FactionId, RankElevation);
    return 1;
}