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

CMD:hacerlider(playerid, params[])
{
	if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comado");
    if(sscanf(params, "dd", params[0], params[1])) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE"/hacerlider [ID][Faccion ID]");
    
    new playerid2 = params[0];
    SetPlayerFactionId(playerid2, params[1]);
    return 1;
}