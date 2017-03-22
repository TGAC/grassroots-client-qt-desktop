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


/*********************************/
/******* STATIC PROTOTYPES *******/
/*********************************/


/*************************************/
/******* FUNCTION DEFINITIONS  *******/
/*************************************/

int main (int argc, char *argv [])
{
  const char *hostname_s = "localhost";
  const char *port_s = NULL;
  const char *username_s = NULL;
  const char *query_s = NULL;
  const char *client_s = "grassroots-qt-client";
  const char *protocol_s = NULL;
  bool web_server_flag = true;
  Operation op = OP_LIST_ALL_SERVICES;
  int i;
  Connection *connection_p = NULL;

  if (argc < 3)
    {
      printf ("USAGE: client <username> <password>\n");
      return 0;
    }
  else
    {
      i = 1;

      while (i < argc)
        {
          char error_arg = 0;

          if (*argv [i] == '-')
            {
              switch (* (argv [i] + 1))
              {
                case 'u':
                  if (++ i < argc)
                    {
                      username_s = argv [i];
                    }
                  else
                    {
                      error_arg = * (argv [i] - 1);
                    }
                  break;

                case 's':
                  if (++ i < argc)
                    {
                      port_s = argv [i];
                    }
                  else
                    {
                      error_arg = * (argv [i] - 1);
                    }
                  break;

                case 'h':
                  if (++ i < argc)
                    {
                      hostname_s = argv [i];
                    }
                  else
                    {
                      error_arg = * (argv [i] - 1);
                    }
                  break;

                case 'a':
                  if (++ i < argc)
                    {
                      int id = atoi (argv [i]);

                      if ((id >= 0) && (id < OP_NUM_OPERATIONS))
                        {
                          op = (Operation) id;
                        }
                      else
                        {
                          error_arg = * (argv [i] - 1);
                        }
                    }
                  else
                    {
                      error_arg = * (argv [i] - 1);
                    }
                  break;

                case 'q':
                  if (++ i < argc)
                    {
                      query_s = argv [i];
                    }
                  else
                    {
                      error_arg = * (argv [i] - 1);
                    }
                  break;

                case 'P':
                  if (++ i < argc)
                    {
                      protocol_s = argv [i];
                    }
                  else
                    {
                      error_arg = * (argv [i] - 1);
                    }
                  break;

                /* raw socket connection */
                case 'r':
                  web_server_flag = false;
                  break;

                default:
                  break;
              }

              if (error_arg)
                {
                  PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Error arg \"%c\"", error_arg);
                }
            }

          ++ i;
        }		/* while (i < argc) */

      if (web_server_flag)
        {
          CURLcode c = curl_global_init (CURL_GLOBAL_DEFAULT);

          if (c == 0)
            {
              connection_p = AllocateWebServerConnection (hostname_s);
            }
          else
            {
              PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to init curl: %d", c);
            }
        }
      else
        {
          connection_p = AllocateRawServerConnection (hostname_s, port_s);
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
                      if (query_s)
                        {
                          json_t *req_p = GetKeywordServicesRequest (user_p, query_s, sv_p);

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

                        }		/* if (query_s) */
                    }
                    break;

                  case OP_GET_NAMED_SERVICES:
                    {
                      GetNamedServicesInClient (client_p, query_s, user_p);
                    }
                    break;

                  case OP_CHECK_SERVICE_STATUS:
                    {
                      json_t *req_p = GetCheckServicesRequest (user_p, query_s, sv_p);

                      if (req_p)
                        {
                          json_t *response_p = MakeRemoteJsonCall (req_p, connection_p);

                          if (response_p)
                            {
                              char *dump_s = json_dumps (response_p, JSON_INDENT (2));

                              if (dump_s)
                                {
                                  PrintLog (STM_LEVEL_INFO, __FILE__, __LINE__, "%s", dump_s);
                                  free (dump_s);
                                }

                              json_decref (response_p);
                            }		/* if (response_p) */

                          json_decref (req_p);
                        }		/* if (req_p) */
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
          printf ("failed to connect to server %s:%s\n", hostname_s, port_s);
        }
    }

  return 0;
}
