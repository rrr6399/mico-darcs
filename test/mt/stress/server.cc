
#include "bench.h"
#include <CORBA.h>
#include <coss/CosNaming.h>
#include <fstream>

#include <unistd.h>

using namespace std;
using namespace PortableServer;

CORBA::ORB_ptr orb = NULL;

class bench_impl;

class Deactivator
    : virtual public MICOMT::Thread
{
public:
    Deactivator(bench_impl* s, PortableServer::POA_ptr p, PortableServer::ObjectId i)
    {
        servant_ = s;
        poa_ = PortableServer::POA::_duplicate(p);
        oid_ = new PortableServer::ObjectId(i);
    }

    virtual void _run(void*);
private:
    bench_impl* servant_;
    POA_var poa_;
    ObjectId_var oid_;
};

class bench_impl
    : public virtual POA_bench
{
public:
    bench_impl()
        : deactivator_(NULL), dthr_started_(FALSE), bmtx_(FALSE, MICOMT::Mutex::Normal), cycles_(0)
    {}

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

    CORBA::ULongLong
    perform_with_deactivate()
    {
        {
            MICOMT::AutoLock lock(bmtx_);
            if (!dthr_started_) {
                deactivator_->start();
                dthr_started_ = TRUE;
            }
        }
        // intentionaly without lock
        return cycles_;
    }

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

    void set_deactivator(Deactivator* dthr)
    { deactivator_ = dthr; }

    void inc_cycles()
    {
        // intentionally without lock!
        cycles_++;
    }
private:
    Deactivator* deactivator_;
    bool dthr_started_;
    MICOMT::Mutex bmtx_;
    volatile CORBA::ULongLong cycles_;
};

void
Deactivator::_run(void*)
{
    cerr << "deactivating cycle started." << endl;
    for(;;) {
        usleep(100);
        try {
            poa_->deactivate_object(*oid_);
            poa_->activate_object_with_id(*oid_, servant_);
            servant_->inc_cycles();
        }
        catch (const CORBA::SystemException& ex) {
            cerr << "caught: " << ex._repoid() << endl;
        }
        catch (...) {
            cerr << "caught something really strange!" << endl;
        }
    }
    cerr << "ERROR! Shouldn't be here! Deactivator thread finishes!" << endl;
}

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
    Deactivator* deactivator = new Deactivator(servant, benchpoa, oid);
    servant->set_deactivator(deactivator);

    CORBA::Object_ptr ref = benchpoa->servant_to_reference (servant);  
    CORBA::String_var ior = orb->object_to_string (ref);

    try {
        CORBA::Object_var nsobj =
            orb->resolve_initial_references ("NameService");
        CosNaming::NamingContext_var nc = 
            CosNaming::NamingContext::_narrow (nsobj);
        cerr << "nc: " << (void*)nc.in() << endl;
        if (!CORBA::is_nil (nc)) {
            CosNaming::Name name;
            name.length (1);
            name[0].id = CORBA::string_dup ("server");
            name[0].kind = CORBA::string_dup ("");
            nc->rebind (name, ref);
        }
    }
    catch (CORBA::Exception& ex) {
        cerr << "sysex: " << ex._repoid() << endl;
    }
    catch(...) {
        cerr << "exception." << endl;
    }

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
