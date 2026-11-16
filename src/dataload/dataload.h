#pragma once

#include "dataload_order_info.h"

#include <memory>
#include <zel/myorm.h>

class Dataload {

  public:
    Dataload(const std::shared_ptr<zel::myorm::Database> &db, const std::string &order_no);
    ~Dataload();

    bool deleteOrder();

  private:
    bool order_info();

    bool delete_record();
    bool delete_production_data();
    bool delete_order_data();

  private:
    std::shared_ptr<zel::myorm::Database> db_;
    DataloadOrderInfo                     order_info_;
};