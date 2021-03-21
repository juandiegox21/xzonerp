#include <YSI\y_timers>

/* -------------- GENERAL COMMANDS -------------- */
CMD:cuenta(playerid, params[])
{
	if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");

    new 
        string[240],
        FactionId = GetPlayerFactionId(playerid),
        PlayerFactionRankElevation = GetPlayerFactionRankElevation(playerid);

    format(string, sizeof string, "Faccion Nombre: %s | Faccion Rango: %s", GetPlayerFactionName(playerid), GetFactionRankName(FactionId, PlayerFactionRankElevation));
    SendClientMessage(playerid, COLOR_ORANGE, string);
    return 1;
}

CMD:ayuda(playerid, params[])
{
    inline DialogHelp(id, dialogid, response, listitem, string: inputtext[]) {
        #pragma unused id, dialogid, inputtext

        if (!response)
            return 1;
            
        ListHelpCommands(playerid, listitem);
    }

    Dialog_ShowCallback(playerid, using inline DialogHelp, DIALOG_STYLE_LIST, "Menu de comandos", \
        "General\n"     \
        "Chat\n"        \
        "Telefonos\n"   \
        "Direcciones\n" \
        "Trabajos\n"    \
        "Vehiculos\n"   \
        "Casas\n"       \
        "Negocios\n"    \
        "Facciones"
    , 
    "Seleccionar", "Cancelar");

    return 1;
}

static ListHelpCommands(playerid, itemId)
{
    switch(itemId) {
        case 0:
        {
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]: General >");
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /cuenta - /comprar");
            return 1;
        }

        case 1:
        {
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]: Chat >");
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /(g)ritar - /(s)usurrar - /(f)accion - /(r)adio");
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /idioma - /b - /me - /do");
            return 1;
        }

        case 2:
        {
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]: Telefonos >");
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /llamar, /sms, /contestar, /colgar, /agenda");
            return 1;
        }

        case 3:
        {
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]: Direcciones >");
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /guia (Para encontrar el puesto de guia mas cercano)");
            return 1;
        }

        case 4:
        {
            // Player doesn't have a job
            if (GetPlayerJobId(playerid) < 1) {
                return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No tienes un trabajo.");
            }

            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]: Trabajos >");
            GetJobCommands(playerid, GetPlayerJobName(playerid));
            return 1;
        }

        case 5:
        {
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]: Vehiculos >");
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /motor (o presiona click) - /luces (o presiona CTRL) - /gas - /cinturon");
            return 1;
        }

        case 6:
        {
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]: Casas >");
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /comprar casa");
            return 1;
        }

        case 7:
        {
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]: Negocios >");
            SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /comprar negocio");
            return 1;
        }

        case 8:
        {
            // Player is not part of a faction
            if (GetPlayerFactionId(playerid) < 1)
                return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: No perteneces a una faccion.");
                
            GetFactionCommands(playerid, GetPlayerFactionName(playerid));
            return 1;
        }

        default:
        {
            SendClientMessage(playerid, COLOR_RED, "[CMDS]: No hay comandos a listar para esta categoria de ayuda.");
            return 1;
        }
    }

    return 1;
}

static GetJobCommands(playerid, jobName)
{
    new string[120];
    format(string, sizeof string, "[CMDS]: %s >", jobName);
    SendClientMessage(playerid, COLOR_ORANGE, string);

    SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /trabajo renunciar");
    return 1;
}

static GetFactionCommands(playerid, const FactionName[])
{
    new string[120];
    format(string, sizeof string, "[CMDS]: %s >", FactionName);
    SendClientMessage(playerid, COLOR_ORANGE, string);

    SendClientMessage(playerid, COLOR_ORANGE, "[CMDS]:"COLOR_WHITE_INLINE" /frenunciar");
    return 1;
}

CMD:aceptar(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    if(sscanf(params, "s[32]", params)) {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /aceptar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" faccion");
		return 1;
	}

    if(!strcmp(params, "faccion", true, 7)) {
        new 
            FactionRecruitingId = GetPVarInt(playerid, "FactionRecruitingId");

        if (FactionRecruitingId == 0) return SendClientMessage(playerid, COLOR_RED, "[ATENCION] Nadie te ha ofrecido formar parte de su faccion");
		if(!GetPlayersProx(5.0, playerid, GetPVarInt(playerid, "FactionRecruitingLeaderId"))) {
            SendClientMessage(playerid, COLOR_RED, "[ATENCION] La persona que te invito a su faccion esta muy lejos de ti.");
            return 1;
        }

        SetPlayerFactionId(playerid, FactionRecruitingId);
        SetPlayerFactionRankElevation(playerid, 0);

        new string[120];
        format(string, sizeof string, "[ATENCION] Ahora eres miembro de la faccion: %s", GetFactionName(FactionRecruitingId));
        SendClientMessage(playerid, COLOR_DODGER_BLUE, string);

        DeletePVar(playerid, "FactionRecruitingId");
        DeletePVar(playerid, "FactionRecruitingLeaderId");
    }
    else {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /aceptar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" faccion");
    }

    return 1;
}

/* ---------- CHAT COMMANDS --------- */
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

/* ------------- NON-LEADER FACTION COMMANDS ------------------ */
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
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] "COLOR_WHITE_INLINE" No puedes invitarte a ti mismo."); 

    else if(!IsPlayerConnected(playerid2)) 
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION] "COLOR_WHITE_INLINE" Este jugador no esta en linea.");
    
    else if (GetPlayerFactionId(playerid2) != FACTION_CIVIL)
        return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Este jugador ya pertenece a una faccion.");
    
    new string [320];
    format(string, sizeof string, "[FACCIONES] %s desea reclutarte para %s", GetPlayerNameRP(playerid), GetFactionName(PlayerFactionId));
    SendClientMessage(playerid2, COLOR_DODGER_BLUE, string);

    SendClientMessage(playerid2, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" para aceptar escribe /aceptar faccion (tienes 2 minutos para aceptar)");

    SetPVarInt(playerid2, "FactionRecruitingId", PlayerFactionId);
    SetPVarInt(playerid2, "FactionRecruitingLeaderId", playerid);
    defer ExpireFactionInvitation(playerid2);
    return 1;
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

// 2 Minutes
timer ExpireFactionInvitation[120000](playerid)
{
    DeletePVar(playerid, "FactionRecruitingId");
    DeletePVar(playerid, "FactionRecruitingLeaderId");
}