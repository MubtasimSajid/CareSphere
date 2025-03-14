#ifndef PATIENTFEED_H
#define PATIENTFEED_H

#include <QWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>

namespace Ui {
class patientfeed;
}

class patientfeed : public QWidget
{
    Q_OBJECT

public:
    explicit patientfeed(QWidget *parent = nullptr);
    ~patientfeed();

private slots:
    void on_logOutButton_clicked();
    void on_feedToProfileButton_clicked();
    void addBulletPoint();
    void editBulletPoint();
    void deleteBulletPoint();
    void showContextMenu(const QPoint &pos);

private:
    Ui::patientfeed *ui;
};

#endif
