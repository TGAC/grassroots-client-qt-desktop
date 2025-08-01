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



GlobusAuth :: GlobusAuth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port)
: Auth (parent_p, client_id_s, client_secret_s, reply_port)
{
	au_flow_p -> setScope ("email");
}


GlobusAuth :: ~GlobusAuth ()
{}


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
											if ((au_auth_endpoint_s = GetConfigValue (data_p, "authorization_endpoint")) != NULL)
												{
													if ((au_token_endpoint_s = GetConfigValue (data_p, "token_endpoint")) != NULL)
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
