#ifndef GLOBUS_AUTH_HPP
#define GLOBUS_AUTH_HPP


#include <QObject>

#include "auth.hpp"

class GlobusAuth : public Auth
{
	Q_OBJECT

public:
	GlobusAuth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port);

	virtual ~GlobusAuth ();

protected:
	virtual bool GetEndpoints ();

private:
	char *GetConfigValue (const json_t *data_p, const char * const key_s);

};

#endif // GLOBUS_AUTH_HPP
