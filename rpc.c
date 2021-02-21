#include "./discord-rpc/linux-dynamic/include/discord_rpc.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void initialize(const char* applicationId) {
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = NULL;
    handlers.disconnected = NULL;
    handlers.errored = NULL;
    handlers.joinGame = NULL;
    handlers.spectateGame = NULL;
    handlers.joinRequest = NULL;
    Discord_Initialize(applicationId, &handlers, 1, NULL);
}

void update_presence(const char* filename, const char* details)
{
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = filename;
    discordPresence.details = details;
    discordPresence.startTimestamp = time(0);
    discordPresence.endTimestamp = 0;
    discordPresence.largeImageKey = NULL;
    discordPresence.smallImageKey = NULL;
    discordPresence.partyId = NULL;
    discordPresence.partySize = 0;
    discordPresence.partyMax = 0;
    discordPresence.matchSecret = NULL;
    discordPresence.joinSecret = NULL;
    discordPresence.spectateSecret = NULL;
    discordPresence.instance = 0;
    Discord_UpdatePresence(&discordPresence);
}
