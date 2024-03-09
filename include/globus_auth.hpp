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

public slots:
	void Authenticate ();
	void TokenGranted ();

signals:
	void GotToken (const QString& token);

private:
	QOAuth2AuthorizationCodeFlow *ga_flow_p;
	const char *ga_auth_endpoint_s;
	const char *ga_token_endpoint_s;


	bool GetEndpoints ();

	char *GetConfigValue (const json_t *data_p, const char * const key_s);

	//void ModifyParametersFunction (QAbstractOAuth :: Stage stage, QMultiMap <QString, QVariant> *parameters_p);

};

#endif // GLOBUS_AUTH_HPP
