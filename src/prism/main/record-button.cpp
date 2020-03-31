#include "record-button.hpp"
#include "window-basic-main.hpp"

void RecordButton::resizeEvent(QResizeEvent *event)
{
	PLSBasic *main = PLSBasic::Get();
	if (!main->pause)
		return;

	QSize pauseSize = main->pause->size();
	//int height = main->ui->recordButton->size().height();
	int height = 25;
	if (pauseSize.height() != height || pauseSize.width() != height) {
		main->pause->setMinimumSize(height, height);
		main->pause->setMaximumSize(height, height);
	}

	event->accept();
}
