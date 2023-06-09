
#include "bench.h"
#include <CORBA.h>
#include <coss/CosNaming.h>
#include <coss/CosEventComm.h>
#include <coss/CosEventChannelAdmin.h>
#include <iostream>
#include <string>


using namespace std;

bench_ptr bench;
int num;
int events_received;
int delay;
int thread_number;
string method = "";
CORBA::Context_var ctx, ctx2;
string ior;

#ifdef HAVE_THREADS

class Notifier : public CORBA::RequestCallback {
private:
    MICOMT::Locked<std::list<CORBA::Request_var> > requests;
public:
    void add_request (CORBA::Request_ptr r) {
        MICOMT::AutoLock l(requests);
        cerr << "add_request: " << requests.size() << endl;
        requests.push_back(CORBA::Request::_duplicate(r));
    }
    void callback (CORBA::Request_ptr r, CORBA::RequestCallback::Event ev) {
        cerr << "callback." << endl;
        assert (ev == CORBA::RequestCallback::RequestDone);
        MICOMT::AutoLock lock(requests);
        list<CORBA::Request_var>::iterator j = requests.begin();
        for (; j != requests.end(); ++j) {
            CORBA::Request_ptr req = (*j).in();
            if (r == req) {
                MICO_CATCHANY (req->get_response ());
                if (req->env()->exception()) {
                    cerr << "eventd: push failed with: "
		     << req->env()->exception() << endl;
                }
                cerr << "erasing req: " << (void*)req << " from queue size: " << requests.size() << endl;
                requests.erase (j);
                break;
            }
        }
    }

};

class Invoker
    : virtual public MICOMT::Thread
{
public:
    void _run(void*)
    {
	if (method == "perform") {
	    for (int i=0; i<num; i++) {
		bench->perform();
	    }
	} else if (method == "perform_oneway") {
	    for (int i=0; i<num; i++) {
		bench->perform_oneway();
	    }
	} else if (method == "perform_with_context") {
	    for (int i=0; i<num; i++) {
		bench->perform_with_context(ctx2);
	    }
	} else if (method == "perform_oneway_with_context") {
	    for (int i=0; i<num; i++) {
		bench->perform_oneway_with_context(ctx2);
	    }
	} else if (method == "non_existent") {
	    for (int i=0; i<num; i++) {
		bench->_non_existent();
	    }
	} else {
	    assert(0);
	}
    }
};

#endif // HAVE_THREADS

class Consumer_impl : virtual public POA_CosEventComm::PushConsumer {
public:
  Consumer_impl () {}

  void push (const CORBA::Any& data);
  void disconnect_push_consumer ();
};

void Consumer_impl::push (const CORBA::Any& data) {
    events_received++;
}

void Consumer_impl::disconnect_push_consumer () {
  cout << "Consumer: disconnected !" << endl;
}

int
main (int argc, char* argv[])
{
  int exnum = 0;
  long succ = 0;
  CORBA::ULongLong cycles = 0;
try {
    CORBA::ORB_ptr orb = CORBA::ORB_init (argc, argv, "mico-local-orb");
  
    cout << "argv[0]: " << argv[0] << endl;
    cout << "argv[1]: " << argv[1] << endl;
    cout << "argv[2]: " << argv[2] << endl;
    cout << "argv[3]: " << argv[3] << endl;
    cout << "argv[4]: " << argv[4] << endl;
    cout << "argv[5]: " << argv[5] << endl;

    string s = argv[1];
    method = argv[2];
    cout << "binding: " << s << endl;
    if (s.find("bind") != string::npos) {
	cout << "binding..." << endl;
        size_t pos = -1;
	CORBA::Object_ptr obj = CORBA::Object::_nil();
        if ((pos = s.find("#")) != string::npos) {
            string tag;
            tag = s.substr(pos + 1, s.size());
            cout << "tag: `" + tag << "'" << endl;
            CORBA::ORB::ObjectTag_var t = CORBA::ORB::string_to_tag(tag.c_str());
            obj = orb->bind ("IDL:bench:1.0", t, "inet:localhost:7788");
        }
        else {
            obj = orb->bind ("IDL:bench:1.0");
        }
	bench = bench::_narrow (obj);
	cout << "binded." << endl;
    }
    else if (s == "ior") {
	cin >> ior;
	CORBA::Object_ptr obj = orb->string_to_object(ior.c_str());
	bench = bench::_narrow (obj);
    }
    else if (s == "nsd") {
        CORBA::Object_var nsobj =
            orb->resolve_initial_references ("NameService");
        CosNaming::NamingContext_var nc = 
            CosNaming::NamingContext::_narrow (nsobj);
        if (CORBA::is_nil (nc)) {
            cerr << "oops, I cannot access the Naming Service!" << endl;
            exit (1);
        }

        CosNaming::Name name;
        name.length (1);
        name[0].id = CORBA::string_dup ("server");
        name[0].kind = CORBA::string_dup ("");
  
        CORBA::Object_var obj;
        cout << "Looking up Server ... " << flush;
        try {
            obj = nc->resolve (name);
        }
        catch (...) {
        }
        cout << "done." << endl;

        bench = bench::_narrow (obj);
    }
    else {
	cerr << "unsupported binding method." << endl;
	exit(2);
    }
    if (CORBA::is_nil(bench) && s != "nsd") {
	cerr << "can't bind to the bench interface" << endl;
	exit(1);
    }
    cout << "operation: " << method << endl;
    num = atoi (argv[3]);
    cout << "num: " << num << endl;
    int tnum = num / 100;
    cout << "tnum: " << tnum << endl;
    cout << "invocation: " << tnum*100 << endl;

    delay = atoi (argv[4]);
    cout << "invoke delay: " << delay << endl;

    CORBA::Any any;
    orb->get_default_context( ctx );

    ctx->create_child ("child", ctx2);

    any <<= "aa";
    ctx->set_one_value ("aa", any);

    any <<= "ab";
    ctx->set_one_value ("ab", any);

    any <<= "bb";
    ctx->set_one_value ("bb", any);

    any <<= "aa-child";
    ctx2->set_one_value ("aa", any);

#ifdef HAVE_THREADS
    thread_number = atoi (argv[5]);
    cout << "number of threads: " << thread_number << endl;
    if (thread_number > 0) {
	Invoker** thr_array = new Invoker*[thread_number];
	for (int i=0; i<thread_number; i++) {
	    thr_array[i] = new Invoker;
	}
	for (int i=0; i<thread_number; i++) {
	    thr_array[i]->start();
	}
	for (int i=0; i<thread_number; i++) {
	    thr_array[i]->wait();
	}
	//bench->shutdown();
	orb->destroy();
	return 0;
    }
#endif // HAVE_THREADS
    if (method == "perform") {
	for (int j=0; j<100; j++) {
	    cout << "\r" << "progress: " << j << "%" << flush;
     	    for (int i=0; i<tnum; i++) {
                bench->perform();
		//::sleep (delay);
	    }
	}
    } else if (method == "perform_oneway") {
	for (int j=0; j<100; j++) {
	    cout << "\r" << "progress: " << j << "%" << flush;
	    for (int i=0; i<tnum; i++) {
                bench->perform_oneway();
		//::sleep (delay);
	    }
	}
    } else if (method == "perform_with_context") {
	for (int j=0; j<100; j++) {
	    cout << "\r" << "progress: " << j << "%" << flush;
	    for (int i=0; i<tnum; i++) {
                bench->perform_with_context(ctx2);
		//::sleep (delay);
	    }
	}
    } else if (method == "perform_oneway_with_context") {
	for (int j=0; j<100; j++) {
	    cout << "\r" << "progress: " << j << "%" << flush;
	    for (int i=0; i<tnum; i++) {
                bench->perform_oneway_with_context(ctx2);
		//::sleep (delay);
	    }
	}
    }
    if (method == "perform_with_deactivate") {
	for (int j=0; j<100; j++) {
	    cout << "\r" << "progress: " << j << "%" << flush;
            cout << " (activation-deactivation cycles: " << cycles << "/exceptions: " << exnum << ")" << flush;
	    for (int i=0; i<tnum; i++) {
                try {
		    cycles = bench->perform_with_deactivate();
                } catch (const CORBA::OBJECT_NOT_EXIST&) {
                    exnum++;
                    CORBA::Object_ptr obj = orb->string_to_object(ior.c_str());
                    bench = bench::_narrow (obj);
                }
		//::sleep (delay);
	    }
	}
    }
    else if (method == "non_existent") {
	for (int j=0; j<100; j++) {
	    cout << "\r" << "progress: " << j << "%" << flush;
	    for (int i=0; i<tnum; i++) {
                bench->_non_existent();
		//::sleep (delay);
	    }
        }
    }
    else if (method == "long_seq") {
	CORBA::ULong len = atoi(argv[6]);
	cerr << "seq len: " << len << endl;
	bench::LongSeq x;
	bench::LongSeq_var ret;
	x.length(len);
	for (int j=0; j<100; j++) {
	    cout << "\r" << "progress: " << j << "%" << flush;
	    for (int i=0; i<tnum; i++) {
                ret = bench->long_seq(x);
		//::sleep (delay);
	    }
	}
    }
    else if (method == "send_deferred") {
        Notifier* notif = new Notifier();
	for (int j=0; j<100; j++) {
	    cout << "\r" << "progress: " << j << "%" << flush;
	    for (int i=0; i<tnum; i++) {
                CORBA::Request_var req = bench->_request("perform");
                notif->add_request(req);
                req->send_deferred(notif);
                req->poll_response();
            }
        }
        while (orb->work_pending()) {
            orb->perform_work();
        }
    }
    else if (method == "send_event") {
        CORBA::Object_var nsobj =
            orb->resolve_initial_references ("NameService");
        assert (! CORBA::is_nil (nsobj));
  
        CosNaming::NamingContext_var nc = 
            CosNaming::NamingContext::_narrow (nsobj);
        assert (! CORBA::is_nil (nc));

        CosNaming::Name name;
        name.length (1);
        name[0].id = CORBA::string_dup ("MyEventChannel");
        name[0].kind = CORBA::string_dup ("");

        cerr << "looking for EventChannel" << endl;
        CORBA::Object_var obj = nc->resolve (name);
  
        CosEventChannelAdmin::EventChannel_var event_channel;
        CosEventChannelAdmin::SupplierAdmin_var supplier_admin;
        CosEventChannelAdmin::ProxyPushConsumer_var proxy_consumer;

        event_channel = CosEventChannelAdmin::EventChannel::_narrow (obj);
        assert (! CORBA::is_nil (event_channel));
        cerr << "EventChannel found !" << endl;

        supplier_admin = event_channel->for_suppliers ();
        assert (! CORBA::is_nil (supplier_admin));

        proxy_consumer = supplier_admin->obtain_push_consumer ();
        assert (! CORBA::is_nil (proxy_consumer));
        cerr << "ProxyPushConsumer obtained !" << endl;

        CORBA::Any any;
	for (int j=0; j<100; j++) {
	    cout << "\r" << "progress: " << j << "%" << flush;
	    for (int i=0; i<tnum; i++) {
                any <<= (CORBA::ULong)i;
                proxy_consumer->push (any);
            }
        }
    }
    else if (method == "recv_event") {
        events_received = 0;
        CORBA::Object_var poa_obj = orb->resolve_initial_references("RootPOA");
        PortableServer::POA_var poa = PortableServer::POA::_narrow(poa_obj);
        assert(!CORBA::is_nil(poa));
        PortableServer::POAManager_var mgr = poa->the_POAManager();

        Consumer_impl* consumer_impl = new Consumer_impl ();
        CosEventComm::PushConsumer_var consumer = consumer_impl->_this();

        CORBA::Object_var nsobj =
            orb->resolve_initial_references ("NameService");
        assert (! CORBA::is_nil (nsobj));
  
        CosNaming::NamingContext_var nc = 
            CosNaming::NamingContext::_narrow (nsobj);
        assert (! CORBA::is_nil (nc));

        CosNaming::Name name;
        name.length (1);
        name[0].id = CORBA::string_dup ("EventChannelFactory");
        name[0].kind = CORBA::string_dup ("");
        CORBA::Object_var obj = nc->resolve (name);
  
        SimpleEventChannelAdmin::EventChannelFactory_var ecf;
        CosEventChannelAdmin::EventChannel_var event_channel;
        CosEventChannelAdmin::ConsumerAdmin_var consumer_admin;
        CosEventChannelAdmin::ProxyPushSupplier_var proxy_supplier;

        ecf = SimpleEventChannelAdmin::EventChannelFactory::_narrow (obj);
        assert (! CORBA::is_nil (ecf));

        name[0].id = CORBA::string_dup ("MyEventChannel");
        name[0].kind = CORBA::string_dup ("");

        CORBA::Boolean channel_exist = FALSE;
        try {
            obj = nc->resolve (name);
            event_channel = CosEventChannelAdmin::EventChannel::_narrow (obj);
            assert (! CORBA::is_nil (event_channel));
            channel_exist = TRUE;
        }
        catch (...) {
        }
        if (!channel_exist) {
            event_channel = ecf->create_eventchannel ();
            assert (! CORBA::is_nil (event_channel));

            nc->bind (name, 
                      CosEventChannelAdmin::EventChannel::_duplicate (event_channel));
        }
        consumer_admin = event_channel->for_consumers ();
        assert (! CORBA::is_nil (consumer_admin));

        proxy_supplier = consumer_admin->obtain_push_supplier ();
        assert (! CORBA::is_nil (proxy_supplier));
        cerr << "ProxyPushSupplier obtained !" << endl;

        proxy_supplier->connect_push_consumer (CosEventComm::PushConsumer::_duplicate (consumer));
 
        mgr->activate();
        while (events_received < num) {
            orb->perform_work();
        }
    } else {
	assert(0);
    }
#ifdef USE_MEMTRACE
    MemTrace_Report(stderr);
#endif
    if (!CORBA::is_nil(bench))
        bench->shutdown();
    orb->shutdown(TRUE);
    orb->destroy();
    cout << "client: finished." << endl;
}
catch (const CORBA::Exception& ex) {
    ex._print(cerr);
    cerr << "  exception thrown" << endl;
}
catch (...) {
    cerr << "... unknown exception thrown" << endl;
}
    return 0;
}
