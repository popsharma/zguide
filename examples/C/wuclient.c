//  Weather update client
//  Connects SUB socket to tcp://localhost:5556
//  Collects weather updates and finds avg temp in zipcode

#include "zhelpers.h"

int main (int argc, char *argv [])
{
    //  Socket to talk to server
    printf ("Collecting updates from weather server...\n");
    void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    // int rc = zmq_connect (subscriber, "tcp://localhost:5556");
    int rc = zmq_connect (subscriber, "tcp://localhost:8100");
    // int rc = zmq_bind (subscriber, "tcp://*:5556");
    assert (rc == 0);

    //  Subscribe to zipcode, default is NYC, 10001
    // char *filter = (argc > 1)? argv [1]: "10001 ";
    // char *filter = (argc > 1)? argv [1]: "1 ";
    char *filter = "";
    rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE,
                         filter, strlen (filter));
    assert (rc == 0);

    //  Process 10 updates
    int update_nbr;
    long total_temp = 0;
    for (update_nbr = 0; update_nbr < 10; update_nbr++) {
        char *string = s_recv (subscriber);

        int zipcode, temperature, relhumidity;
        sscanf (string, "%d %d %d",
            &zipcode, &temperature, &relhumidity);
        total_temp += temperature;
        free (string);
    	printf("temperature: %d\n", temperature);
    }
    printf ("Average temperature for zipcode '%s' was %dF\n",
        filter, (int) (total_temp / update_nbr));

    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
