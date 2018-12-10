#ifndef __BS_SRV__
#define __BS_SRV__

void request_respond(int efd, int cfd);
void sendheader(int fd, char *http_version, char *status, int mess_len,
     char *mess_type);
int getdata(int fd, char *buf, int len);
//void senddata(int cfd, char *buf, int len);
void senddata(int cfd, char *fileName);


#endif
