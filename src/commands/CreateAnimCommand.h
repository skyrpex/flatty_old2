#ifndef CREATEANIMCOMMAND_H
#define CREATEANIMCOMMAND_H

#include <QUndoCommand>

class AnimModel;
class Anim;
class AnimsWidget;

class CreateAnimCommand : public QUndoCommand
{
public:
    explicit CreateAnimCommand(AnimModel *model, Anim *anim, AnimsWidget *widget);

    void undo();
    void redo();

private:
    AnimsWidget *m_widget;
    AnimModel *m_model;
    Anim *m_anim;
};

#endif // CREATEANIMCOMMAND_H
