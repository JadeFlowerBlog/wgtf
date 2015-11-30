#ifndef QT_STATUS_BAR_HPP
#define QT_STATUS_BAR_HPP

#include "core_ui_framework/i_status_bar.hpp"

class QStatusBar;

class QtStatusBar : public IStatusBar
{
public:
	QtStatusBar(QStatusBar& qStatusBar);

	virtual void showMessage(const char* message, int timeout = 0) override;

	virtual void clearMessage() override;

private:
	QStatusBar & qStatusBar_;
};

#endif//QT_STATUS_BAR_HPP