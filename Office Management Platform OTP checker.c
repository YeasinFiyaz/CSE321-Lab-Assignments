#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg {
	long int type;
	char txt[6];
};

int main() {
	int msg_id;
	struct msg m_data;
	pid_t pid_otp, pid_mail;
	char workspace[20];
	long int otp;

	msg_id = msgget((key_t)301, 0666 | IPC_CREAT);
	if(msg_id == -1) {
		printf("Error creating message queue\n");
		exit(1);
	}

	printf("Please enter the workspace name:\n");
	read(0, workspace, sizeof(workspace));
	workspace[strcspn(workspace, "\n")] = 0;

	if(strcmp(workspace, "cse321") != 0) {
		printf("Invalid workspace name\n");
		msgctl(msg_id, IPC_RMID, 0);
		exit(0);
	}

	m_data.type = 1;
	strcpy(m_data.txt, workspace);
	if(msgsnd(msg_id, (void *)&m_data, sizeof(m_data.txt), 0) == -1) {
		printf("Error sending message\n");
	}
	printf("Workspace name sent to otp generator from log in: %s\n\n", m_data.txt);

	pid_otp = fork();
	if(pid_otp < 0) {
		perror("fork");
		exit(1);
	}
	else if(pid_otp == 0 {
		if(msgrcv(msg_id, (void *)&m_data, sizeof(m_data.txt), 1, 0) < 0) {
			perror("Error receiving message");
		}
		printf("OTP generator received workspace name from log in: %s\n\n", m_data.txt);

		otp = getpid();
		sprintf(m_data.txt, "%ld", otp);
		m_data.type = 2;
		if(msgsnd(msg_id, (void *)&m_data, sizeof(m_data.txt), 0) == -1) {
			printf("Error sending OTP to log in\n");
		}
		printf("OTP sent to log in from OTP generator: %s\n", m_data.txt);

		m_data.type = 3;
		if(msgsnd(msg_id, (void *)&m_data, sizeof(m_data.txt), 0) == -1) {
			printf("Error sending OTP to mail\n");
		}
		printf("OTP sent to mail from OTP generator: %s\n", m_data.txt);

		pid_mail = fork();
		if(pid_mail < 0) {
			perror("fork");
			exit(1);
		}
		else if(pid_mail == 0 {
			if(msgrcv(msg_id, (void *)&m_data, sizeof(m_data.txt), 3, 0) < 0) {
				perror("Error receiving OTP in mail");
			}
			printf("Mail received OTP from OTP generator: %s\n", m_data.txt);

			m_data.type = 4;
			if(msgsnd(msg_id, (void *)&m_data, sizeof(m_data.txt), 0) == -1) {
				printf("Error sending OTP to log in from mail\n");
			}
			printf("OTP sent to log in from mail: %s\n", m_data.txt);
			exit(0);
		}
		else {
			exit(0);
		}
	}
	else {
		if(msgrcv(msg_id, (void *)&m_data, sizeof(m_data.txt), 2, 0) < 0) {
			perror("Error receiving OTP from OTP generator");
		}
		char otp_from_otp[10];
		strcpy(otp_from_otp, m_data.txt);
		printf("Log in received OTP from OTP generator: %s\n", m_data.txt);

		if(msgrcv(msg_id, (void *)&m_data, sizeof(m_data.txt), 4, 0) < 0) {
			perror("Error receiving OTP from mail");
		}
		char otp_from_mail[10];
		strcpy(otp_from_mail, m_data.txt);
		printf("Log in received OTP from mail: %s\n", m_data.txt);

		if(strcmp(otp_from_otp, otp_from_mail) == 0)
			printf("OTP Verified\n");
		else
			printf("OTP Incorrect\n");

		msgctl(msg_id, IPC_RMID, 0);
	}

	return 0;
}
