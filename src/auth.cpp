

#include "auth.hpp"


quint16 Auth :: AU_DEFAULT_PORT;


Auth :: Auth (QObject *parent_p, const char * const client_id_s, const char * const client_secret_s, const quint16 reply_port)
	{

	}

Auth :: ~Auth()
	{

	}


bool Auth :: IsAccessTokenSet () const
	{
		bool success_flag = false;

		return success_flag;
	}

const char *Auth :: GetAccessToken () const
	{
		const char *access_token_s = nullptr;

		return access_token_s;
	}

bool Auth :: Init ()
	{
		bool success_flag = false;

		return success_flag;

	}


void Auth :: Authenticate ()
	{

	}

void Auth :: Granted ()
	{

	}

void Auth :: TokensReceived (const QVariantMap &tokens_r)
	{

	}


