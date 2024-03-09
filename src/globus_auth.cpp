#include "globus_auth.hpp"

#include <QString>
#include <QDir>
#include <QUrl>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QDebug>

#include "curl_tools.h"
#include "string_utils.h"
#include "json_util.h"


static void ModifyParametersFunction (QAbstractOAuth :: Stage stage, QMultiMap <QString, QVariant> *parameters_p);


GlobusAuth *GlobusAuth :: GetGlobusAuth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port)
{

}


GlobusAuth :: GlobusAuth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port)
: QObject (parent_p)
{
	ga_flow_p = new QOAuth2AuthorizationCodeFlow (this);
	ga_flow_p -> setScope ("email");

	connect (ga_flow_p, &QOAuth2AuthorizationCodeFlow :: authorizeWithBrowser, &QDesktopServices :: openUrl);

	if (GetEndpoints ())
		{
			QUrl auth_url (ga_auth_endpoint_s);
			QUrl token_url (ga_token_endpoint_s);

			qDebug () << "auth " << ga_auth_endpoint_s << Qt :: endl;
			qDebug () << "token " << ga_token_endpoint_s << Qt :: endl;

			qDebug () << "client_id_s " << client_id_s << Qt :: endl;
			qDebug () << "client_secret_s " << client_secret_s << Qt :: endl;


			ga_flow_p -> setClientIdentifier (client_id_s);
			ga_flow_p -> setClientIdentifierSharedKey (client_secret_s);
			ga_flow_p -> setAuthorizationUrl (auth_url);
			ga_flow_p -> setAccessTokenUrl (token_url);
			ga_flow_p -> setScope ("email");
			ga_flow_p -> setModifyParametersFunction (ModifyParametersFunction);

			QOAuthHttpServerReplyHandler *reply_handler_p = new QOAuthHttpServerReplyHandler (reply_port, this);
			ga_flow_p -> setReplyHandler (reply_handler_p);

			connect (ga_flow_p, &QOAuth2AuthorizationCodeFlow :: granted, this, &GlobusAuth :: TokenGranted);
		}
}

void GlobusAuth :: TokenGranted ()
{
	const QString token = ga_flow_p -> token ();
	emit GotToken (token);
}

GlobusAuth :: ~GlobusAuth ()
{
	delete ga_flow_p;
}

// Invoked externally to initiate
void GlobusAuth :: Authenticate ()
{
	ga_flow_p -> grant ();
}


static void ModifyParametersFunction (QAbstractOAuth :: Stage stage, QMultiMap <QString, QVariant> *parameters_p)
{
	qDebug () << "modifyParametersFunction stage=" << static_cast <int> (stage);
	if (stage == QAbstractOAuth :: Stage :: RequestingAuthorization)
		{
				// The only way to get refresh_token from Google Cloud
				parameters_p -> insert ("access_type", "offline");
				parameters_p -> insert ("prompt", "consent");
		}
	else if (stage == QAbstractOAuth :: Stage :: RequestingAccessToken)
		{
				// Percent-decode the "code" parameter so Google can match it
				QByteArray code = parameters_p -> value ("code").toByteArray ();
				parameters_p -> replace("code", QUrl :: fromPercentEncoding (code));
		}
}

bool GlobusAuth :: GetEndpoints ()
{
	bool success_flag = false;
	const char * const metadata_url_s = "https://auth.globus.org/.well-known/openid-configuration";
	CurlTool *curl_p = AllocateMemoryCurlTool (8192);

	if (curl_p)
		{
			if (SetUriForCurlTool (curl_p, metadata_url_s))
				{
					CURLcode c = RunCurlTool (curl_p);

					if (c == CURLE_OK)
						{
							const char *data_s = GetCurlToolData (curl_p);

							if (data_s)
								{
									json_error_t err;
									json_t *data_p = json_loads (data_s, 0, &err);

									if (data_p)
										{
											if ((ga_auth_endpoint_s = GetConfigValue (data_p, "authorization_endpoint")) != NULL)
												{
													if ((ga_token_endpoint_s = GetConfigValue (data_p, "token_endpoint")) != NULL)
														{
															success_flag = true;
														}

												}

											json_decref (data_p);
										}

								}
						}
					else
						{

						}
				}

			FreeCurlTool (curl_p);
		}

	return success_flag;
}


char *GlobusAuth :: GetConfigValue (const json_t *data_p, const char * const key_s)
{
	char *copied_value_s = nullptr;
	const char *value_s = GetJSONString (data_p, key_s);

	if (value_s)
		{
			copied_value_s = EasyCopyToNewString (value_s);

			if (!copied_value_s)
				{
				}
		}

	return copied_value_s;
}
