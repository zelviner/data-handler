#include "dataload.h"
#include "model/xh_order_list.hpp"
#include "model/xh_datatool_record.hpp"

#include <zel/core.h>
#include <fmt/core.h>

Dataload::Dataload(const std::shared_ptr<zel::myorm::Database> &db, const std::string &order_no)
    : db_(db) {
    order_info_.order_no = order_no;
    order_info();
}

Dataload::~Dataload() {}

bool Dataload::deleteOrder() {
    // delete about record table and records
    if (!delete_record()) {
        return false;
    }

    // delete about production table and records
    if (!delete_production_data()) {
        return false;
    }

    // delete about order data table and records
    if (!delete_order_data()) {
        return false;
    }

    return true;
}

void Dataload::generatingFinancePrintRecords(const std::string &output_file) {
    try {
        zel::fs::File print_file(output_file);
        std::string   header = "SN,";
        for (auto &field : order_info_.data_table_fields) {
            header += field + ",";
        }
        header += "Filename\n";

        std::string content = header;
        int         sn      = 1;
        for (auto &table_name : order_info_.data_table_names) {
            auto        sql     = "SELECT " + header.substr(3, header.length() - 13) + " FROM `" + table_name + "`";
            auto        results = db_->query(sql);
            std::string datas   = "";
            for (auto &one : results) {
                std::string data = fmt::format("{:06d},", sn);
                for (auto &field : order_info_.data_table_fields) {
                    data += one.find(field)->second.asString() + ",";
                }
                data += table_name + "\n";
                datas += data;
                sn++;
            }
            content += datas;
        }
        print_file.write(content);
    } catch (const std::exception &e) {
        log_error("generating finance print records failed, error: ", e.what());
    }
}

bool Dataload::order_info() {
    XhOrderList xh_order_list(*db_);
    xh_order_list.where("xh_order_number", order_info_.order_no);
    auto dpo_all = xh_order_list.all();
    if (dpo_all.empty() || dpo_all.size() > 1) {
        log_error("table xh_order_list has no or more than one record, order: %s", order_info_.order_no.c_str());
        return false;
    }

    order_info_.order_id = dpo_all[0]("RID").asInt();

    XhDatatoolRecord xh_datatool_record(*db_);
    xh_datatool_record.where("RID", order_info_.order_id);
    auto doc_all = xh_datatool_record.all();
    if (doc_all.empty()) {
        log_error("table xh_datatool_record has no record, Order: %d", order_info_.order_id);
        return false;
    }

    for (auto &one : doc_all) {
        auto filename   = one("xh_order_filename").asString();
        auto table_name = filename.substr(0, filename.length() - 4);
        order_info_.data_table_names.push_back(String::toLower(table_name));
    }

    auto sql     = "DESCRIBE `" + order_info_.data_table_names[0] + "`";
    auto results = db_->query(sql);
    for (auto &one : results) {
        auto field = one.find("Field")->second.asString();
        if (field.find("M_") == std::string::npos && field.find("Complete") == std::string::npos && field.find("WriteNum") == std::string::npos &&
            field.find("IC") == std::string::npos && field.find("CRC") == std::string::npos) {
            order_info_.data_table_fields.push_back(field);
        }
    }

    return true;
}

bool Dataload::delete_production_data() {
    try {
        for (auto table_name : order_info_.data_table_names) {
            std::string sql = "DROP TABLE IF EXISTS `" + table_name + "`";
            db_->execute(sql);
        }
        return true;
    } catch (const std::exception &e) {
        return false;
    }
}

bool Dataload::delete_record() {
    try {
        XhDatatoolRecord xh_datatool_record(*db_);
        xh_datatool_record.where("RID", order_info_.order_id).remove();
        return true;
    } catch (const std::exception &e) {
        return false;
    }
}

bool Dataload::delete_order_data() {
    try {
        XhOrderList(*db_).where("xh_order_number", order_info_.order_no).remove();
        return true;
    } catch (const std::exception &e) {
        return false;
    }
}