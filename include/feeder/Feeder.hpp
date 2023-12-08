#pragma once

#include <iostream>
#include <QObject>

class Feeder : public QObject {
    Q_OBJECT

public:
    Feeder();
    ~Feeder();

public slots:
    void feedCard();
    void stop();

private:
    const int SERVO;
    const int pwmDispense;
    const int pwmNeutral;
};
