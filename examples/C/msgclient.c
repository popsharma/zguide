/*
 * client to test reception of messages from instrumentation layer
 */
#include <stdio.h>
#include <zmq.h>
#include <assert.h>

int main (int argc, char *argv [])
{
    printf ("Waiting for messages from server...\n");
    void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    int rc = zmq_connect (subscriber, "tcp://localhost:8100");
    assert (rc == 0);

    char *filter = "";
    int filterLen = 0;
    rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE,
                         filter, filterLen);
    assert (rc == 0);

    unsigned char buffer [512];
    int size;
    while (1) {
	    size = zmq_recv (subscriber, buffer, 255, 0);
	    if (size >= 0) {
	    	int i;
	    	for(i = 0; i < size; i++) {
	    		printf("%02x ", buffer[i]);
	    	}
	    	printf(" <-\n");
	    }
    }

    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
