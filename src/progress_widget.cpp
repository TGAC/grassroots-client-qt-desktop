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
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "progress_widget.h"
#include "progress_window.h"
#include "json_util.h"
#include "json_tools.h"
#include "memory_allocations.h"

#include "json_viewer.h"
#include "alloc_failure.hpp"
#include "streams.h"


ProgressWidget *ProgressWidget :: CreateProgressWidgetFromJSON (const json_t *json_p, ProgressWindow *parent_p, const char * const service_name_s, const char * const service_description_s, const char * const service_uri_s)
{
	ProgressWidget *widget_p = NULL;
	try
		{
			widget_p = new ProgressWidget (json_p, service_name_s, service_description_s, parent_p);
		}
	catch (AllocFailure &f_r)
		{

		}

	return widget_p;
}



ProgressWidget :: ProgressWidget (const json_t *json_p, const char *service_name_s, const char *service_description_s, ProgressWindow *parent_p)
{
	const char *error_s = NULL;
	const char *uuid_s = GetJSONString (json_p, JOB_UUID_S);

	if (uuid_s)
		{
			uuid_t id;

			if (uuid_parse (uuid_s, id) == 0)
				{
					if (GetStatusFromJSON (json_p, &pw_current_status))
						{
							pw_json_p = json_deep_copy (json_p);

							if (pw_json_p)
								{
									const char *name_s = GetJSONString (json_p, JOB_NAME_S);
									const json_t *errors_p = json_object_get (json_p, JOB_ERRORS_S);

									QHBoxLayout *layout_p = new QHBoxLayout;

									uuid_copy (pw_id, id);

									QVBoxLayout *info_layout_p = new QVBoxLayout;

									QString s = "<b>";
									s.append (service_name_s);
									s.append (" - ");
									s.append (name_s);
									s.append ("</b>");

									pw_title_p = new QLabel (s);
									info_layout_p -> addWidget (pw_title_p);

									if (service_description_s)
										{
											pw_description_p = new QLabel (service_description_s);
											info_layout_p -> addWidget (pw_description_p);
										}

									layout_p -> addLayout (info_layout_p);


									pw_progress_label_p = new QLabel;
									pw_anim_p = new QMovie ("images/anim_progress.gif");
									pw_progress_label_p -> setMovie (pw_anim_p);
									layout_p -> addWidget (pw_progress_label_p);

									/*
									pw_progress_p = new QProgressBar;
									pw_progress_p -> setMinimum (0);
									pw_progress_p -> setMaximum (0);
									layout_p -> addWidget (pw_progress_p);
									*/
									pw_status_p = new QLabel;
									layout_p -> addWidget (pw_status_p);


									pw_results_button_p = new QPushButton (tr ("View Results"));
									pw_results_button_p -> setEnabled (false);
									connect (pw_results_button_p, &QPushButton :: clicked, this, &ProgressWidget :: ShowResults);
									layout_p -> addWidget (pw_results_button_p);

									pw_parent_p = parent_p;

									if ((errors_p != 0) && (json_object_size (errors_p) > 0))
										{
											QPushButton *errors_button_p = new QPushButton (tr ("View Errors"));
											errors_button_p -> setEnabled (true);
											connect (errors_button_p, &QPushButton :: clicked, this, &ProgressWidget :: ShowErrors);
											layout_p -> addWidget (errors_button_p);
										}

									SetStatus (pw_current_status);

									setLayout (layout_p);


									QPushButton *delete_btn_p = new QPushButton (QIcon ("images/delete"), tr ("Delete"), this);
									connect (delete_btn_p, &QPushButton :: clicked, this, &ProgressWidget :: DeleteFromParent);
									layout_p -> addWidget (delete_btn_p);

								}		/* if (pw_json_p) */
							else
								{
									error_s = "Failed to copy job data";
									PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, json_p, error_s);
								}

						}		/* if (GetStatusFromJSON (json_p, &status)) */
					else
						{
							error_s = "Failed to get job status";
							PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, json_p, error_s);
						}

				}		/* if (uuid_parse (uuid_s, id) == 0) */
			else
				{
					error_s = "Failed to parse job id";
					PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to parse id from \"%s\"", uuid_s);
				}

		}		/* if (uuid_s) */
	else
		{
			error_s = "Failed to find job id";
			PrintJSONToErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, json_p, error_s);
		}

	if (error_s)
		{
			throw AllocFailure (error_s);
		}
}


ProgressWidget ::	~ProgressWidget ()
{
	delete pw_anim_p;

	json_decref (pw_json_p);
}


const uuid_t *ProgressWidget ::	GetUUID () const
{
	return &pw_id;
}


void ProgressWidget ::	GetServiceResults ()
{

}


void ProgressWidget :: ShowResults (bool checked_flag)
{
	ProgressWidget **widget_pp = reinterpret_cast <ProgressWidget **> (AllocMemory (sizeof (ProgressWidget *)));

	if (widget_pp)
		{
			*widget_pp = this;

			pw_parent_p -> RefreshStatuses (widget_pp, 1);

			FreeMemory (widget_pp);
		}

}


void ProgressWidget :: ShowErrors (bool checked_flag)
{
	json_t *errors_p = json_object_get (pw_json_p, JOB_ERRORS_S);

	if (errors_p && (json_object_size (errors_p) > 0))
		{
			JSONViewer *viewer_p = new JSONViewer ();
			viewer_p -> SetJSONData (errors_p);
			viewer_p -> setVisible (true);
		}
}


void ProgressWidget :: DeleteFromParent (bool checked_flag)
{
	if (!pw_parent_p -> RemoveProgressWidget (this))
		{
			PrintErrors (STM_LEVEL_SEVERE, __FILE__, __LINE__, "Failed to remove progress widget");
		}
}


void ProgressWidget :: SetStatus (OperationStatus status)
{
	const char *text_s = "";
	bool results_flag = false;

	switch (status)
		{
			case OS_FAILED:
			case OS_FAILED_TO_START:
			case OS_ERROR:
				text_s = "Failed";
				pw_anim_p -> stop ();
				break;

			case OS_IDLE:
				text_s = "Idle";
				break;

			case OS_PENDING:
				text_s = "Waiting to start";
				break;

			case OS_STARTED:
				text_s = "Started";
				pw_anim_p -> start ();
				break;

			case OS_FINISHED:
				text_s = "Finished";
				pw_anim_p -> stop ();
				results_flag = true;
				break;

			case OS_SUCCEEDED:
				text_s = "Succeeded";
				pw_anim_p -> stop ();
				results_flag = true;
				break;

			default:
				break;
		}

	pw_results_button_p -> setEnabled (results_flag);

	pw_status_p -> setText (text_s);
	pw_current_status = status;
}


OperationStatus ProgressWidget :: GetCurrentStatus () const
{
	return pw_current_status;
}

