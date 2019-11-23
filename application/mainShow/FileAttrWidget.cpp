#include "FileAttrWidget.h"

#include "frame/DocummentFileHelper.h"
#include "AttrScrollWidget.h"
#include <DWindowCloseButton>
#include <QFileInfo>
#include "controller/DataManager.h"
#include <DFontSizeManager>
#include <QMdiArea>
#include <DFrame>

FileAttrWidget::FileAttrWidget(DWidget *parent)
    : DAbstractDialog(parent)
{
    setAttribute(Qt::WA_ShowModal, true); //  模态对话框， 属性设置
    setFixedSize(QSize(400, 642));

    m_pVBoxLayout = new QVBoxLayout;
    m_pVBoxLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(m_pVBoxLayout);

    initWidget();
}

//  各个 对应的 label 赋值
void FileAttrWidget::setFileAttr()
{
    auto dproxy = DocummentFileHelper::instance();
    if (nullptr == dproxy) {
        return;
    }

    QImage image;
    bool rl = dproxy->getImage(0, image, 94, 113);
    if (rl) {
        labelImage->setPixmap(QPixmap::fromImage(image));
    }

    QString filePath = DataManager::instance()->strOnlyFilePath();
    QFileInfo info(filePath);
    QString szTitle = info.fileName();

    labelFileName->setText(szTitle);

    auto hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(10, 35, 10, 10);

    auto sss = new AttrScrollWidget(this);
    auto scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(sss);
    scrollArea->setWidgetResizable(true);
    hLayout->addWidget(scrollArea);

    m_pVBoxLayout->addItem(hLayout);
}

void FileAttrWidget::showScreenCenter()
{
    setFileAttr();

    Dtk::Widget::moveToCenter(this);
    this->show();
}

void FileAttrWidget::initWidget()
{
    initCloseBtn();

    initImageLabel();
}

void FileAttrWidget::initCloseBtn()
{
    auto layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addStretch(1);

    auto closeButton = new DWindowCloseButton(this);
    closeButton->setFixedSize(QSize(50, 50));
    closeButton->setIconSize(QSize(50, 50));
    closeButton->setToolTip(tr("close"));
    connect(closeButton, &DWindowCloseButton::clicked, this, &FileAttrWidget::slotBtnCloseClicked);

    layout->addWidget(closeButton);

    m_pVBoxLayout->addItem(layout);
}

void FileAttrWidget::initImageLabel()
{
    labelImage = new DLabel(this);
    labelImage->setAlignment(Qt::AlignCenter);

    labelFileName = new DLabel("", this);
    labelFileName->setFont(DFontSizeManager::instance()->get(DFontSizeManager::T8));
    labelFileName->setAlignment(Qt::AlignCenter);

    auto vlayout = new QVBoxLayout;
    vlayout->setContentsMargins(0, 6, 0, 30);
    vlayout->setSpacing(10);
    vlayout->addWidget(labelImage);
    vlayout->addWidget(labelFileName);

    m_pVBoxLayout->addItem(vlayout);
}

void FileAttrWidget::slotBtnCloseClicked()
{
    this->close();
}
