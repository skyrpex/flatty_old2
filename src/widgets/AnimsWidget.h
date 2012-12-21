#ifndef ANIMSWIDGET_H
#define ANIMSWIDGET_H

#include <QWidget>

class AnimModel;
class Anim;
class QListView;
class QModelIndex;
class QAction;

class AnimsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnimsWidget(AnimModel *model, QWidget *parent = 0);

public slots:
    void setCurrentAnim(Anim *anim);

signals:
    void currentAnimChanged(Anim *anim);

private slots:
    void createAnim();
    void removeAnim();
    void updateActions();
    void onCurrentRowChanged(const QModelIndex &index);

private:
    Anim *animFromUser();

    AnimModel *m_model;
    QListView *m_view;
    QAction *m_createAction;
    QAction *m_removeAction;
};

#endif // ANIMSWIDGET_H
