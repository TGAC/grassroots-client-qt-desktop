#include "orcid_auth.hpp"

#include <QString>
#include <QDir>
#include <QUrl>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QDebug>



OrcidAuth :: OrcidAuth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port)
: Auth (parent_p, client_id_s, client_secret_s, reply_port)
{
//	au_flow_p -> setScope ("authenticate");

}


OrcidAuth :: ~OrcidAuth ()
{}



bool OrcidAuth :: GetEndpoints ()
{
	au_auth_endpoint_s = "https://orcid.org/oauth/authorize";
	au_token_endpoint_s = "https://orcid.org/oauth/token";

	return true;
}

