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

        if (FactionRecruitingId == 0)
            return SendClientMessage(playerid, COLOR_RED, "[ATENCION] Nadie te ha ofrecido formar parte de su faccion");
        
        else if (GetPlayerFactionId(playerid) != FACTION_CIVIL)
            return SendClientMessage(playerid, COLOR_RED, "[ATENCION]: Ya perteneces a una faccion, renuncia a tu faccion antes de aceptar una nueva invitacion.");

		else if(!GetPlayersProx(5.0, playerid, GetPVarInt(playerid, "FactionRecruitingLeaderId"))) {
            return SendClientMessage(playerid, COLOR_RED, "[ATENCION] La persona que te invito a su faccion esta muy lejos de ti.");
        }

        SetPlayerFactionId(playerid, FactionRecruitingId);
        SetPlayerFactionRankElevation(playerid, 0);

        new string[120];
        format(string, sizeof string, "[ATENCION] Ahora eres miembro de la faccion: %s", GetFactionName(FactionRecruitingId));
        SendClientMessage(playerid, COLOR_DODGER_BLUE, string);

        format(string, sizeof string, "[%s] %s se ha unido", GetFactionName(FactionRecruitingId), GetPlayerNameRP(playerid));
        SendMessageToFactionMembers(playerid, FactionRecruitingId, string);

        DeletePVar(playerid, "FactionRecruitingId");
        DeletePVar(playerid, "FactionRecruitingLeaderId");
    }
    else {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /aceptar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" faccion");
    }

    return 1;
}

CMD:rechazar(playerid, params[])
{
    if(!IsPlayerConnected(playerid)) return SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" Debes estar logueado para usar este comando");
    
    if(sscanf(params, "s[32]", params)) {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /rechazar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" faccion");
		return 1;
	}

    if(!strcmp(params, "faccion", true, 7)) {
        new 
            FactionRecruitingId = GetPVarInt(playerid, "FactionRecruitingId"),
            FactionRecruitingLeaderId = GetPVarInt(playerid, "FactionRecruitingLeaderId");

        if (FactionRecruitingId == 0) return SendClientMessage(playerid, COLOR_RED, "[ATENCION] Nadie te ha ofrecido formar parte de su faccion");
		if(!GetPlayersProx(5.0, playerid, FactionRecruitingLeaderId)) {
            return SendClientMessage(playerid, COLOR_RED, "[ATENCION] La persona que te invito a su faccion esta muy lejos de ti.");
        }

        new string[120];
        format(string, sizeof string, "[FACCIONES] %s ha rechazado unirse a tu faccion", GetPlayerNameRP(playerid));
        SendClientMessage(FactionRecruitingLeaderId, COLOR_DODGER_BLUE, string);

        format(string, sizeof string, "[ATENCION] Has rechazado unirte a %s", GetFactionName(FactionRecruitingId));
        SendClientMessage(playerid, COLOR_DODGER_BLUE, string);

        DeletePVar(playerid, "FactionRecruitingId");
        DeletePVar(playerid, "FactionRecruitingLeaderId");
    }
    else {
        SendClientMessage(playerid, COLOR_ORANGE, "[ATENCION] "COLOR_WHITE_INLINE" /rechazar [opcion]");
        SendClientMessage(playerid, COLOR_ORANGE, "[OPCIONES] > "COLOR_WHITE_INLINE" faccion");
    }

    return 1;
}