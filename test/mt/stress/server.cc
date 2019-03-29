
#include "bench.h"
#include <CORBA.h>
#include <fstream>


using namespace std;

CORBA::ORB_ptr orb = NULL;

class bench_impl
    : public virtual POA_bench
{
public:
    void
    perform()
    {}

    void
    perform_oneway()
    {}

    void
    perform_with_context(CORBA::Context_ptr ctx)
    {}

    void
    perform_oneway_with_context(CORBA::Context_ptr ctx)
    {}

    void shutdown()
    {
#ifdef USE_MEMTRACE
	MemTrace_Report(stderr);
#endif
	//__orb->shutdown(TRUE);
    }

    bench::LongSeq*
    long_seq(const bench::LongSeq& x)
    {
	return new bench::LongSeq(x);
    }
};

int
main (int argc, char* argv[])
{
    string tag = "bench";
    //  cerr << "main thread: " << MICOMT::Thread::self() << endl;
    orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
    if (argc > 1) {
        tag = argv[1];
    }
    CORBA::Object_ptr obj = orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_ptr poa = PortableServer::POA::_narrow (obj);
    CORBA::release (obj);
    PortableServer::POAManager_ptr manager = poa->the_POAManager ();

    CORBA::PolicyList pl2;
    pl2.length(2);
    pl2[0] = poa->create_lifespan_policy (PortableServer::PERSISTENT);
    pl2[1] = poa->create_id_assignment_policy (PortableServer::USER_ID);
    PortableServer::POA_var benchpoa = poa->create_POA ("bench", manager, pl2);

    bench_impl* servant = new bench_impl;
    PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId (tag.c_str());
    benchpoa->activate_object_with_id (*oid, servant);

    CORBA::Object_ptr ref = benchpoa->servant_to_reference (servant);  
    CORBA::String_var ior = orb->object_to_string (ref);
    cout << ior << endl;

    CORBA::release (ref);

    manager->activate ();

    orb->run ();

    CORBA::release (manager);
    CORBA::release (benchpoa);
    CORBA::release (poa);
    CORBA::release (orb);

    return 0;
}
