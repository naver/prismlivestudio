#pragma once

#include <obs.hpp>
#include <vector>

#include "dialog-view.hpp"

class PLSAdvAudioCtrl;
class QGridLayout;

// "Basic advanced audio"?  ...

class PLSBasicAdvAudio : public PLSDialogView {
	Q_OBJECT

private:
	QWidget *controlArea;
	QGridLayout *mainLayout;
	OBSSignal sourceAddedSignal;
	OBSSignal sourceRemovedSignal;

	std::vector<PLSAdvAudioCtrl *> controls;

	inline void AddAudioSource(obs_source_t *source);

	static bool EnumSources(void *param, obs_source_t *source);

	static void OBSSourceAdded(void *param, calldata_t *calldata);
	static void OBSSourceRemoved(void *param, calldata_t *calldata);

public slots:
	void SourceAdded(OBSSource source);
	void SourceRemoved(OBSSource source);

public:
	explicit PLSBasicAdvAudio(QWidget *parent, PLSDpiHelper dpiHelper = PLSDpiHelper());
	~PLSBasicAdvAudio();

protected:
	bool eventFilter(QObject *watched, QEvent *e) override;
};
