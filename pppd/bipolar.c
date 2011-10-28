#include "pppd.h"

#define BIPOLAR_IDLE   0
#define BIPOLAR_OPEN   1
#define BIPOLAR_WATCH  2
#define BIPOLAR_ACCEPT 3
#define BIPOLAR_DONE   4
static int state;

void bipolar_set_connector(char** connector)
{
  if (bipolar) {
    if (state == BIPOLAR_OPEN)
      *connector = NULL;
    else if (state == BIPOLAR_DONE)
      *connector = accept_script;
  }
}

void bipolar_set_disconnector(char** disconnector)
{
  if (bipolar && 
      (state == BIPOLAR_WATCH || state == BIPOLAR_ACCEPT))
    *disconnector = NULL;
}

int bipolar_incoming(void)
{
  if (bipolar && isready_fd(devfd)) {
    state = BIPOLAR_ACCEPT;
    return 1;
  }
  else
    return 0;
}

void bipolar_setup(void)
{
  if (bipolar) {
    state = BIPOLAR_OPEN;
    devfd = the_channel->connect();
    if (devfd < 0) {
      state = BIPOLAR_IDLE;
      warn("bipolar: unable to monitor incoming connections (%d)", devfd);
    }
    else {
      state = BIPOLAR_WATCH;
      add_fd(devfd);
    }
  }
  else {
    state = BIPOLAR_IDLE;
  }
}

void bipolar_teardown(void)
{
  if (bipolar && state != BIPOLAR_IDLE) {
    remove_fd(devfd);
    the_channel->disconnect();
    devfd = -1;
    
    if (state == BIPOLAR_ACCEPT)
      state = BIPOLAR_DONE;
    else
      state = BIPOLAR_IDLE;
  }
}
