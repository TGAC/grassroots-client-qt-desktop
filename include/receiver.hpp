#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include "auth.hpp"

#include <QObject>

 class Receiver : public QObject
{
	Q_OBJECT

public:
	Receiver (Auth *globus_p);
	virtual ~Receiver ();

signals:
	void GotAccessToken (const char *access_token_s);


public slots:
	void ReceiveToken (const QString &token_r);
	void ReceiveAllTokens (const QVariantMap &tokens_r);

private:
	char *re_token_s;
};


#endif // RECEIVER_HPP
