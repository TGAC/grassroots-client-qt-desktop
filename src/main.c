/*
 ** Copyright 2014-2016 The Earlham Institute
 **
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 **
 **     http://www.apache.org/licenses/LICENSE-2.0
 **
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>


#include "json_tools.h"
#include "json_util.h"
#include "request_tools.h"
#include "parameter_set.h"
#include "client.h"
#include "byte_buffer.h"
#include "connection.h"
#include "string_utils.h"
#include "streams.h"
#include "client_ui_api.h"


#ifdef _DEBUG
#define STANDALONE_CLIENT_DEBUG	(STM_LEVEL_FINER)
#else
#define STANDALONE_CLIENT_DEBUG	(STM_LEVEL_NONE)
#endif


/*************************************/
/******* FUNCTION DEFINITIONS  *******/
/*************************************/

int main (int argc, char *argv [])
{
	if (argc < 3)
		{
			printf (
					"USAGE: grassroots-qt-client "
					"\t-h <server_url>, the web address of the Grassroots server to connect to.\n"
					"\t--list-all, get all available services from the Grassroots server.\n"
					"\t--get-service <service name> get named services from the Grassroots server.\n"
					"\t--keyword-search <keyword>, perform a keyword search against all keyword-aware services.\n"
					"\t--list-interested <resource>, get all services that are able to run against a given resource.\n"
					"\t--get-service-data <service_name>, get the JSON for Lucene indexing.\n"
					"\t\tThe resource is in the form <protocol>://<name> e.g. file:///home/test.fa, https://my.data/object, irods://data.fa\n"
					);
			return 0;
		}
	else
		{
			int i = 1;
			const char *hostname_s = "localhost";
			char *protocol_s = NULL;
			char *query_s = NULL;
			const char *keyword_s = NULL;
			Operation op = OP_NONE;
			bool web_server_flag = true;
			CURLcode c;
			Connection *connection_p = NULL;

			while (i < argc)
				{
					if (strcmp (argv [i], "-h") == 0)
						{
							if ((i + 1) < argc)
								{
									hostname_s = argv [++ i];
								}
							else
								{
									printf ("Hostname argument missing");
								}

						}
					else if (strcmp (argv [i], "--list-all") == 0)
						{
							op = OP_LIST_ALL_SERVICES;
						}
					else if (strcmp (argv [i], "--list-interested") == 0)
						{
							if ((i + 1) < argc)
								{
									const char *arg_s = argv [++ i];
									const char *delimiter_s = "://";
									const char *ptr = strstr (arg_s, delimiter_s);

									if (ptr)
										{
											protocol_s = CopyToNewString (arg_s, ptr - arg_s, false);

											if (protocol_s)
												{
													query_s = EasyCopyToNewString (ptr + strlen (delimiter_s));

													if (query_s)
														{
															op = OP_LIST_INTERESTED_SERVICES;
														}
													else
														{
															printf ("Failed to get resource name from \"%s\"", arg_s);
														}
												}
											else
												{
													printf ("Failed to copy resource type from \"%s\"", arg_s);
												}
										}
									else
										{
											printf ("Failed to find resource separator \"%s\" in \"%s\"", delimiter_s, arg_s);
										}

								}
							else
								{
									printf ("hostname argument missing");
								}
						}
					else if (strcmp (argv [i], "--keyword-search") == 0)
						{
							op = OP_RUN_KEYWORD_SERVICES;


							if ((i + 1) < argc)
								{
									keyword_s = argv [++ i];
								}
							else
								{
									printf ("Keyword argument missing");
								}
						}
					else if (strcmp (argv [i], "--get-service") == 0)
						{
							op = OP_GET_NAMED_SERVICES;

							if ((i + 1) < argc)
								{
									keyword_s = argv [++ i];
								}
							else
								{
									printf ("named service argument missing");
								}
						}
					else if (strcmp (argv [i], "--get-service-data") == 0)
						{
							op = OP_GET_SERVICE_INFO;

							if ((i + 1) < argc)
								{
									keyword_s = argv [++ i];
								}
							else
								{
									printf ("named service argument missing");
								}
						}
					else
						{
							printf ("Unknown argument: \"%s\"", argv [i]);
						}

					++ i;
				}		/* while (i < argc) */



			c = curl_global_init (CURL_GLOBAL_DEFAULT);

			if (c == 0)
				{
					connection_p = AllocateWebServerConnection (hostname_s);
				}
			else
				{
					PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to initialise curl environment: %d", c);
				}


			if (connection_p)
				{
					Client *client_p = GetClient (connection_p);

					if (client_p)
						{
							SchemaVersion *sv_p = AllocateSchemaVersion (CURRENT_SCHEMA_VERSION_MAJOR, CURRENT_SCHEMA_VERSION_MINOR);
							UserDetails *user_p = NULL;

							SetClientSchema (client_p, sv_p);

							switch (op)
								{
									case OP_LIST_ALL_SERVICES:
										{
											GetAllServicesInClient (client_p, user_p);
										}
										break;

									case OP_LIST_INTERESTED_SERVICES:
										{
											GetInterestedServicesInClient (client_p, protocol_s, query_s, user_p);
										}
										break;

									case OP_RUN_KEYWORD_SERVICES:
										{
											json_t *req_p = GetKeywordServicesRequest (user_p, keyword_s, sv_p);

											if (req_p)
												{
													json_t *response_p = MakeRemoteJsonCall (req_p, connection_p);

													if (response_p)
														{
															if (DisplayResultsInClient (client_p, response_p))
																{

																}

															json_decref (response_p);
														}		/* if (response_p) */
													else
														{
															PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "MakeRemoteJsonCall failed");
														}

													json_decref (req_p);
												}		/* if (req_p) */
											else
												{
													PrintErrors (STM_LEVEL_SEVERE,__FILE__, __LINE__, "GetKeywordServicesRequest failed for %s", query_s);
												}
										}
										break;

									case OP_GET_NAMED_SERVICES:
										{
											GetNamedServicesInClient (client_p, keyword_s, user_p);
										}
										break;


									case OP_GET_SERVICE_INFO:
										{
											GetNamedServicesIndexingDataInClient (client_p, keyword_s, user_p);
										}
										break;

									default:
										break;
								}		/* switch (api_id) */

							ReleaseClient (client_p);
						}		/* if (client_p) */

					FreeConnection (connection_p);

					if (web_server_flag)
						{
							curl_global_cleanup ();
						}
				}
			else
				{
					printf ("Failed to connect to server %s\n", hostname_s);
				}

			if (query_s)
				{
					FreeCopiedString (query_s);
				}

			if (protocol_s)
				{
					FreeCopiedString (protocol_s);
				}

		}


	return 0;
}

