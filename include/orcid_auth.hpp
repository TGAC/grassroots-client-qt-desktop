#ifndef ORCID_AUTH_H
#define ORCID_AUTH_H

#include "auth.hpp"

class OrcidAuth : public Auth
{
	Q_OBJECT

public:
	OrcidAuth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port);

	virtual ~OrcidAuth ();

protected:
	virtual bool GetEndpoints ();

};


#endif // ORCID_AUTH_H
