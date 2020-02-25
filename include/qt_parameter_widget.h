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

/**
 * @file
 * @brief
 */
#ifndef QT_PARAMETER_WIDGET_H
#define QT_PARAMETER_WIDGET_H

#include <QFormLayout>
#include <QGridLayout>
#include <QHash>
#include <QWidget>
#include <QCheckBox>
#include <QWebEngineView>
#include <QGroupBox>
#include <QList>
#include <QVBoxLayout>

#include "parameter.h"
#include "parameter_set.h"
#include "service_metadata.h"
#include "base_param_widget.h"
#include "param_group_box.h"
#include "parameter_widget_container.h"


// forward class declaration
class ServicePrefsWidget;
struct QTClientData;


class QTParameterWidget : public QWidget
{
	Q_OBJECT


public:
	QTParameterWidget (const char *name_s, const char * const description_s, const char * const uri_s, const json_t *provider_p, ParameterSet *parameters_p, ServiceMetadata *metadata_p, const ServicePrefsWidget * const prefs_widget_p, const ParameterLevel initial_level, const struct QTClientData *client_data_p);

	virtual ~QTParameterWidget ();

	virtual void UpdateParameterLevel (const ParameterLevel level, const QWidget * const parent_widget_p);


	void AddParameters (ParameterSet *params_p);

	json_t *GetParameterValuesAsJSON () const;


	ParameterSet *GetParameterSet (bool refresh_flag) const;

	BaseParamWidget *GetWidgetForParameter (const char * const param_name_s) const;


	/**
	 * @brief CreateWidgetForParameter
	 *
	 * Create the appropriate widget for a given parameter.
	 *
	 * @param param_p The Parameter to create the widget for.
	 * @param add_param_flag If <code>true</code> then the Parameter will also be added to the underlying ParameteSet.
	 * This is for when you have repeatable parameters.
	 * @return The widget or 0 upon error.
	 */
	BaseParamWidget *CreateWidgetForParameter (Parameter * const param_p, bool add_param_flag);

	void ParameterWidgetAdded (Parameter *param_p, BaseParamWidget *widget_p);

	bool SetParamValuesFromJSON (const json_t *param_set_json_p);



public slots:
	void ResetToDefaults ();

	void RefreshService ();

private slots:
	void OpenLink (const QString &link_r);


private:
	ParameterSet *qpw_params_p;

	const ServicePrefsWidget * const qpw_parent_prefs_widget_p;

	QHash <Parameter *, BaseParamWidget *> qpw_widgets_map;

	//QGridLayout *qpw_layout_p;
	QFormLayout *qpw_layout_p;

	ParameterLevel qpw_level;

	QList <QWebEngineView *> qpw_browsers;

	QHash <const char *, ParameterWidgetContainer *> qpw_groupings;

	const struct QTClientData *qpw_client_data_p;

	bool qpw_refresh_active;


	/**
	 * Get pre-existing widget for parameter.
	 */
	BaseParamWidget *GetWidgetForParameter (const Parameter * const param_p);

	void AddRow (QWidget *first_p, QWidget *second_p, const int row_span);
	void AddParameterWidget (Parameter *param_p,  ParameterWidgetContainer *container_p, bool add_params_flag);

	void AddProvider (const json_t *provider_p, const size_t i, const size_t last_index, QVBoxLayout *info_layout_p);

	void AddServiceMetadata (const ServiceMetadata *metadata_p, QBoxLayout *layout_p);

	bool AddListForSchemaTerms (const LinkedList *terms_p, const char * const key_s, QFormLayout *layout_p);

	bool AddLabelForSchemaTerm (const SchemaTerm *term_p, const char * const key_s, QFormLayout *layout_p);

	static const int QPW_NUM_COLUMNS;
};


#endif		/* #ifndef QT_PARAMETER_WIDGET_H */
