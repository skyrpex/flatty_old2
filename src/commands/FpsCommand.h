#ifndef FPSCOMMAND_H
#define FPSCOMMAND_H

#include <QUndoCommand>

class Anim;
class AnimsWidget;

class FpsCommand : public QUndoCommand
{
public:
    FpsCommand(Anim *anim, int fps, AnimsWidget *widget);

    void undo();
    void redo();

private:
    AnimsWidget *m_widget;
    Anim *m_anim;
    int m_index;
    int m_fps;
    int m_oldFps;
};

#endif // FPSCOMMAND_H
