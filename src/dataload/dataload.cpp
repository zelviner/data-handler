#include "dataload.h"
#include "model/xh_order_list.hpp"
#include "model/xh_datatool_record.hpp"

#include <zel/core.h>

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

bool Dataload::order_info() {
    XhOrderList xh_order_list(*db_);
    xh_order_list.where("xh_order_name", order_info_.order_no);
    auto dpo_all = xh_order_list.all();
    if (dpo_all.empty() || dpo_all.size() > 1) {
        log_error("table dms_product_orders has no or more than one record, order: %s", order_info_.order_no.c_str());
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
        auto filename = one("xh_order_filename").asString();
        auto table_name = filename.substr(0, filename.length() -1);
        printf("table_name: %s\n" ,table_name.c_str());
    }

    return true;
}

bool Dataload::delete_record() {
    // std::string order_no = String::toLower(order_info_.order_no);

    // // delete dms_remake_record table
    // DmsRemakeRecord dms_remake_record(*db_, order_no, "ID");
    // std::string     sql = "DROP TABLE IF EXISTS `" + dms_remake_record.table() + "`";

    // // delete dms_record_logs table
    // DmsRecordLogs dms_record_logs(*db_, order_no, "ID");
    // sql = "DROP TABLE IF EXISTS `" + dms_record_logs.table() + "`";
    // db_->execute(sql);

    // // delete dms_deleterecord records
    // DmsDeleteRecord(*db_).where("Order", order_info_.order_id).remove();

    // // delete dms_bindrecord records
    // DmsBindRecord(*db_)
    //     .where("Perso", order_info_.perso_script_path)
    //     .where("Verify", order_info_.verify_script_path)
    //     .where("Clear", order_info_.clear_script_path)
    //     .remove();

    // // delete dms_processrecord records
    // DmsProcessRecord(*db_).where("Product", order_info_.order_no).remove();

    return true;
}

bool Dataload::delete_production_data() {
    // std::string order_no = String::toLower(order_info_.order_no);

    // // delete dms_remake table
    // DmsRemake   dms_remake(*db_, order_no, "ID");
    // std::string sql = "DROP TABLE IF EXISTS `" + dms_remake.table() + "`";
    // db_->execute(sql);

    // // delete dms_remake_file records
    // DmsRemakeFile(*db_).where("Order", order_info_.order_id).remove();

    // // delete dms_tasklist records
    // DmsTaskList(*db_).where("Order", order_info_.order_id).remove();

    return true;
}

bool Dataload::delete_order_data() {
    // std::string data_table_name = String::toLower(order_info_.perso_data_table_name);

    // // delete perso_data table
    // DmsPersoData dms_perso_data(*db_, data_table_name, "ID");
    // std::string  sql = "DROP TABLE IF EXISTS `" + dms_perso_data.table() + "`";
    // db_->execute(sql);

    // // delete dms_batch_file records
    // DmsBatchFiles(*db_).where("Batch", order_info_.batch_list_id).remove();

    // // delete dms_batch_list records
    // DmsBatchList(*db_).where("ID", order_info_.batch_list_id).remove();

    // // delete dms_orderconf records
    // DmsOrderConf(*db_).where("Order", order_info_.order_id).remove();

    // // delete dms_productorders records
    // DmsProductOrders(*db_).where("Code", order_info_.order_no).remove();

    return true;
}