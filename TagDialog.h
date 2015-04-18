#ifndef TAGDIALOG_H
#define TAGDIALOG_H

#include <QDialog>
#include <QMainWindow>
//#include "ui_TagDialog.h"

namespace Ui
{
    class TagDialog;
}

class TagDialog:public QDialog
 {
	Q_OBJECT
	
	public:
	explicit TagDialog(QWidget *parent=0);
	~TagDialog();

	private:
	Ui::TagDialog *ui;
	public slots:
	void RecSignalFromMainWindow();

};

#endif //TAGDIALOG_H