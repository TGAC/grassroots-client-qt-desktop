#include "auth.hpp"

#include <QString>
#include <QDir>
#include <QUrl>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QDebug>

#include "curl_tools.h"
#include "string_utils.h"
#include "json_util.h"


quint16 Auth :: AU_DEFAULT_PORT = 1337;

static void ModifyParametersFunction (QAbstractOAuth :: Stage stage, QMultiMap <QString, QVariant> *parameters_p);


Auth :: Auth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port)
: QObject (parent_p)
{
	au_flow_p = new QOAuth2AuthorizationCodeFlow (this);
	au_flow_p -> setScope ("email");

	au_access_token_s  = nullptr;

	connect (au_flow_p, &QOAuth2AuthorizationCodeFlow :: authorizeWithBrowser, &QDesktopServices :: openUrl);

	qDebug () << "client_id_s " << client_id_s << Qt :: endl;
	qDebug () << "client_secret_s " << client_secret_s << Qt :: endl;


	au_flow_p -> setClientIdentifier (client_id_s);
	au_flow_p -> setClientIdentifierSharedKey (client_secret_s);
	au_flow_p -> setModifyParametersFunction (ModifyParametersFunction);

	QOAuthHttpServerReplyHandler *reply_handler_p = new QOAuthHttpServerReplyHandler (reply_port, this);
	au_flow_p -> setReplyHandler (reply_handler_p);

	connect (au_flow_p, &QOAuth2AuthorizationCodeFlow :: granted, this, &Auth :: Granted);

	connect (reply_handler_p, &QOAuthHttpServerReplyHandler :: tokensReceived, this, &Auth :: TokensReceived);
}



bool Auth :: Init ()
{
	bool success_flag = false;

	if (GetEndpoints ())
		{
			QUrl auth_url (au_auth_endpoint_s);
			QUrl token_url (au_token_endpoint_s);
			au_flow_p -> setAuthorizationUrl (auth_url);
			au_flow_p -> setAccessTokenUrl (token_url);

			qDebug () << "auth " << au_auth_endpoint_s << Qt :: endl;
			qDebug () << "token " << au_token_endpoint_s << Qt :: endl;

			success_flag = true;
		}

	return success_flag;
}


void Auth :: Granted ()
{
	const QString token = au_flow_p -> token ();

	qDebug () << "Granted: " << token << Qt :: endl;
}


void Auth :: PrintToken (QString key, const QVariant v) const
{
	if (v.canConvert (QMetaType :: QString))
		{
			QString s = v.toString ();
			qDebug () << key << ": " << s << " (" << v.typeName () << ")" << Qt :: endl;
		}
	else if (v.canConvert (QMetaType :: QVariantList))
		{
			QList <QVariant> l = v.toList ();
			const int size = l.size ();

			qDebug () << "BEGIN list" << Qt :: endl;

			for (int i = 0; i < size; ++ i)
				{
					QVariant value = l.at (i);
					QString s = QString :: number (i);
					PrintToken (s, value);
				}

			qDebug () << "END list" << Qt :: endl;

		}
	else
		{
			qDebug () << key << " has unsupported type " << v.typeName () << Qt :: endl;
		}

}


void Auth :: TokensReceived (const QVariantMap &tokens_r)
{
	QMapIterator <QString, QVariant> itr (tokens_r);

	qDebug () << "BEGIN tokens" << Qt :: endl;

	while (itr.hasNext ())
		{
			itr.next ();

			PrintToken (itr.key (), itr.value ());
		}

	qDebug () << "END tokens" << Qt :: endl;

	QString access_token_key ("access_token");
	QVariant access_token = tokens_r.value (access_token_key);

	if (access_token.isValid ())
		{
			QString s = access_token.toString ();
			QByteArray ba = s.toLocal8Bit ();
			const char *token_s = ba.constData ();

			emit GotAccessToken (token_s);
		}


	emit GotAllTokens (tokens_r);
}


Auth :: ~Auth ()
{
	delete au_flow_p;

	if (au_access_token_s)
		{
			FreeCopiedString (au_access_token_s);
		}
}

// Invoked externally to initiate
void Auth :: Authenticate ()
{
	au_flow_p -> grant ();
}


bool Auth :: IsAccessTokenSet () const
{
	return (au_access_token_s != nullptr);
}


const char *Auth :: GetAccessToken () const
{
	return au_access_token_s;
}


static void ModifyParametersFunction (QAbstractOAuth :: Stage stage, QMultiMap <QString, QVariant> *parameters_p)
{
	qDebug () << "modifyParametersFunction stage=" << static_cast <int> (stage);


	qDebug () << "BEGIN PARAMETERS" << Qt :: endl;
	QMultiMapIterator<QString, QVariant> i (*parameters_p);
	while (i.hasNext ())
		{
			i.next ();
			qDebug () << i.key () << ": " << i.value() << Qt :: endl;
		}
	qDebug () << "END PARAMETERS" << Qt :: endl;

	if (stage == QAbstractOAuth :: Stage :: RequestingAuthorization)
		{
				// Percent-decode the "code" parameter so Google can match it
				QByteArray code = parameters_p -> value ("code").toByteArray ();
				const char *code_s = code.constData();

				qDebug () << ">>> code: \"" << code_s << "\"" << Qt :: endl;
		}
	else if (stage == QAbstractOAuth :: Stage :: RequestingAccessToken)
		{
				// Percent-decode the "code" parameter so Google can match it
				QByteArray code = parameters_p -> value ("code").toByteArray ();

				const char *code_s = code.constData();

				qDebug () << "code: \"" << code_s << "\"" << Qt :: endl;

				parameters_p -> replace("code", QUrl :: fromPercentEncoding (code));
		}
}
