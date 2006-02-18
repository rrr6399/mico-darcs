//
// Test for etherealize/incarnate
//

#include "hello.h"
#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream>
#else // HAVE_ANSI_CPLUSPLUS_HEADERS
#include <fstream.h>
#endif // HAVE_ANSI_CPLUSPLUS_HEADERS

#include <mico/security/sl3utils.h>

using namespace SL3PM;

using namespace std;

CORBA::ORB_ptr orb = CORBA::ORB::_nil();

class PasswordProcessor_impl
    : virtual public UserPassword::PasswordProcessor,
      virtual public CORBA::LocalObject
{
public:
    virtual CORBA::Boolean
    client_authen_required();

    virtual SL3PM::PrincipalName*
    password_target();

    virtual UserPassword::ErrorCode
    verify_password
    (const CORBA::WChar* username,
     const CORBA::WChar* password,
     SL3PM::PrincipalName_out principal);
};

CORBA::Boolean
PasswordProcessor_impl::client_authen_required()
{
    return TRUE;
}

SL3PM::PrincipalName*
PasswordProcessor_impl::password_target()
{
    SL3PM::PrincipalName* result = new SL3PM::PrincipalName;
    result->the_type = (const char*)SL3PM::NT_StringName;
    result->the_name.length(1);
    result->the_name[0] = L"@objectsecurity.com";
    return result;
}

UserPassword::ErrorCode
PasswordProcessor_impl::verify_password
(const CORBA::WChar* username,
 const CORBA::WChar* password,
 SL3PM::PrincipalName_out principal)
{
    //cerr << "PasswordProcessor_impl::verify_password" << endl;
    wstring name = username;
    wstring pw = password;
    if (name == L"relay") {
	if (pw == L"relay_pw") {
	    principal = new SL3PM::PrincipalName;
	    principal->the_type = (const char*)SL3PM::NT_StringName;
	    principal->the_name.length(1);
	    principal->the_name[0] = L"relay";
	    return UserPassword::EC_Success;
	}
	return UserPassword::EC_BadPassword;
    }
    return UserPassword::EC_NoUser;
}

class HelloWorld_impl
    : virtual public POA_HelloWorld
{
public:
    void hello();
};

void
HelloWorld_impl::hello()
{
    CORBA::Object_var obj = orb->resolve_initial_references
	("SecurityLevel3::SecurityCurrent");
    SecurityLevel3::SecurityCurrent_var current
	= SecurityLevel3::SecurityCurrent::_narrow(obj);
    assert(!CORBA::is_nil(current));
    SecurityLevel3::ClientCredentials_ptr creds
	= current->client_credentials();
    Principal_var princ = creds->client_principal();
    PrincipalName name = princ->the_name();
    if (princ->the_type() == PT_Proxy) {
	wcout << "server: operation executed by: ``" << name.the_name[0].in() << "''" << endl;
    }
    if (princ->the_type() == PT_Quoting) {
	CORBA::add_ref(princ);
	QuotingPrincipal_var qp = QuotingPrincipal::_downcast(princ);
	Principal_ptr speaking = qp->speaking();
	PrincipalName speaking_name = speaking->the_name();
	wcout << "server: operation executed by: ``" << speaking_name.the_name[0].in()
	      << "'' on behalf of ``" << name.the_name[0].in() << "''" << endl;
    }
    wcout << "server: Hello World" << endl;
}

int
main (int argc, char *argv[])
{
    //wcout << "wcout init" << endl;
    try {
	orb = CORBA::ORB_init(argc, argv);
	CORBA::Object_var obj = orb->resolve_initial_references
	    ("SecurityLevel3::SecurityManager");
	SecurityLevel3::SecurityManager_var secman
	    = SecurityLevel3::SecurityManager::_narrow(obj);
	assert(!CORBA::is_nil(secman));
	SecurityLevel3::CredentialsCurator_var curator
	    = secman->credentials_curator();
	assert(!CORBA::is_nil(curator));
	// first we need to obtain transport credentials
	obj = orb->resolve_initial_references("SL3TCPIP::ArgBuilderFactory");
	SL3AQArgs::ArgBuilderFactory_var ts_fact
	    = SL3AQArgs::ArgBuilderFactory::_narrow(obj);
	assert(!CORBA::is_nil(ts_fact));
	SL3TCPIP::TCPIPArgBuilder_var ts_builder
	    = SL3TCPIP::TCPIPArgBuilder::_narrow
	    (ts_fact->create_arg_builder(SL3CM::CU_AcceptOnly));
	assert(!CORBA::is_nil(ts_builder));
	SL3AQArgs::Argument_var ts_args = ts_builder->reap_args();
	obj = orb->resolve_initial_references
	    ("TransportSecurity::SecurityManager");
	TransportSecurity::SecurityManager_var ts_secman
	    = TransportSecurity::SecurityManager::_narrow(obj);
	assert(!CORBA::is_nil(ts_secman));
	TransportSecurity::CredentialsCurator_var ts_curator
	    = ts_secman->credentials_curator();
	TransportSecurity::CredentialsAcquirer_ptr ts_acquirer
	    = ts_curator->acquire_credentials(ts_args);
	TransportSecurity::OwnCredentials_ptr ts_creds = ts_acquirer->get_credentials(TRUE);
	//MICOSL3Utils::PP::print_own_credentials(&wcout, ts_creds);

	obj = orb->resolve_initial_references("SL3CSI::ArgBuilderFactory");
	SL3AQArgs::ArgBuilderFactory_var csi_fact
	    = SL3AQArgs::ArgBuilderFactory::_narrow(obj);
	assert(!CORBA::is_nil(csi_fact));
	SL3CSI::CSIArgBuilder_var csi_builder
	    = SL3CSI::CSIArgBuilder::_narrow
	    (csi_fact->create_arg_builder(SL3CM::CU_AcceptOnly));
	assert(!CORBA::is_nil(csi_builder));

	csi_builder->add_transport_credentials(ts_creds);
	UserPassword::PasswordProcessor_var processor = new PasswordProcessor_impl;
	csi_builder->add_password_processor(processor);
	SL3AQArgs::Argument_var args = csi_builder->reap_args();

	SecurityLevel3::CredentialsAcquirer_var acquirer
	    = curator->acquire_credentials(args);
	SecurityLevel3::OwnCredentials_var creds = acquirer->get_credentials(TRUE);
	//wcout << "server: OwnCredentials:" << endl;
	//MICOSL3Utils::PP::print_own_credentials(&wcout, creds);

	CORBA::Object_var poaobj = orb->resolve_initial_references("RootPOA");
	PortableServer::POA_var poa = PortableServer::POA::_narrow(poaobj);
	PortableServer::POAManager_var mgr = poa->the_POAManager();

	HelloWorld_impl* servant = new HelloWorld_impl;
	PortableServer::ObjectId_var oid = poa->activate_object(servant);
	CORBA::Object_var ref = poa->id_to_reference(oid.in());
	ofstream of ("hello.ref");
	CORBA::String_var str = orb->object_to_string(ref);
	of << str.in() << endl;
	of.close();

	wcout << "server: Running." << endl;

	mgr->activate();
	orb->run();
    } catch (CORBA::UserException& ex) {
	wcout << "UserException caught: " << ex._repoid() << endl;
    } catch (CORBA::SystemException_catch& ex) {
	wcout << "SystemException caught: " << ex._repoid() << endl;
    } catch (...) {
	wcout << "... caught!" << endl;
    }
    return 0;
}
