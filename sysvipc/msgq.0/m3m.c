/*
 * m3m.c
 *
 * message-queue, the monitoring
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define	BUFSIZE	256
#define	KEY	(0xFADE)

int	msgid;
char	buf[BUFSIZE];

int	main(void) {
	time_t	t;

	/* Create message queues */
	msgid=msgget(
			(key_t)KEY,
			0600
		    );
	if (msgid == -1) { perror("msgget"); exit(-1); }
	memset(buf,0,sizeof(buf));

	t=time(NULL);

	while (1) {
		struct msqid_ds q,oldq;

		if (msgctl(msgid, IPC_STAT, &q)==-1) {
			perror("msgctl:stat"); exit(-1);
		}
		if (memcmp(&q,&oldq,sizeof(q))!=0) {
		fprintf(stderr,	"stime=%d,rtime=%d,ctime=%d,"
				"qnum=%d,qbytes=%d\n",
				(int)(q.msg_stime-t),
				(int)(q.msg_rtime-t),
				(int)(q.msg_ctime-t),
				(int)q.msg_qnum,
				(int)q.msg_qbytes);
		}
		oldq=q;
		sleep(1);
	}

	return 0;
}

/*
 * end of file.
 */
