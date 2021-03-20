#include <YSI_Coding\y_hooks>

#define MAX_FACTIONS                5
#define MAX_FACTION_RANK_ELEVATION  5 // Factions can only have 5 ranks

// Factions
#define FACTION_CIVIL   0
#define FACTION_SAPD    1
#define FACTION_MEPD    2
#define FACTION_CNN     3

forward GetFactions();
stock GetFactions()
{
    new 
        FactionsString[300],
        FactionDescription[42];

    for (new FactionId = 0; FactionId < MAX_FACTIONS; FactionId++) {
        new FactionName[42];

        FactionName = GetFactionName(FactionId);

        if (strcmp(FactionName, "Unassigned")) {
            format(FactionDescription, sizeof FactionDescription, "(ID: %d) %s\n", FactionId, FactionName);
            strcat(FactionsString, FactionDescription);
        }
    }

    return FactionsString;
}

forward FactionIsValid(const FactionId);
stock FactionIsValid(const FactionId)
{
    if (FactionId < 0 || FactionId > MAX_FACTIONS) {
        return false;
    }

    return true;
}

forward GetFactionName(const FactionId);
stock GetFactionName(const FactionId)
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

forward GetFactionMembers(FactionId);
stock GetFactionMembers(FactionId)
{
    new 
        MemberDescription[48],
        MembersList[700];

    for (new i = 0; i < MAX_PLAYERS; i++) {
        if (IsPlayerConnected(i) && GetPlayerFactionId(i) == FactionId) {
            new MemberRankName[120];
            MemberRankName = GetFactionRankName(FactionId, GetPlayerFactionRankElevation(i));

            format(MemberDescription, sizeof MemberDescription, "%s - %s\n", GetPlayerNameRP(i), MemberRankName);
            strcat(MembersList, MemberDescription);
        }
    }

    return MembersList;
}

forward GetFactionRankName(FactionId, Elevation);
stock GetFactionRankName(FactionId, Elevation)
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

forward SendMessageToFactionMembers(SenderId, FactionId, const message[]);
public SendMessageToFactionMembers(SenderId, FactionId, const message[])
{
    new 
        string[320], 
        SenderElevation = GetPlayerFactionRankElevation(SenderId);

    for (new i = 0; i < MAX_PLAYERS; i++) {
        if(!IsPlayerConnected(i)) continue;
        else if (Player[i][pFactionId] != FactionId) continue;
        
        format(string, sizeof(string), "[%s] %s - %s: %s", GetFactionName(FactionId), GetFactionRankName(FactionId, SenderElevation), GetPlayerNameRP(SenderId), message);
        SendClientMessage(i, COLOR_DODGER_BLUE, string);
    }

    return 1;
}