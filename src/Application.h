#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QUndoStack>

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QCoreApplication::instance()))

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int argc, char **argv);

    QUndoStack *undoStack();

private:
    QUndoStack *m_undoStack;
};

#endif // APPLICATION_H
