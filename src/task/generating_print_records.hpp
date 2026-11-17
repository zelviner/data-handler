#pragma once

#include "dataload/dataload.h"

#include <qobjectdefs.h>
#include <qcoreapplication>
#include <qthread>
#include <zel/core.h>
#include <zel/myorm.h>

class GeneratingPrintRecords : public QThread {
    Q_OBJECT

  public:
    GeneratingPrintRecords(const std::shared_ptr<zel::myorm::Database> &finance_db, const std::string &order_no, const std::string &generate_path)
        : finance_db_(finance_db)
        , order_no_(order_no)
        , generate_path_(generate_path) {}
    ~GeneratingPrintRecords() {}

  signals:
    void success();
    void failure();

  protected:
    void run() override {
        Dataload dataload(finance_db_, order_no_);
        dataload.generatingFinancePrintRecords(generate_path_);

        emit success();
    }

  private:
    std::shared_ptr<zel::myorm::Database> finance_db_;
    std::string                           order_no_;
    std::string                           generate_path_;
};