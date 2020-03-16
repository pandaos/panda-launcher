#ifndef SEARCHEDIT_H
#define SEARCHEDIT_H

#include <QLineEdit>
#include <QLabel>

class SearchEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit SearchEdit(QWidget *parent = nullptr);

    void normalMode();
    void editMode();

protected:
    void resizeEvent(QResizeEvent *e) override;

private:
    void moveFloatWidget();
    void onTextChanged();

private:
    QLabel *m_searchIcon;
    QLabel *m_placeholderText;
    QWidget *m_floatWidget;
};

#endif // SEARCHEDIT_H
