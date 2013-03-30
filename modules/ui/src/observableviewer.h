#ifndef _OBSERVABLE_VIEWER_H_
#define _OBSERVABLE_VIEWER_H_

#include <QTextEdit>
#include <QPushButton>
#include <cadence-embedded/core/oid.h>
#include <QLineEdit>
#include <QListWidget>
#include <QComboBox>
#include "history.h"

class MsgAgent;

class ObservableViewer : public QWidget {
	Q_OBJECT

	public:
	ObservableViewer(HistoryLog *hist, const cadence::core::OID &object, const cadence::core::OID &key);
	ObservableViewer(HistoryLog *hist, const cadence::core::OID &object);
	~ObservableViewer() {};
	

	private:

	QTextEdit *m_def;
	QLineEdit *m_obsname;
	QListWidget *m_keylist;
	QPushButton *m_submit;
	QLineEdit *m_valstring;
	QComboBox *m_deftype;
	cadence::core::OID m_object;
	cadence::core::OID m_key;
	HistoryLog *m_history;
	

	public slots:
	void submit();
	void closeme();
	void typechange(int);
	void textchanged();

};

#endif
 
