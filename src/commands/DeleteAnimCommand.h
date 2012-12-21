#ifndef DELETEANIMCOMMAND_H
#define DELETEANIMCOMMAND_H

#include <QUndoCommand>

class AnimModel;
class Anim;
class AnimsWidget;

class DeleteAnimCommand : public QUndoCommand
{
public:
    DeleteAnimCommand(AnimModel *model, Anim *anim, AnimsWidget *widget);

    void undo();
    void redo();

private:
    AnimsWidget *m_widget;
    AnimModel *m_model;
    Anim *m_anim;
};

#endif // DELETEANIMCOMMAND_H
