#pragma once

#include "ui_authenticator.h"

#include <qapplication>
#include <qmainwindow.h>
#include <qmovie>
#include <qpushbutton>

class Authenticator : public QMainWindow {
    Q_OBJECT

  public:
    Authenticator(QMainWindow *parent = nullptr, int type = 0);
    ~Authenticator();

    /// @brief 确定按钮点击事件
    void confirmBtnClicked();

    /// @brief 取消按钮点击事件
    void cancelBtnClicked();

  signals:
    void confirmDeleteOrder(int type, const std::string &password);
    void cancelDeleteOrder();

  private:
    void init_window();

    void init_signals_slots();

  private:
    Ui_Authenticator *ui_;
    int               type_;
};