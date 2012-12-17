#include "AnimsWidget.h"
#include "model/AnimModel.h"
#include "dialogs/AnimDialog.h"
#include "model/Anim.h"
#include <QListView>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QDebug>
#include "commands/CreateAnimCommand.h"
#include "commands/DeleteAnimCommand.h"
#include "commands/EditAnimCommand.h"
#include "Application.h"

AnimsWidget::AnimsWidget(AnimModel *model, QWidget *parent) :
    QWidget(parent),
    m_model(model),
    m_view(new QListView(this))
{
    m_view->setModel(model);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    {
        QToolBar *t = new QToolBar(this);
        m_createAction = t->addAction("Create Anim", this, SLOT(createAnim()));
        m_createAction->setShortcut(QKeySequence("Ctrl+A"));
        m_editAction = t->addAction("Edit Anim", this, SLOT(editAnim()));
        m_editAction->setShortcut(QKeySequence("Ctrl+E"));
        m_removeAction = t->addAction("Remove Anim", this, SLOT(removeAnim()));
        m_removeAction->setShortcut(QKeySequence("Ctrl+R"));
        layout->addWidget(t);
    }
    layout->addWidget(m_view);

    connect(m_view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(onCurrentRowChanged(QModelIndex)));
    connect(m_view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(updateActions()));

    updateActions();
}

void AnimsWidget::onCurrentRowChanged(const QModelIndex &index)
{
    emit currentAnimChanged(index.row());
}

Anim *AnimsWidget::animFromUser()
{
    AnimDialog dialog(this);
    if(!dialog.exec())
        return NULL;

    return new Anim(dialog.name(), dialog.frameCount(), dialog.fps());
}

void AnimsWidget::createAnim()
{
    Anim *anim = this->animFromUser();
    if(!anim)
        return;

    CreateAnimCommand *command = new CreateAnimCommand(m_model, anim);;
    qApp->undoStack()->push(command);

    // Set the new anim as current index
    int row = m_model->anims().count()-1;
    QModelIndex index = m_model->index(row, 0, QModelIndex());
    m_view->setCurrentIndex(index);
}

void AnimsWidget::editAnim()
{
    Anim *anim = static_cast<Anim *>(m_view->currentIndex().internalPointer());

    AnimDialog d(anim->name(), anim->frameCount(), anim->fps(), this);
    if(!d.exec())
        return;

    EditAnimCommand *command = new EditAnimCommand(anim, d.name(), d.frameCount(), d.fps());;
    qApp->undoStack()->push(command);
}

void AnimsWidget::removeAnim()
{
    Anim *anim = static_cast<Anim *>(m_view->currentIndex().internalPointer());
    DeleteAnimCommand *command = new DeleteAnimCommand(m_model, anim);
    qApp->undoStack()->push(command);
}

void AnimsWidget::updateActions()
{
    bool hasCurrent = m_view->currentIndex().isValid();
    m_editAction->setEnabled(hasCurrent);
    m_removeAction->setEnabled(hasCurrent);
}
