#ifndef INPUTEDIT_H
#define INPUTEDIT_H

#include <QDialog>

namespace Ui {
    class InputEdit;
}

class InputEdit : public QDialog {
    Q_OBJECT
public:
    InputEdit(QWidget *parent = 0);
    ~InputEdit();

    void SetTextLine(QString str);
    QString GetTextLine();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::InputEdit *ui;
};

#endif // INPUTEDIT_H
