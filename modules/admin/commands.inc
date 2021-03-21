CMD:hacerlider(playerid, params[])
{
	if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    if(sscanf(params, "d", params[0])) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE"/hacerlider [ID]");
    
    new
        playerid2 = params[0];

    inline DialogAvailableFactions(id, dialogid, response, listitem, string: inputtext[]) {
        #pragma unused id, dialogid, inputtext

        if (!response)
            return 1;

        new 
            string[120],
            FactionId = listitem,
            Elevation = 4; // MAX elevation
            
        SetPlayerFactionId(playerid2, FactionId);

        format(string, sizeof string, "[ATENCION] Ahora eres miembro de la faccion: %s", GetFactionName(FactionId));
        SendClientMessage(playerid2, COLOR_DODGER_BLUE, string);

        // Only promote rank when the faction is not CIVIL
        if (FactionId > 0) {
            SetPlayerFactionRankElevation(playerid2, Elevation);

            format(string, sizeof string, "[ATENCION] Has sido promovido a rango %s", GetFactionRankName(FactionId, Elevation));
            SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
        }
    }

    new DialogTitleString[120];
    format(DialogTitleString, sizeof DialogTitleString, "De que faccion %s sera lider?", GetPlayerNameRP(playerid2));
    Dialog_ShowCallback(playerid, using inline DialogAvailableFactions, DIALOG_STYLE_LIST, DialogTitleString, GetFactions(), "Asignar", "Cancelar");

    return 1;
}