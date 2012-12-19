#include "TimeWidget.h"
#include "ui_TimeWidget.h"
#include "model/JointModel.h"
#include "model/AnimModel.h"
#include "widgets/time/JointProxyModel.h"
#include "widgets/time/JointDelegate.h"
#include "widgets/time/JointHeaderView.h"
#include "commands/AnimFpsEditCommand.h"
#include "model/Anim.h"
#include "Application.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QSplitter>
#include <QScrollbar>
#include <QToolBar>
#include <QDebug>
#include <QSpinBox>
#include <QInputDialog>
#include <limits>

const char * const OneFrameCountButtonText = "1 frame";
const char * const FrameCountButtonText = "%1 frames";
const char * const FpsButtonText = "%1 fps";

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

    // Configure push buttons
    connect(ui->frameCount, SIGNAL(pressed()), SLOT(showFrameCountDialog()));
    connect(ui->fps, SIGNAL(pressed()), SLOT(showFpsDialog()));

    // Initialize
    setCurrentAnim(-1);
}

void TimeWidget::setCurrentAnim(int i)
{
    bool hasAnim = (i != -1);

    // Update current anim
    m_currentAnim = hasAnim?
                m_model->animModel()->anims().at(i)
              : NULL;

    // Enable / disable widgets
    ui->namesView->setEnabled(hasAnim);
    ui->timeLineView->setEnabled(hasAnim);
    ui->frameCount->setEnabled(hasAnim);
    ui->fps->setEnabled(hasAnim);

    // Show the animation with the proxy
    m_rightProxy->showAnim(i);

    // Update spinboxes
    if(hasAnim)
    {
        updateFrameCountButton();
        updateFpsButton();
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

void TimeWidget::showFrameCountDialog()
{
    QString title = tr("title");
    QString label = tr("label");
    int value = m_currentAnim->frameCount();
    int minValue = 1;
    int maxValue = std::numeric_limits<int>::max();
    int step = 1;
    bool ok;
    int result = QInputDialog::getInt(this, title, label, value, minValue, maxValue, step, &ok);
    if(!ok)
        return;

    m_currentAnim->setFrameCount(result);
    updateFrameCountButton();
}

void TimeWidget::showFpsDialog()
{
    QString title = tr("title");
    QString label = tr("label");
    int value = m_currentAnim->fps();
    int minValue = 1;
    int maxValue = std::numeric_limits<int>::max();
    int step = 1;
    bool ok;
    int result = QInputDialog::getInt(this, title, label, value, minValue, maxValue, step, &ok);
    if(!ok)
        return;

    // Change fps
    AnimFpsEditCommand *command = new AnimFpsEditCommand(m_currentAnim, result);
    qApp->undoStack()->push(command);
    updateFpsButton();
}

void TimeWidget::updateFrameCountButton()
{
    int frameCount = m_currentAnim->frameCount();
    const char * const text = (frameCount == 1)?
                OneFrameCountButtonText
              : FrameCountButtonText;
    ui->frameCount->setText(tr(text).arg(frameCount));
}

void TimeWidget::updateFpsButton()
{
    int fps = m_currentAnim->fps();
    ui->fps->setText(tr(FpsButtonText).arg(fps));
}
