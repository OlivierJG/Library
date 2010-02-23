#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QtGui/QLineEdit>


class MyLineEdit : public QLineEdit
{
public:
    MyLineEdit(QString placeHolderText, QWidget* parent = 0);

    virtual QString text();
    virtual void setText(QString text);

protected:
    virtual void focusInEvent(QFocusEvent* event );
    virtual void focusOutEvent(QFocusEvent* event );

private:
    void showPlaceholder(bool show);
    
    QString m_placeholderText;
    QFont m_placeholderFont;
    QPalette m_placeholderPalette;

    QFont m_normalFont;
    QPalette m_normalPalette;

    bool m_showingPlaceholder;
};

#endif // MYLINEEDIT_H
