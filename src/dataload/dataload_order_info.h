#pragma once

#include <string>
#include <vector>

struct DataloadOrderInfo {
    std::string              order_no;         // 订单号
    int                      order_id;         // 订单ID
    std::vector<std::string> data_table_names; // 数据表名列表
};