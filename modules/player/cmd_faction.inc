#include <YSI\y_timers>

/* ------------- MEMBERS FACTION COMMANDS ------------------ */

CMD:f(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    new PlayerFactionId = GetPlayerFactionId(playerid);
    
    if (PlayerFactionId < 1)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");

    if(sscanf(params, "s[128]", params)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /(f)accion [Texto]");

    new 
        string[320],
        PlayerFactionRankElevation = GetPlayerFactionRankElevation(playerid);

    format(string, sizeof string, "[%s] %s - %s: %s", GetFactionName(PlayerFactionId), GetFactionRankName(PlayerFactionId, PlayerFactionRankElevation), GetPlayerNameRP(playerid), params);

    return SendMessageToFactionMembers(playerid, PlayerFactionId, string);
}

CMD:fmiembros(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    new 
        PlayerFactionId = GetPlayerFactionId(playerid);

    if (PlayerFactionId < 1)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");

    return SendClientMessage(playerid, COLOR_WHITE, GetFactionMembers(PlayerFactionId));
}

CMD:frenunciar(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    else if (GetPlayerFactionId(playerid) < 1)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");
    
    else if (PlayerIsFactionLeader(playerid))
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] No puedes renunciar a esta faccion, ya que eres su lider.");

    new PlayerFactionId = GetPlayerFactionId(playerid);

    inline DialogQuitFaction(id, dialogid, response, listitem, string: inputtext[]) {
        #pragma unused id, dialogid, listitem, inputtext
        if (!response)
            return 1;

        new 
            string[220],
            PlayerFactionRankElevation = GetPlayerFactionRankElevation(playerid);

        SetPlayerFactionId(playerid, 0);
        SendClientMessage(playerid, COLOR_DODGER_BLUE, "[FACCIONES] Has renunciado a tu faccion");

        format(string, sizeof string, "[%s] (%s) %s ha renunciado", GetFactionName(PlayerFactionId), GetFactionRankName(PlayerFactionId, PlayerFactionRankElevation), GetPlayerNameRP(playerid));
        SendMessageToFactionMembers(playerid, PlayerFactionId, string);
    }

    Dialog_ShowCallback(playerid, using inline DialogQuitFaction, DIALOG_STYLE_MSGBOX, GetFactionName(PlayerFactionId), "Estas seguro que quieres renunciar a tu faccion?", "Renunciar", "Cancelar");
    return 1;
}

/* ------------- LEADER FACTION COMMANDS ------------------ */

CMD:finvitar(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    new 
        PlayerFactionId = GetPlayerFactionId(playerid);

    if(PlayerFactionId < 1)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");

    else if (!PlayerIsFactionLeader(playerid))
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No eres el lider de esta faccion.");
    
    if(sscanf(params, "i", params[0])) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /finvitar [ID]");

    new playerid2 = params[0];

    if (playerid2 == playerid)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] No puedes invitarte a ti mismo."); 

    else if(!IsPlayerConnected(playerid2)) 
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] Este jugador no esta en linea.");

    else if(!GetPlayersProx(5.0, playerid, playerid2))
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] La persona a la que quieres invitar esta muy lejos de ti.");
    
    else if (GetPlayerFactionId(playerid2) != FACTION_CIVIL)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Este jugador ya pertenece a una faccion.");

    else if (GetPVarInt(playerid2, "FactionRecruitingId") > 0)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Este jugador ya ha sido invitado a una faccion.");
    
    new string [320];

    format(string, sizeof string, "[FACCIONES] Has invitado a %s a tu faccion", GetPlayerNameRP(playerid2));
    SendClientMessage(playerid, COLOR_DODGER_BLUE, string);

    format(string, sizeof string, "[FACCIONES] %s desea reclutarte para %s", GetPlayerNameRP(playerid), GetFactionName(PlayerFactionId));
    SendClientMessage(playerid2, COLOR_DODGER_BLUE, string);

    SendClientMessage(playerid2, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" para aceptar escribe "COLOR_ORANGE_INLINE" /aceptar faccion "COLOR_WHITE_INLINE" o para rechazar "COLOR_ORANGE_INLINE" /rechazar faccion");

    SetPVarInt(playerid2, "FactionRecruitingId", PlayerFactionId);
    SetPVarInt(playerid2, "FactionRecruitingLeaderId", playerid);
    defer ExpireFactionInvitation(playerid2);
    return 1;
}

CMD:fpromover(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    new 
        PlayerFactionId = GetPlayerFactionId(playerid);

    if(PlayerFactionId < 1)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");

    else if (!PlayerIsFactionLeader(playerid))
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No eres el lider de esta faccion.");
    
    if(sscanf(params, "i", params[0]))
        return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /fpromover [ID]");

    new playerid2 = params[0];

    if (playerid2 == playerid)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] No puedes promoverte a ti mismo."); 

    else if(!IsPlayerConnected(playerid2)) 
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] Este jugador no esta en linea.");

    else if (GetPlayerFactionId(playerid2) != PlayerFactionId)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Este jugador no pertenece a esta faccion.");

    else if(!GetPlayersProx(5.0, playerid, playerid2))
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] La persona a la que quieres promover esta muy lejos de ti.");

    inline DialogPromoteFactionMember(id, dialogid, response, listitem, string: inputtext[]) {
        #pragma unused id, dialogid, inputtext
        if (!response)
            return 1;

        new
            string[220],
            RankElevation = listitem;

        SetPlayerFactionRankElevation(playerid2, RankElevation);

        format(string, sizeof string, "[FACCIONES] %s te ha promovido a rango %s", GetPlayerNameRP(playerid), GetFactionRankName(PlayerFactionId, RankElevation));
        SendClientMessage(playerid2, COLOR_DODGER_BLUE, string);

        format(string, sizeof string, "[FACCIONES] has promovido a %s a rango %s", GetPlayerNameRP(playerid2), GetFactionRankName(PlayerFactionId, RankElevation));
        SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
    }

    new DialogTitleString[120];
    format(DialogTitleString, sizeof DialogTitleString, "A que rango quieres promover a %s?", GetPlayerNameRP(playerid2));
    Dialog_ShowCallback(playerid, using inline DialogPromoteFactionMember, DIALOG_STYLE_LIST, DialogTitleString, GetFactionRanksName(PlayerFactionId), "Promover", "Cancelar");
    return 1;
}

CMD:fexpulsar(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    else if (GetPlayerFactionId(playerid) < 1)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");
    
    else if (GetPlayerFactionRankElevation(playerid) < 3)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No tienes permisos para expulsar a un jugador.");

    if(sscanf(params, "i", params[0]))
        return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /fexpulsar [ID]");

    new 
        playerid2 = params[0],
        PlayerFactionId = GetPlayerFactionId(playerid);

    if (playerid2 == playerid)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] No te puedes expulsar a ti mismo."); 

    else if(!IsPlayerConnected(playerid2)) 
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] Este jugador no esta en linea.");

    else if (GetPlayerFactionId(playerid2) != PlayerFactionId)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Este jugador no pertenece a esta faccion.");

    else if (GetPlayerFactionRankElevation(playerid2) > GetPlayerFactionRankElevation(playerid))
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No puedes expulsar a alguien con mas rango");

    inline DialogKickMemberFromFaction(id, dialogid, response, listitem, string: inputtext[]) {
        #pragma unused id, dialogid, listitem, inputtext
        if (!response)
            return 1;

        new 
            string[220],
            PlayerFactionRankElevation = GetPlayerFactionRankElevation(playerid2);

        SetPlayerFactionId(playerid2, 0);
        SendClientMessage(playerid2, COLOR_DODGER_BLUE, "[FACCIONES] Has sido expulsado de tu faccion");

        format(string, sizeof string, "[%s] (%s) %s ha sido expulsado", GetFactionName(PlayerFactionId), GetFactionRankName(PlayerFactionId, PlayerFactionRankElevation), GetPlayerNameRP(playerid2));
        SendMessageToFactionMembers(playerid, PlayerFactionId, string);
    }

    new KickMemberConfirmationString[120];
    
    format(KickMemberConfirmationString, sizeof KickMemberConfirmationString, "Estas seguro que quieres expulsar a %s de tu faccion?", GetPlayerNameRP(playerid2));
    Dialog_ShowCallback(playerid, using inline DialogKickMemberFromFaction, DIALOG_STYLE_MSGBOX, GetFactionName(PlayerFactionId), KickMemberConfirmationString, "Expulsar", "Cancelar");
    return 1;
}

/* ------------- TIMERS ------------------ */

// 2 Minutes
timer ExpireFactionInvitation[120000](playerid)
{
    DeletePVar(playerid, "FactionRecruitingId");
    DeletePVar(playerid, "FactionRecruitingLeaderId");
}