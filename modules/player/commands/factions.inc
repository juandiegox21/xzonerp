// Faction commands
CMD:f(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    new PlayerFactionId = GetPlayerFactionId(playerid);
    
    if (PlayerFactionId < 1)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");

    if(sscanf(params, "s[128]", params)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /(f)accion [Texto]");

    
    return SendMessageToFactionMembers(playerid, PlayerFactionId, params);
}

CMD:finvitar(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    new PlayerFactionId = GetPlayerFactionId(playerid);
    if(PlayerFactionId < 1)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");
    
    if(sscanf(params, "i", params[0])) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /finvitar [ID]");

    
    AddFactionMember(GetPlayerNameRP(playerid), PlayerFactionId, 8); 
}

CMD:fmiembros(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    if (GetPlayerFactionId(playerid) < 1)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");

    new 
        PlayerFactionId = GetPlayerFactionId(playerid),
        FactionMembersMessage[540];
        
    FactionMembersMessage = GetFactionMembers(PlayerFactionId);
    SendClientMessage(playerid, COLOR_WHITE, FactionMembersMessage);
    return 1;
}