#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

// Define a structure for the message queue
struct msg_buffer {
    long msg_type;
    char msg_text[1024];
};

int main() {
    int msgid;
    key_t key;
    struct msg_buffer msg;
    ssize_t msg_size;

    // Generate the key for the message queue
    key = ftok("/tmp/1234", 'a');
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Open the message queue
    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Print the size of the message queue
    struct msqid_ds buf;
    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("Message Queue ID: %d\n", msgid);
    printf("Number of messages in queue: %lu\n", buf.msg_qnum);

    // Receive a message from the message queue
    msg_size = msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, 0);
    if (msg_size == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    // Print the size and type of the received message
    printf("Received message type: %ld\n", msg.msg_type);
    printf("Size of the message: %zd\n", msg_size);

    return 0;
}