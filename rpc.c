#include "./discord-rpc/linux-dynamic/include/discord_rpc.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

static char* APPLICATION_ID = "813116720646455346";
static char* SOCKET_PATH = "/tmp/micro_rpc.socket";

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
    discordPresence.state = details;
    discordPresence.details = filename;
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

static void readline(char* buffer) {
	int i = 0;
	while(1) {
		char byte = fgetc(stdin);
		if (byte == '\n' || i > 1022) {
			buffer[i]  = '\0';
			break;
		}
		buffer[i++] = byte;
	}
}


static void usage() {
	printf("Usage: micro_rpc filename details OR micro_rpc -d\n");
}

static char* string_split(char* string, char del) {
	char *new = NULL;
	for (int i=0;i<=strlen(string);i++) {
		if (string[i] == del) {
			string[i] = '\0';
			new = string+i+1;
			break;
		}
	}
	return new;
}

static void start_socket();

static void send_presence(char* filename, char* details);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		usage();
		exit(1);
	}

	if (strcmp(argv[1], "-d") == 0) {
		initialize(APPLICATION_ID);
		update_presence("Doing nothing", NULL);
		start_socket();
	} else {
		if (argc < 3) {
			usage();
			exit(1);
		}

		send_presence(argv[1], argv[2]);
	}

	/*char *line = malloc(1024);
	while (1) {
		readline(line);

		char* state;
		char* details;
		
		for (int i=0;i<=strlen(line);i++) {
			if (line[i] == ' ') {
				line[i] = '\0';
				state = line;
				details = line+i+1;
				break;
			}
		}

		printf("s:%s d:%s\n\n", state, details);

		update_presence(state, details);
	}
	free(line);
	*/
}

static struct sockaddr_un get_addr() {
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);
	return addr;
}

static void send_presence(char* filename, char* details) {
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un addr = get_addr();

	if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    	perror("connect error");
    	exit(1);
  	}

	char combined_string[200];
	strcpy(combined_string, filename);
	strcat(combined_string, " ");
	strcat(combined_string, details);


	printf("%s\n", combined_string);

  	if (write(fd, combined_string, strlen(combined_string)) < strlen(combined_string)) {
  		perror("write error");
  		exit(1);
  	}
}

static void start_socket() {
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);

	struct sockaddr_un addr = get_addr();
	unlink(SOCKET_PATH);
	bind(fd, (struct sockaddr*)&addr, sizeof(addr));
	listen(fd, 5);

	char buf[1024];

	int cl,rc;
	while (1) {
	    if ((cl=accept(fd, NULL, NULL)) == -1) {
	     	continue;
	    }

	    while ((rc=read(cl, buf, sizeof(buf))) > 0) {
	    	buf[rc] = '\0';
	    	printf("read %u bytes: %.*s\n", rc, rc, buf);
	    }
	    
	    if (rc == -1) {
	     	perror("read");
	     	exit(-1);
	    }

	    //EOF
	    else if (rc == 0) {
	    	
	    	char* filename = buf;
			char* details = string_split(buf, ' ');

			printf("file: %s, details: %s\n", filename, details);
			
			update_presence(filename, details);
	      	close(cl);
	    }
	}
}
