#ifndef PLSSCENETRANSITIONSVIEW_H
#define PLSSCENETRANSITIONSVIEW_H

#include <QDialog>
#include <QSpinBox>
#include <QComboBox>
#include <QString>
#include "PLSDpiHelper.h"
#include "frontend-api/dialog-view.hpp"
#include "obs.hpp"

namespace Ui {
class PLSSceneTransitionsView;
}

class PLSBasic;

class PLSSceneTransitionsView : public PLSDialogView {
	Q_OBJECT

public:
	explicit PLSSceneTransitionsView(QWidget *parent = nullptr, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSSceneTransitionsView();

	void InitLoadTransition(obs_data_array_t *transitions, obs_source_t *fadeTransition, const int &transitionDuration, const QString &crrentTransition);
	void InitTransition(obs_source_t *transition);
	obs_source_t *FindTransition(const char *name);
	OBSSource GetCurrentTransition();
	OBSSource GetTransitionByIndex(const int &index);
	obs_data_array_t *SaveTransitions();
	void LoadTransitions(obs_data_array_t *transitions);

	void AddTransition();
	void RenameTransition();
	void ClearTransition();
	void SetTransition(OBSSource transition);
	void AddTransitionsItem(std::vector<OBSSource> &transitions);

	QComboBox *GetTransitionCombobox();
	QSpinBox *GetTransitionDuration();
	int GetTransitionComboBoxCount();
	int GetTransitionDurationValue();
	void SetTransitionDurationValue(const int &value);

public slots:
	void on_transitions_currentIndexChanged(int index);
	void on_transitionAdd_clicked();
	void on_transitionRemove_clicked();
	void on_transitionProps_clicked();
	void on_transitionDuration_valueChanged(int value);
	void OnCurrentTextChanged(const QString &text);

protected:
	void showEvent(QShowEvent *event);

private:
	Ui::PLSSceneTransitionsView *ui;
	PLSBasic *main{};
	QString currentText{};
};

#endif // PLSSCENETRANSITIONSVIEW_H
