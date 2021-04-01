#include <YSI_Coding\y_hooks>

#define MAX_FACTIONS                5
#define MAX_FACTION_RANK_ELEVATION  5 // Factions can only have 5 ranks

// Factions
#define FACTION_CIVIL   0
#define FACTION_SAPD    1
#define FACTION_MEPD    2
#define FACTION_CNN     3

stock Faction_GetList()
{
    new 
        FactionsString[300],
        FactionDescription[42];

    for (new FactionId = 0; FactionId < MAX_FACTIONS; FactionId++) {
        new FactionName[42];

        FactionName = Faction_GetName(FactionId);

        if (strcmp(FactionName, "Unassigned")) {
            format(FactionDescription, sizeof FactionDescription, "(ID: %d) %s\n", FactionId, FactionName);
            strcat(FactionsString, FactionDescription);
        }
    }

    return FactionsString;
}

stock Faction_IsValid(const FactionId)
{
    if (FactionId < 0 || FactionId > MAX_FACTIONS) {
        return false;
    }

    return true;
}

stock Faction_GetName(const FactionId)
{
    new FactionName[42];

    switch (FactionId) 
    {
        case FACTION_CIVIL: FactionName = "Civil";
        case FACTION_SAPD: FactionName = "Los Santos PD";
        case FACTION_MEPD: FactionName = "Medicos";
        case FACTION_CNN: FactionName = "CNN";
        default: FactionName = "Unassigned";
    }

    return FactionName;
}

stock Faction_GetMembers(FactionId)
{
    new 
        MemberDescription[48],
        MembersList[700];

    for (new i = 0; i < MAX_PLAYERS; i++) {
        if (IsPlayerConnected(i) && Account_GetFactionId(i) == FactionId) {
            new MemberRankName[120];
            MemberRankName = Faction_GetRankName(FactionId, Account_GetFactionRankElevation(i));

            format(MemberDescription, sizeof MemberDescription, "%s - %s\n", GetPlayerNameRP(i), MemberRankName);
            strcat(MembersList, MemberDescription);
        }
    }

    return MembersList;
}

stock Faction_GetRanksName(FactionId)
{
    new 
        FactionRanksNameString[300],
        FactionRankName[42];

    for (new RankElevation = 0; RankElevation < MAX_FACTION_RANK_ELEVATION; RankElevation++) {
        format(FactionRankName, sizeof FactionRankName, "(Nivel de privilegio: %d) %s\n", RankElevation, Faction_GetRankName(FactionId, RankElevation));
        strcat(FactionRanksNameString, FactionRankName);
    }

    return FactionRanksNameString;
}

stock Faction_GetRankName(FactionId, Elevation)
{
    new RankName[120];

    switch (FactionId)
    {
        case FACTION_SAPD: {
            switch (Elevation)
            {
                case 0: RankName = "Cadete";
                case 1: RankName = "..";
                case 2: RankName = "..";
                case 3: RankName = "..";
                case 4: RankName = "General";
            }
        }

        case FACTION_MEPD: {
            switch(Elevation)
            {
                case 0: RankName = "Medico";
                case 1: RankName = "..";
                case 2: RankName = "..";
                case 3: RankName = "..";
                case 4: RankName = "Doctor";
            }
        }

        case FACTION_CNN: {
            switch(Elevation)
            {
                case 0: RankName = "Reportero";
                case 1: RankName = "..";
                case 2: RankName = "..";
                case 3: RankName = "..";
                case 4: RankName = "Lider";
            }
        }

        default: RankName = "Civil";
    }

    return RankName;
}

stock Faction_IsCivil(FactionId)
{
    return FactionId == FACTION_CIVIL;
}

forward Faction_SendMessageToMembers(SenderId, FactionId, const message[]);
public Faction_SendMessageToMembers(SenderId, FactionId, const message[])
{
    new 
        string[320];

    for (new i = 0; i < MAX_PLAYERS; i++) {
        if(!IsPlayerConnected(i)) continue;
        else if (Account_GetFactionId(i) != FactionId) continue;
        
        format(string, sizeof(string), "%s", message);
        SendClientMessage(i, COLOR_DODGER_BLUE, string);
    }

    printf("[FACCIONES] %s", message);

    return 1;
}