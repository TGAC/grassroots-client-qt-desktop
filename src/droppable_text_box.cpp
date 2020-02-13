#include <QMimeData>

#include "string_utils.h"

#include "droppable_text_box.h"

#include "byte_buffer.h"



DroppableTextBox :: DroppableTextBox (QTParameterWidget *parent_p)
: QPlainTextEdit (parent_p)
{
	setAcceptDrops (true);

	/* Set tab width to 2 spaces */
	QFontMetrics metrics (font ());
	int space_width = metrics.width (' ');
	setTabStopWidth (2 * space_width);
}


DroppableTextBox ::	~DroppableTextBox ()
{}


void DroppableTextBox :: dropEvent (QDropEvent *event_p)
{
	QList <QUrl> urls = event_p -> mimeData () -> urls ();

	if (! (urls.isEmpty ()))
		{
			QString filename = urls.first ().toLocalFile ();

			if (! ((filename.isEmpty ()) || (filename.isNull ())))
				{
					QByteArray ba = filename.toLocal8Bit ();
					const char * const filename_s = ba.constData ();

					qDebug () << "dropped " << filename;

					LoadText (filename_s);
				}		/* if (! (filename.isEmpty ())) */

		}		/* if (! (urls.isEmpty ())) */

}

void DroppableTextBox :: dragEnterEvent (QDragEnterEvent *event_p)
{
	event_p -> acceptProposedAction ();
}


void DroppableTextBox :: LoadText (const char *filename_s)
{
	ByteBuffer *buffer_p = AllocateByteBuffer (1024);

	if (buffer_p)
		{
			FILE *in_f = fopen (filename_s, "r");

			if (in_f)
				{
					bool loop_flag = true;
					bool success_flag = true;
					char *buffer_s = NULL;

					while (loop_flag)
						{
							if (GetLineFromFile (in_f, &buffer_s))
								{
									if (!AppendStringsToByteBuffer (buffer_p, buffer_s, "\n", NULL))
										{
											success_flag = false;
										}

									FreeLineBuffer (buffer_s);

								}
							else
							{
								loop_flag = false;
							}
						}

					if (success_flag)
						{
							const char *data_s = GetByteBufferData (buffer_p);

							if (!SetFromText (data_s))
								{
									PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to set text to %s", data_s);
								}
						}

					fclose (in_f);
				}

			FreeByteBuffer (buffer_p);
		}
}


bool DroppableTextBox :: SetFromText (const char * const data_s)
{
	clear ();
	insertPlainText (data_s);

	return true;
}
