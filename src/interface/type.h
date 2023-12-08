#ifndef TYPE_H
#define TYPE_H

#include <QWidget>
#include <QCheckBox>

namespace Ui
{
    class Type;
}

class Type : public QWidget
{
    Q_OBJECT

public:
    explicit Type(QWidget *parent = nullptr);
    ~Type();

signals:
    void typesSelected(const QStringList &types);

private slots:
    void on_okButton_clicked();

private:
    Ui::Type *ui;
    QList<QCheckBox *> typeCheckBoxes;
};

#endif // TYPE_H
