#ifndef GLOBUS_AUTH_HPP
#define GLOBUS_AUTH_HPP


#include <QNetworkReply>
#include <QOAuth2AuthorizationCodeFlow>

#include "jansson.h"

class GlobusAuth : public QObject
{
	Q_OBJECT

public:
	GlobusAuth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port);

	virtual ~GlobusAuth();

	static quint16 GA_DEFAULT_PORT;

	bool IsAccessTokenSet () const;

	const char *GetAccessToken () const;

public slots:
	void Authenticate ();
	void Granted ();
	void TokensReceived (const QVariantMap &tokens_r);

signals:
	void GotGrant (const QString &token_r);
	void GotAllTokens (const QVariantMap &tokens_r);
	void GotAccessToken (const char *access_token_s);

private:
	QOAuth2AuthorizationCodeFlow *ga_flow_p;
	const char *ga_auth_endpoint_s;
	const char *ga_token_endpoint_s;
	char *ga_access_token_s;

	bool GetEndpoints ();

	char *GetConfigValue (const json_t *data_p, const char * const key_s);


};

#endif // GLOBUS_AUTH_HPP
