/*
 * test XPUB/XSUB proxy to capture and forward messages from instrumentation layer
 */

#include <zmq.h>

int main (void)
{
    void *context = zmq_ctx_new ();

    //  This is where the weather server sits
    void *frontend = zmq_socket (context, ZMQ_XSUB);
    // zmq_connect (frontend, "tcp://192.168.55.210:5556");
    zmq_bind (frontend, "tcp://*:5556");

    //  This is our public endpoint for subscribers
    void *backend = zmq_socket (context, ZMQ_XPUB);
    // zmq_bind (backend, "tcp://10.1.1.0:8100");
    zmq_bind (backend, "tcp://*:8100");

    //  Run the proxy until the user interrupts us
    zmq_proxy (frontend, backend, NULL);
    
    zmq_close (frontend);
    zmq_close (backend);
    zmq_ctx_destroy (context);
    return 0;
}
