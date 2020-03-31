#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include "window-basic-adv-audio.hpp"
#include "window-basic-main.hpp"
#include "item-widget-helpers.hpp"
#include "adv-audio-control.hpp"
#include "pls-app.hpp"
#include "qt-wrappers.hpp"

Q_DECLARE_METATYPE(OBSSource);

PLSBasicAdvAudio::PLSBasicAdvAudio(QWidget *parent, PLSDpiHelper dpiHelper)
	: PLSDialogView(parent, dpiHelper),
	  sourceAddedSignal(obs_get_signal_handler(), "source_activate", OBSSourceAdded, this),
	  sourceRemovedSignal(obs_get_signal_handler(), "source_deactivate", OBSSourceRemoved, this)
{
	dpiHelper.setCss(this, {PLSCssIndex::PLSBasicAdvAudio});

	QScrollArea *scrollArea;
	QVBoxLayout *vlayout;
	QWidget *widget;
	QLabel *label;
	QSpacerItem *horizontalSpacer;

	mainLayout = new QGridLayout;
	mainLayout->setHorizontalSpacing(0);
	mainLayout->setVerticalSpacing(10);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	//init the title view layout
	QHBoxLayout *hlayout = new QHBoxLayout;
	hlayout->setContentsMargins(0, 0, 0, 0);
	hlayout->setSpacing(0);

	//init the name title label
	int idx = 0;
	QString styleSheet = QString("font-weight:bold; font-size: /*hdpi*/ 14px; min-height: /*hdpi*/ 32px; max-height: /*hdpi*/ 32px; ");
	label = new QLabel(QTStr("Basic.AdvAudio.Name"));
	dpiHelper.setStyleSheet(label, styleSheet);
	label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	dpiHelper.setMinimumWidth(label, NAME_LABEL_WIDTH);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	hlayout->addWidget(label);

	//add horizon space item
	horizontalSpacer = new QSpacerItem(MONITOR_LABEL_SPACE + MONITOR_LABEL_LEFT_SPACE, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
	hlayout->addSpacerItem(horizontalSpacer);

	//init the audio monitor label
#if defined(_WIN32) || defined(__APPLE__) || HAVE_PULSEAUDIO

	//add monitor type title
	label = new QLabel(QTStr("Basic.AdvAudio.Monitoring"));
	dpiHelper.setStyleSheet(label, styleSheet);
	label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	dpiHelper.setFixedWidth(label, MONITOR_TYPE_WIDTH - MONITOR_LABEL_LEFT_SPACE);
	hlayout->addWidget(label);

	//add horizon space item
	horizontalSpacer = new QSpacerItem(VOLUME_SPINBOX_SPACE, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
	hlayout->addSpacerItem(horizontalSpacer);
#endif

	// add volumn label
	label = new QLabel(QTStr("Basic.AdvAudio.Volume"));
	dpiHelper.setStyleSheet(label, styleSheet);
	label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	dpiHelper.setMinimumWidth(label, VOLUME_SPINBOX_WIDTH);
	hlayout->addWidget(label);

	//add horizon space it em
	horizontalSpacer = new QSpacerItem(BALANCE_SPACE, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
	hlayout->addSpacerItem(horizontalSpacer);

	// add balance label
	label = new QLabel(QTStr("Basic.AdvAudio.Balance"));
	dpiHelper.setStyleSheet(label, styleSheet);
	label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	dpiHelper.setFixedWidth(label, BALANCE_WIDTH);
	hlayout->addWidget(label);

	//add horizon space item
	horizontalSpacer = new QSpacerItem(MONO_LEFT_SPACE, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
	hlayout->addSpacerItem(horizontalSpacer);

	// add mono label
	label = new QLabel(QTStr("Basic.AdvAudio.Mono"));
	label->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	dpiHelper.setStyleSheet(label, styleSheet);
	dpiHelper.setFixedWidth(label, MONO_TITLE_WIDTH);
	hlayout->addWidget(label);

	//add horizon space item
	horizontalSpacer = new QSpacerItem(SYNC_OFFSET_SPACE, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
	hlayout->addSpacerItem(horizontalSpacer);

	// add sync offset label
	label = new QLabel(QTStr("Basic.AdvAudio.SyncOffset"));
	dpiHelper.setStyleSheet(label, styleSheet);
	dpiHelper.setMinimumWidth(label, VOLUME_SPINBOX_WIDTH);
	label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	hlayout->addWidget(label);

	//add horizon space item
	horizontalSpacer = new QSpacerItem(TRACKS_SPACE, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
	hlayout->addSpacerItem(horizontalSpacer);

	// add audio tracks label
	label = new QLabel(QTStr("Basic.AdvAudio.AudioTracks"));
	dpiHelper.setStyleSheet(label, styleSheet);
	label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	dpiHelper.setMinimumWidth(label, TRACKS_MIN_CONTAINER_WIDTH);
	dpiHelper.setMaximumWidth(label, TRACKS_MAX_CONTAINER_WIDTH);
	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	hlayout->addWidget(label);

	controlArea = new QWidget;
	controlArea->setLayout(mainLayout);
	controlArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

	//init the vertical layout
	vlayout = new QVBoxLayout;
	vlayout->setContentsMargins(0, 0, 0, 0);
	vlayout->addWidget(controlArea);

	widget = new QWidget;
	widget->setLayout(vlayout);
	widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

	scrollArea = new QScrollArea;
	scrollArea->setObjectName("advScrollArea");
	scrollArea->setWidget(widget);
	scrollArea->setWidgetResizable(true);
	scrollArea->setFrameShape(QFrame::NoFrame);

	vlayout = new QVBoxLayout;
	vlayout->setContentsMargins(25, 30, 3, 30);
	vlayout->addLayout(hlayout);
	vlayout->addWidget(scrollArea);
	this->content()->setLayout(vlayout);

	installEventFilter(CreateShortcutFilter());

	/* enum user scene/sources */
	obs_enum_sources(EnumSources, this);

	dpiHelper.setMinimumSize(this, {1102, 280});
	dpiHelper.setInitSize(this, {1202, 480});
	setHasMaxResButton(true);
	setWindowTitle(QTStr("Basic.AdvAudio"));
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowModality(Qt::NonModal);
	setAttribute(Qt::WA_DeleteOnClose, true);

	//install namelabel event filter
	this->installEventFilter(this);
}

PLSBasicAdvAudio::~PLSBasicAdvAudio()
{
	PLSBasic *main = reinterpret_cast<PLSBasic *>(parent());
	for (size_t i = 0; i < controls.size(); ++i)
		delete controls[i];
	main->SaveProject();
}

bool PLSBasicAdvAudio::eventFilter(QObject *watched, QEvent *e)
{
	if (e->type() == QEvent::UpdateRequest) {
		if (watched == this) {
			for (auto control : controls) {
				control->setSourceName();
			}
		}
	}
	return PLSDialogView::eventFilter(watched, e);
}

bool PLSBasicAdvAudio::EnumSources(void *param, obs_source_t *source)
{
	PLSBasicAdvAudio *dialog = reinterpret_cast<PLSBasicAdvAudio *>(param);
	uint32_t flags = obs_source_get_output_flags(source);
	if ((flags & OBS_SOURCE_AUDIO) != 0 && obs_source_audio_active(source) && obs_source_active(source)) {
		dialog->AddAudioSource(source);
	}
	return true;
}

void PLSBasicAdvAudio::OBSSourceAdded(void *param, calldata_t *calldata)
{
	OBSSource source((obs_source_t *)calldata_ptr(calldata, "source"));
	QMetaObject::invokeMethod(reinterpret_cast<PLSBasicAdvAudio *>(param), "SourceAdded", Q_ARG(OBSSource, source));
}

void PLSBasicAdvAudio::OBSSourceRemoved(void *param, calldata_t *calldata)
{
	OBSSource source((obs_source_t *)calldata_ptr(calldata, "source"));
	QMetaObject::invokeMethod(reinterpret_cast<PLSBasicAdvAudio *>(param), "SourceRemoved", Q_ARG(OBSSource, source));
}

inline void PLSBasicAdvAudio::AddAudioSource(obs_source_t *source)
{
	QString strName = obs_source_get_name(source);
	assert(strName.length() > 0);
	for (auto control : controls) {
		if (control->objectName() == strName)
			return;
	}

	PLSAdvAudioCtrl *control = new PLSAdvAudioCtrl(controlArea, source);
	PLSDpiHelper::dpiDynamicUpdate(controlArea);
	control->setObjectName(strName);
	InsertQObjectByName(controls, control);
	for (auto control : controls) {
		control->ShowAudioControl(mainLayout);
	}
}

void PLSBasicAdvAudio::SourceAdded(OBSSource source)
{
	uint32_t flags = obs_source_get_output_flags(source);

	if ((flags & OBS_SOURCE_AUDIO) == 0)
		return;
	//check the audio source active status
	if (!obs_source_audio_active(source)) {
		return;
	}
	AddAudioSource(source);
}

void PLSBasicAdvAudio::SourceRemoved(OBSSource source)
{
	uint32_t flags = obs_source_get_output_flags(source);

	if ((flags & OBS_SOURCE_AUDIO) == 0)
		return;

	for (size_t i = 0; i < controls.size(); i++) {
		if (controls[i]->GetSource() == source) {
			delete controls[i];
			controls.erase(controls.begin() + i);
			break;
		}
	}
}
