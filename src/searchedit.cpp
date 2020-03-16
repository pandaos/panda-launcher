#include "searchedit.h"
#include "utils.h"
#include <QHBoxLayout>
#include <QTimer>

SearchEdit::SearchEdit(QWidget *parent)
    : QLineEdit(parent),
      m_searchIcon(new QLabel),
      m_placeholderText(new QLabel(tr("Search"))),
      m_floatWidget(new QWidget(this))
{
    m_searchIcon->setPixmap(Utils::renderSVG(":/images/search-dark.svg", QSize(16, 16)));
    m_searchIcon->setFixedSize(16, 16);

    QHBoxLayout *floatLayout = new QHBoxLayout;
    floatLayout->addStretch();
    floatLayout->addWidget(m_searchIcon);
    floatLayout->setAlignment(m_searchIcon, Qt::AlignVCenter);
    floatLayout->addSpacing(5);
    floatLayout->addWidget(m_placeholderText);
    floatLayout->addStretch();
    floatLayout->setMargin(0);
    floatLayout->setSpacing(0);
    m_floatWidget->setLayout(floatLayout);
    m_floatWidget->setFixedHeight(30);

    QFontMetrics fm(m_placeholderText->font());
    m_placeholderText->setFixedWidth(fm.width(m_placeholderText->text()) + 10);
    m_placeholderText->setForegroundRole(QPalette::BrightText);
    m_floatWidget->setFixedWidth(m_searchIcon->width() + m_placeholderText->width() + 5);

//    QHBoxLayout *layout = new QHBoxLayout;
//    layout->addStretch();
//    layout->addWidget(m_floatWidget);
//    setLayout(layout);

    setTextMargins(20, 0, 5, 0);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFocusPolicy(Qt::ClickFocus);
    setFixedHeight(30);
    setFixedWidth(300);

    normalMode();

    connect(this, &SearchEdit::textChanged, this, &SearchEdit::onTextChanged);
}

void SearchEdit::normalMode()
{
    QLineEdit::clear();
    QLineEdit::clearFocus();

    m_placeholderText->show();
    m_floatWidget->move(rect().center() - m_floatWidget->rect().center());
    m_floatWidget->raise();
}

void SearchEdit::editMode()
{
    m_placeholderText->hide();
    m_floatWidget->move(QPoint(5, 0));
}

void SearchEdit::resizeEvent(QResizeEvent *e)
{
    QLineEdit::resizeEvent(e);

    QTimer::singleShot(1, this, &SearchEdit::moveFloatWidget);
}

void SearchEdit::moveFloatWidget()
{
    m_floatWidget->move(rect().center() - m_floatWidget->rect().center());
}

void SearchEdit::onTextChanged()
{
    if (!this->text().isEmpty()) {
        this->setFocus();
    }
}
