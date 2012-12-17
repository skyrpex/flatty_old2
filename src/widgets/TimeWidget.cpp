#include "TimeWidget.h"
#include "ui_TimeWidget.h"
#include "model/JointModel.h"
#include "model/AnimModel.h"
#include "widgets/time/JointProxyModel.h"
#include "widgets/time/JointDelegate.h"
#include "widgets/time/JointHeaderView.h"
#include "model/Anim.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QSplitter>
#include <QScrollbar>
#include <QToolBar>
#include <QDebug>
#include <QSpinBox>

TimeWidget::TimeWidget(JointModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeWidget),
    m_model(model),
    m_delegate(new JointDelegate(this)),
    m_leftProxy(new JointProxyModel(model, this)),
    m_rightProxy(new JointProxyModel(model, this))
{
    ui->setupUi(this);

    QTreeView *namesView = ui->namesView;
    QTreeView *timeLineView = ui->timeLineView;

    // Sync views
    connect(namesView->verticalScrollBar(), SIGNAL(valueChanged(int)), timeLineView->verticalScrollBar(), SLOT(setValue(int)));
    connect(timeLineView->verticalScrollBar(), SIGNAL(valueChanged(int)), namesView->verticalScrollBar(), SLOT(setValue(int)));
    connect(namesView, SIGNAL(expanded(QModelIndex)), SLOT(onExpanded(QModelIndex)));
    connect(namesView, SIGNAL(collapsed(QModelIndex)), SLOT(onCollapsed(QModelIndex)));

    // Configure names view
    m_leftProxy->showAnims(false);

    namesView->setModel(m_leftProxy);
    namesView->setItemDelegate(m_delegate);
    namesView->setHeader(new JointHeaderView(false));
    namesView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    namesView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // Configure time line view
    JointHeaderView *header = new JointHeaderView(true);

    m_rightProxy->showVisibleColumn(false);
    m_rightProxy->showAnim(0);

    timeLineView->setModel(m_rightProxy);
    timeLineView->setItemDelegate(m_delegate);
    timeLineView->setHeader(header);
    timeLineView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    timeLineView->hideColumn(JointModel::NameColumn);
    timeLineView->setAutoScroll(false);
    timeLineView->setMouseTracking(true);
    timeLineView->setItemsExpandable(false);

    connect(timeLineView, SIGNAL(entered(QModelIndex)), SLOT(openEditor(QModelIndex)));
    connect(model->animModel(), SIGNAL(rowsInserted(QModelIndex, int, int)), SLOT(resetEditor()));
    connect(model->animModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(resetEditor()));
    connect(model->animModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(resetEditor()));

    connect(m_delegate, SIGNAL(currentFrameChanged(int)), header, SLOT(setCurrentFrame(int)));
    connect(header, SIGNAL(currentFrameChanged(int)), m_delegate, SLOT(setCurrentFrame(int)));
    connect(header, SIGNAL(currentFrameChanged(int)), timeLineView->viewport(), SLOT(update()));
    connect(header, SIGNAL(currentFrameChanged(int)), SIGNAL(currentFrameChanged(int)));

    // Initialize
    setCurrentAnim(-1);
}

void TimeWidget::setCurrentAnim(int i)
{
    // Enable / disable widgets
    bool enable = (i != -1);
    ui->namesView->setEnabled(enable);
    ui->timeLineView->setEnabled(enable);
    ui->frameCount->setEnabled(enable);
    ui->fps->setEnabled(enable);

    m_rightProxy->showAnim(i);

    if(i != -1)
    {
        Anim *anim = m_model->animModel()->anims().at(i);
        qDebug() << anim->name();
    }
}

void TimeWidget::openEditor(const QModelIndex &index)
{
    if(m_openEditorIndex.isValid())
    {
        ui->timeLineView->closePersistentEditor(m_openEditorIndex);
        m_openEditorIndex = QModelIndex();
    }

    if(index.column() != JointModel::NameColumn)
    {
        ui->timeLineView->openPersistentEditor(index);
        m_openEditorIndex = index;
    }
}

void TimeWidget::onExpanded(const QModelIndex &proxyIndex)
{
    QModelIndex sourceIndex = m_leftProxy->mapToSource(proxyIndex);
    ui->timeLineView->expand(m_rightProxy->mapFromSource(sourceIndex));
}

void TimeWidget::onCollapsed(const QModelIndex &proxyIndex)
{
    QModelIndex sourceIndex = m_leftProxy->mapToSource(proxyIndex);
    ui->timeLineView->collapse(m_rightProxy->mapFromSource(sourceIndex));
}

void TimeWidget::resetEditor()
{
    openEditor(ui->timeLineView->currentIndex());
}
