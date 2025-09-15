#ifndef AUTH_HPP
#define AUTH_HPP

#include <QNetworkReply>
#include <QOAuth2AuthorizationCodeFlow>

#include "jansson.h"

class Auth : public QObject
{
	Q_OBJECT

public:
	Auth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port);

	virtual ~Auth();

	static quint16 AU_DEFAULT_PORT;

	bool IsAccessTokenSet () const;

	const char *GetAccessToken () const;

	bool Init ();


public slots:
	void Authenticate ();
	void Granted ();
	void TokensReceived (const QVariantMap &tokens_r);

signals:
	void GotGrant (const QString &token_r);
	void GotAllTokens (const QVariantMap &tokens_r);
	void GotAccessToken (const char *access_token_s);

protected:
	QOAuth2AuthorizationCodeFlow *au_flow_p;
	const char *au_auth_endpoint_s;
	const char *au_token_endpoint_s;
	char *au_access_token_s;

	virtual bool GetEndpoints () = 0;

	void PrintToken (QString key, const QVariant v) const;

};


#endif // AUTH_HPP
