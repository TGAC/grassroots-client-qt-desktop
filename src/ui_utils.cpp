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
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>

#include "ui_utils.h"
#include "curl_tools.h"

void UIUtils :: CentreWidget (QWidget *parent_p, QWidget *child_p)
{
	const QRect parent_frame = parent_p -> frameGeometry ();
	const QRect child_frame = child_p -> frameGeometry ();

	/*
	 * Start be getting the iniital top left corner of our child widget
	 * centred over the parent widget.
	 */
	int x = parent_p -> x () + ((parent_frame.width () - child_frame.width ()) >> 1);
	int y = parent_p -> y () + ((parent_frame.height () - child_frame.height ()) >> 1);
	int i;

	/* Get our current screen dimensions */
    QScreen *screen_p = QGuiApplication :: primaryScreen ();
    QRect screen_frame = screen_p -> availableGeometry ();

	/*
	 * If the child widget would partially lie outside of the screen, adjust x and y until it isn't
	 */
	i = screen_frame.left ();
	if (x < i)
		{
			x = i;
		}
	else
		{
			i = screen_frame.right () - child_frame.width ();

			if (x > i)
				{
					x = i;
				}
		}

	i = screen_frame.top ();
	if (y < i)
		{
			y = i;
		}
	else
		{
			i = screen_frame.bottom () - child_frame.height ();

			if (y > i)
				{
					y = i;
				}
		}

	child_p -> move (x, y);
}


QIcon *UIUtils :: GetRemoteIconImage (const char * const icon_path_s)
{
	QIcon *icon_p = 0;
	CurlTool *curl_tool_p = AllocateMemoryCurlTool (0);

	if (curl_tool_p)
		{
			if (SetUriForCurlTool (curl_tool_p, icon_path_s))
				{
					CURLcode res = RunCurlTool (curl_tool_p);

					if (res == CURLE_OK)
						{
							size_t length = GetCurlToolDataSize (curl_tool_p);
							const uchar *data_p = reinterpret_cast <const uchar *> (GetCurlToolData (curl_tool_p));

							QPixmap pix;

							if (pix.loadFromData (data_p, length))
								{
									icon_p = new QIcon (pix);
								}
						}
				}

			FreeCurlTool (curl_tool_p);
		}

	return icon_p;
}
