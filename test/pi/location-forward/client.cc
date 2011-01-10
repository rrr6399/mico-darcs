#include "hello.h"

#include <cstdio>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <direct.h>
#endif


using namespace std;
using namespace PortableInterceptor;

HelloWorld_var hello2;
    
class MyClientInterceptor
    : virtual public PortableInterceptor::ClientRequestInterceptor,
      virtual public CORBA::LocalObject
{
    string nm;
    int count_;
    int id;
public:
    MyClientInterceptor() : count_(0)
    { nm = ""; }

    MyClientInterceptor(const char * name, int _id) : count_(0)
    { 
    nm = name;
    id = _id;
    }
	
    char* name()
    { return CORBA::string_dup(nm.c_str()); }

    void destroy()
    { cerr << this->name() << " destroy" << endl; }

    void send_request(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: interceptor[" << id << "]: send_request" << endl;
	/*
	if (!count_) { 
	    CORBA::Object_ptr target = ri->target();
	    count_++;
	    throw PortableInterceptor::ForwardRequest(target, false);
	}
	*/
	
    }
    
    void send_poll(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: interceptor[" << id << "]: send_poll" << endl;
    }
    
    void receive_reply(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: interceptor[" << id << "]: receive_reply" << endl;
    }
    
    void receive_exception(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: interceptor[" << id << "]: receive_exception" << endl;
	if (id == 2) {
	assert(!CORBA::is_nil(hello2));
    throw ForwardRequest(hello2, false);
    }
    }
    
    void receive_other(PortableInterceptor::ClientRequestInfo_ptr ri)
    {
	cout << "client: interceptor[" << id << "]: receive_other" << endl;
    }
};

class MyInitializer
    : virtual public PortableInterceptor::ORBInitializer,
      virtual public CORBA::LocalObject
{
public:
    MyInitializer() {}
    ~MyInitializer() {}

    virtual void pre_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
    	// register interceptor	
    	MyClientInterceptor * interceptor = new MyClientInterceptor("MyInterceptor", 1);
    	info->add_client_request_interceptor(interceptor);
    	MyClientInterceptor * interceptor2 = new MyClientInterceptor("MyInterceptor", 2);
    	info->add_client_request_interceptor(interceptor2);          
    	MyClientInterceptor * interceptor3 = new MyClientInterceptor("MyInterceptor", 3);
    	info->add_client_request_interceptor(interceptor3);          
    	MyClientInterceptor * interceptor4 = new MyClientInterceptor("MyInterceptor", 4);
    	info->add_client_request_interceptor(interceptor4);          
    	MyClientInterceptor * interceptor5 = new MyClientInterceptor("MyInterceptor", 5);
    	info->add_client_request_interceptor(interceptor5);
    }
    
    virtual void post_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
    	// nothing
    }
};


int
main (int argc, char *argv[])
{
    MyInitializer* ini = new MyInitializer;
    PortableInterceptor::register_orb_initializer(ini);

    CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

    char pwd[256], uri[300], uri2[300];
    sprintf (uri, "file://%s/hello.ref", getcwd(pwd, 256));
    sprintf (uri2, "file://%s/hello2.ref", getcwd(pwd, 256));

    // for mem-leak checking, please uncomment following loop
    //for (unsigned long i = 0; i < 100000; i++) {
    CORBA::Object_var obj = orb->string_to_object (uri);
    assert(!CORBA::is_nil(obj));
    HelloWorld_var hello = HelloWorld::_narrow(obj);
    assert(!CORBA::is_nil(hello));
    hello->hello(false);

    CORBA::Object_var obj2 = orb->string_to_object (uri2);
    assert(!CORBA::is_nil(obj2));
    hello2 = HelloWorld::_narrow(obj2);
    assert(!CORBA::is_nil(hello2));
    hello->hello(true);
    //}
    return 0;
}
