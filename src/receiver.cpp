#include "receiver.hpp"

#include <QDebug>

#include "string_utils.h"


Receiver ::	Receiver (GlobusAuth *globus_p)
{
	re_token_s = nullptr;
	connect (globus_p, &GlobusAuth :: GotGrant, this, &Receiver :: ReceiveToken);
	connect (globus_p, &GlobusAuth :: GotAllTokens, this, &Receiver :: ReceiveAllTokens);
}

Receiver :: ~Receiver ()
{
	if (re_token_s)
		{
			FreeCopiedString (re_token_s);
		}
}


const char *Receiver :: GetToken () const
{
	return re_token_s;
}


void Receiver :: ReceiveToken (const QString &token_r)
{
	qDebug () << "token: \"" << token_r << "\"" << Qt :: endl;
}


void Receiver :: ReceiveAllTokens (const QVariantMap &tokens_r)
{
	QMapIterator <QString, QVariant> itr (tokens_r);

	qDebug () << "BEGIN tokens" << Qt :: endl;

	while (itr.hasNext ())
		{
			itr.next ();
			QVariant v = itr.value ();
			QString s = v.toString ();

			qDebug () << itr.key () << ": " << s << " (" << v.typeName () << ")" << Qt :: endl;
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

}
