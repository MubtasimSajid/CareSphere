#include "patientfeed.h"
#include "ui_patientfeed.h"
#include "login.h"
#include "patientprofile.h"
#include <qmenu.h>

patientfeed::patientfeed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientfeed)
{
    ui->setupUi(this);
    connect(ui->notesPlusButton, &QPushButton::clicked, this, &patientfeed::addBulletPoint);
    ui->notesListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->notesListWidget, &QListWidget::customContextMenuRequested, this, &patientfeed::showContextMenu);
}

patientfeed::~patientfeed()
{
    delete ui;
}

void patientfeed::on_logOutButton_clicked()
{
    Login *loginWindow = new Login();
    loginWindow->show();
    this->close();
}


void patientfeed::on_feedToProfileButton_clicked()
{
    patientprofile *patientProfileWindow = new patientprofile();
    patientProfileWindow->show();
    this->close();
}

void patientfeed::addBulletPoint()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Add Notes", "Enter text:", QLineEdit::Normal, "", &ok);

    if (ok && !text.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem("• " + text, ui->notesListWidget);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}

void patientfeed::deleteBulletPoint()
{
    QListWidgetItem *item = ui->notesListWidget->currentItem();
    if (item) {
        delete item;
    }
}

void patientfeed::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->notesListWidget->viewport()->mapToGlobal(pos);
    QMenu menu;
    QAction *editAction = menu.addAction("Edit");
    QAction *deleteAction = menu.addAction("Delete");

    QAction *selectedAction = menu.exec(globalPos);
    if (selectedAction == editAction) {
        editBulletPoint();
    } else if (selectedAction == deleteAction) {
        deleteBulletPoint();
    }
}

void patientfeed::editBulletPoint()
{
    QListWidgetItem *item = ui->notesListWidget->currentItem();
    if (!item) return;

    bool ok;
    QString text = QInputDialog::getText(this, "Edit Notes", "Edit text:", QLineEdit::Normal, item->text().mid(2), &ok);

    if (ok && !text.isEmpty()) {
        item->setText("• " + text);
    }
}
