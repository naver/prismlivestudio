#include "PLSThemeManager.h"

#include <map>
#include <tuple>

#include <QMetaEnum>
#include <QFile>
#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QDockWidget>

#include "PLSToplevelView.hpp"
#include "log.h"

#define CSTR_THEME "theme"

namespace {
void skipSpace(int &index, const char *css, int count)
{
	for (; index < count; ++index) {
		if (!isspace(css[index])) {
			break;
		}
	}
}

void skipSpace(QByteArray &preprocessed, int &index, const char *css, int count)
{
	int startIndex = index;
	skipSpace(index, css, count);
	if ((startIndex != index) && !preprocessed.endsWith(' ')) {
		preprocessed.append(' ');
	}
}

void skipSpaceReverse(int &index, const char *css, int end)
{
	for (; index > end; --index) {
		if (!isspace(css[index])) {
			break;
		}
	}
}

bool isCommentStartTag(int &index, const char *css, int count)
{
	if ((index + 2) >= count) {
		return false;
	} else if ((css[index] != '/') || (css[index + 1] != '*')) {
		return false;
	}

	index += 2;
	return true;
}

bool isCommentEndTag(int &index, const char *css, int count)
{
	if ((index + 2) >= count) {
		return false;
	} else if ((css[index] != '*') || (css[index + 1] != '/')) {
		return false;
	}

	index += 2;
	return true;
}

void processComment(int &index, const char *css, int count)
{
	while (!isCommentEndTag(index, css, count)) {
		++index;
	}
}

bool isHdpiTag(int &index, const char *css, int count)
{
	// /*hdpi*/
	skipSpace(index, css, count);

	int start = index;
	while (!isCommentEndTag(index, css, count)) {
		++index;
	}

	int end = index - 2;
	if (end <= start) {
		return false;
	}

	skipSpaceReverse(end, css, start);
	if ((end - start) != 4) {
		return false;
	} else if (css[start] != 'h' || css[start + 1] != 'd' || css[start + 2] != 'p' || css[start + 3] != 'i') {
		return false;
	}
	return true;
}

void processHdpi(QByteArray &preprocessed, int &index, const char *css, int count, double dpi)
{
	skipSpace(index, css, count);

	QByteArray num;
	for (; index < count; ++index) {
		if (isdigit(css[index]) || (css[index] == '-') || (css[index] == '.')) {
			num.append(css[index]);
		} else {
			break;
		}
	}

	if (!num.isEmpty()) {
		// support float
		int val = PLSDpiHelper::calculate(dpi * num.toDouble(), 1);
		preprocessed.append(QByteArray::number(val));
	}
}

bool isValueStartTag(QByteArray &preprocessed, int &index, const char *css, int count)
{
	if ((index + 1) >= count) {
		return false;
	} else if (css[index] != ':') {
		return false;
	}

	preprocessed.append(css[index++]);
	return true;
}

bool isValueEndTag(QByteArray &preprocessed, int &index, const char *css, int count)
{
	if ((index + 1) >= count) {
		return false;
	} else if (css[index] != ';') {
		return false;
	}

	preprocessed.append(css[index++]);
	return true;
}

void processValue(QByteArray &preprocessed, int &index, const char *css, int count)
{
	const auto ch = css[index++];
	if (isspace(ch)) {
		preprocessed.append(' ');
		skipSpace(index, css, count);
	} else {
		preprocessed.append(ch);
	}
}

void processValue(QByteArray &preprocessed, int &index, const char *css, int count, double dpi)
{
	skipSpace(index, css, count);

	while (index < count) {
		if (isValueEndTag(preprocessed, index, css, count)) {
			return;
		} else if (!isCommentStartTag(index, css, count)) {
			processValue(preprocessed, index, css, count);
		} else {
			if (!preprocessed.endsWith(':')) {
				preprocessed.append(' ');
			}
			if (isHdpiTag(index, css, count)) {
				processHdpi(preprocessed, index, css, count, dpi);
			}
		}
	}
}
}

PLSThemeManager::PLSThemeManager() {}
PLSThemeManager ::~PLSThemeManager() {}

PLSThemeManager *PLSThemeManager::instance()
{
	static PLSThemeManager themeManager;
	return &themeManager;
}

QString PLSThemeManager::loadCss(double dpi, const QList<CssIndex> &cssIndexes)
{
	QString css;
	for (int i = 0; i < cssIndexes.count(); ++i) {
		css.append(loadCss(dpi, cssIndexes[i]));
	}
	return css;
}

QString PLSThemeManager::loadCss(double dpi, CssIndex cssIndex)
{
	const char *cssName = QMetaEnum::fromType<CssIndex>().valueToKey(cssIndex);
	PLS_DEBUG(CSTR_THEME, "load css, name: %s", cssName);

	QString css;
	if (findCachedCss(css, dpi, cssIndex)) {
		return css;
	}

	char cssPath[256];
	sprintf_s(cssPath, ":/css/%s.css", cssName);

	QFile file(cssPath);
	if (!file.open(QFile::ReadOnly)) {
		PLS_ERROR(CSTR_THEME, "can't load css, name: %s", cssName);
		return QString();
	}

	css = preprocessCss(dpi, file.readAll());
	addCachedCss(dpi, cssIndex, css);
	return css;
}

QString PLSThemeManager::preprocessCss(double dpi, const QByteArray &css)
{
	if (css.isEmpty()) {
		return QString();
	}

	QByteArray preprocessed;
	const char *cssc = css.constData();
	for (int index = 0, count = css.size(); index < count;) {
		skipSpace(preprocessed, index, cssc, count);
		if (isValueStartTag(preprocessed, index, cssc, count)) {
			processValue(preprocessed, index, css, count, dpi);
		} else if (!isCommentStartTag(index, cssc, count)) {
			preprocessed.append(cssc[index++]);
		} else {
			processComment(index, cssc, count);
		}
	}
	return QString::fromUtf8(preprocessed);
}

QString PLSThemeManager::preprocessCss(double dpi, const QString &qss)
{
	return preprocessCss(dpi, qss.toUtf8());
}

void PLSThemeManager::addCachedCss(double dpi, CssIndex cssIndex, const QString &css)
{
	QWriteLocker writeLocker(&cssCacheLock);
	cssCache.insert({cssIndex, int(dpi * 96)}, css);
}

bool PLSThemeManager::findCachedCss(QString &css, double dpi, CssIndex cssIndex) const
{
	QReadLocker readLocker(&cssCacheLock);
	if (auto iter = cssCache.constFind({cssIndex, int(dpi * 96)}); iter != cssCache.end()) {
		css = iter.value();
		return true;
	}
	return false;
}

bool PLSThemeManager::eventFilter(QObject *watched, QEvent *event)
{
	switch (event->type()) {
	case QEvent::ParentChange:
		if (QWidget *widget = dynamic_cast<QWidget *>(watched); widget) {
			emit parentChange(widget);
		}
		break;
	}

	return false;
}
