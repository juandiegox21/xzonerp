#include <YSI_Coding\y_hooks>

#define MAX_JOBS    5

// Jobs
#define JOB_UNEMPLOYED  0
#define JOB_TRUCKER     1

enum e_JobData
{
    e_JobId,
	e_JobLabel[120],
	Float:e_JobPos[3], // X, Y, Z
};

static const g_arrJobData[][e_JobData] = 
{
    // Trucker
    {
        JOB_TRUCKER, "Trabajo de camionero "COLOR_ORANGE_INLINE" /trabajo", {2455.14282, -2113.93774, 13.65593}
    }
};

hook OnGameModeInit()
{
    Job_CreateMiscs();

    return 1;
}

// Miscellaneous stuff, such as map icon, pick up and 3D text label
forward Job_CreateMiscs();
public Job_CreateMiscs()
{
    for (new i = 0; i < sizeof(g_arrJobData); i++) {
        new 
            Float:x = g_arrJobData[i][e_JobPos][0],
            Float:y = g_arrJobData[i][e_JobPos][1],
            Float:z = g_arrJobData[i][e_JobPos][2];

        CreateDynamic3DTextLabel(g_arrJobData[i][e_JobLabel], -1, x, y, z, 9, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
	    CreateDynamicPickup(1210, 1, x, y, z);
        CreateDynamicMapIcon(x, y, z, 51, 0, -1, -1, -1, 300.0, MAPICON_GLOBAL, -1, 0);
    }

    return 1;
}

stock Job_GetList()
{
    new 
        jobsString[300],
        jobDescription[42];

    for (new jobId = 0; jobId < MAX_JOBS; jobId++) {
        new jobName[42];

        jobName = Job_GetName(jobId);

        if (strcmp(jobName, "Unassigned")) {
            format(jobDescription, sizeof jobDescription, "%s\n", jobName);
            strcat(jobsString, jobDescription);
        }
    }

    return jobsString;
}

stock Job_IsValid(const jobId)
{
    if (jobId < 0 || jobId > MAX_JOBS) {
        return false;
    }

    return true;
}

stock Job_GetName(const jobId)
{
    new jobName[42];

    switch (jobId) 
    {
        case JOB_UNEMPLOYED: jobName = "Desempleado";
        case JOB_TRUCKER: jobName = "Camionero";
        default: jobName = "Unassigned";
    }

    return jobName;
}

stock Job_IsUnemployed(jobId)
{
    return jobId == JOB_UNEMPLOYED;
}