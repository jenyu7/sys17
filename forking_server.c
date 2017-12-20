#include "pipe_networking.h"
#include <signal.h>
#include <ctype.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
  int from_client;
  int parent = getpid();
  while ( 1 ) {
    from_client = server_setup();
    if (getpid() != parent) {
      subserver(from_client);
    }
  }
  return 0;
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);
  char buf[BUFFER_SIZE];
  memset(buf, 0, BUFFER_SIZE);
  while ( read(from_client, buf, sizeof(buf)) ) {
    printf("Received from client: %s\n", buf);
    process(buf);
    write(to_client, buf, sizeof(buf));
  }
  exit(0);
}

void process(char * s) {
  int i;
  for ( i=0 ; s[i] ; i++ ) {
    s[i] = toupper(s[i]);
  }
}
