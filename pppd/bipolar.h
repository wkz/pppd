#ifndef __BIPOLAR_H
#define __BIPOLAR_H

void bipolar_set_connector(char** connector);
void bipolar_set_disconnector(char** disconnector);

int  bipolar_incoming(void);

void bipolar_setup(void);
void bipolar_teardown(void);

#endif /* __BIPOLAR_H */
