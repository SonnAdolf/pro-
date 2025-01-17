#include "sqlite_utils.h"


int sqlite_utils::sqlite_conn()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("data.db");
    if (!database.open())
    {
        return -1;
    }
    return 0;
}

void sqlite_utils::sqlite_close()
{
    database.close();
}

bool sqlite_utils::create_tables()
{
    QString str_sql = "create table if not exists  pro(id integer  primary key autoincrement,writing_pro integer,reading_pro integer,art_learning_pro integer,computer_science_tech_pro integer,total_pro integer,note varchar(500),date date NOT NULL); ";
    QSqlQuery sql_query;
    sql_query.prepare(str_sql);
    qDebug() << str_sql;
    if(!sql_query.exec(str_sql))
    {
        return false;
    }
    return true;
}

bool sqlite_utils::add_pro(pro pro, QString date)
{
    QString sql = "INSERT INTO pro(writing_pro, reading_pro, art_learning_pro, computer_science_tech_pro, total_pro, note, date) VALUES ('";
    sql.append(pro.get_writing_pro_str()).append("','").append(pro.get_reading_pro_str()).append("','").append(pro.get_art_learning_pro_str());
    sql.append("','").append(pro.get_computer_learning_pro_str()).append("','").append(pro.get_total_pro_str()).append("','");
    sql.append(pro.get_note()).append("','").append(date).append("');");
    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    return true;
}

bool sqlite_utils::check_if_pro_of_date_exist(QString date)
{
    QString sql = "select count(*) from pro where date='";
    sql.append(date);
    sql.append("';");
    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    if(sql_query.next())
     {
         int num = sql_query.value(0).toInt();
         qDebug()<<QString("num:%1").arg(num);
         if (num > 0)
         {
             return true;
         }
     }
    return false;
}


bool sqlite_utils::update_pro(pro pro, QString date)
{
    QString sql = "update pro set writing_pro='";
    sql.append(pro.get_writing_pro_str()).append("',reading_pro='").append(pro.get_reading_pro_str()).append("',art_learning_pro='")
        .append(pro.get_art_learning_pro_str()).append("',computer_science_tech_pro='")
        .append(pro.get_computer_learning_pro_str()).append("',total_pro='").append(pro.get_total_pro_str()).append("',note='")
        .append(pro.get_note()).append("' where date='");
    sql.append(date);
    sql.append("';");

    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    return true;
}


bool sqlite_utils::get_pro(QString date, pro* selpro)
{
    float  writing_pro= 0;
    float  reading_pro = 0;
    float  art_learning_pro = 0;
    float  computer_science_tech_pro = 0;
    QString  note = "";
    QString sql = "select writing_pro,reading_pro,art_learning_pro,computer_science_tech_pro,note from pro where date='";
    sql.append(date).append("';");
    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    if(sql_query.next())
    {
        if (sql_query.value(0) == NULL || sql_query.value(0).toString().compare("") == 0)
        {
            writing_pro = 0;
        }
        else
        {
            writing_pro = sql_query.value(0).toFloat();
        }
        if (sql_query.value(1) == NULL || sql_query.value(1).toString().compare("") == 0)
        {
            reading_pro = 0;
        }
        else
        {
            reading_pro = sql_query.value(1).toFloat();
        }
        if (sql_query.value(2) == NULL || sql_query.value(2).toString().compare("") == 0)
        {
            art_learning_pro = 0;
        }
        else
        {
            art_learning_pro = sql_query.value(2).toFloat();
        }
        if (sql_query.value(3) == NULL || sql_query.value(3).toString().compare("") == 0)
        {
            computer_science_tech_pro = 0;
        }
        else
        {
            computer_science_tech_pro = sql_query.value(3).toFloat();
        }
        if (sql_query.value(4) == NULL || sql_query.value(4).toString().compare("") == 0)
        {
            note = "";
        }
        else
        {
            note = sql_query.value(4).toString();
        }
        pro new_pro(writing_pro,reading_pro,art_learning_pro,computer_science_tech_pro,note);
        *selpro = new_pro;
        return true;
    }
    return false;
}


bool sqlite_utils::count_pro_by_month(int* const num, const int year, const int month)
{
    QString year_str = "";
    QString month_str = "";
    year_str = QString("%1").arg(year);
    if (month < 10)
    {
        month_str = "0";
        month_str.append(QString("%1").arg(month));
    }
    else
    {
        month_str = QString("%1").arg(month);
    }
    QString sql = "select count(*) from pro where strftime('%Y-%m',date)='";
    sql.append(year_str).append("-").append(month_str).append("';");

    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    if(sql_query.next())
    {
        if (sql_query.value(0) == NULL || sql_query.value(0).toString().compare("") == 0)
        {
            *num = 0;
        }
        else
        {
            *num = sql_query.value(0).toInt();
        }
        return true;
    }
    return false;
}


bool sqlite_utils::count_pro_by_year(int* const num, const int year) {
    QString year_str = "";
    year_str = QString("%1").arg(year);
    QString sql = "select count(*) from pro where strftime('%Y',date)='";
    sql.append(year_str).append("';");

    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    if(sql_query.next())
    {
        if (sql_query.value(0) == NULL || sql_query.value(0).toString().compare("") == 0)
        {
            *num = 0;
        }
        else
        {
            *num = sql_query.value(0).toInt();
        }
        return true;
    }
    return false;
}


bool sqlite_utils::count_pro(int* const num) {
    QString sql = "select count(*) from pro;";
    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    if(sql_query.next())
    {
        if (sql_query.value(0) == NULL || sql_query.value(0).toString().compare("") == 0)
        {
            *num = 0;
        }
        else
        {
            *num = sql_query.value(0).toInt();
        }
        return true;
    }
    return false;
}


bool sqlite_utils::get_pro_sum_by_month(float* const writing_pro_sum, float* const reading_pro_sum,
    float* const art_learning_pro_sum, float* const computer_learning_pro_sum, float* const total_pro_sum, const int year, const int month) {
    QString year_str = "";
    QString month_str = "";
    year_str = QString("%1").arg(year);
    if (month < 10)
    {
        month_str = "0";
        month_str.append(QString("%1").arg(month));
    }
    else
    {
        month_str = QString("%1").arg(month);
    }

    QString sql = "select sum(writing_pro),sum(reading_pro),sum(art_learning_pro),sum(computer_science_tech_pro),sum(total_pro) from pro where strftime('%Y-%m',date)='";
    sql.append(year_str).append("-").append(month_str).append("';");

    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    if(sql_query.next())
    {
        if (sql_query.value(0) == NULL || sql_query.value(0).toString().compare("") == 0)
        {
            *writing_pro_sum = 0;
        }
        else
        {
            *writing_pro_sum = sql_query.value(0).toFloat();
        }
        if (sql_query.value(1) == NULL || sql_query.value(1).toString().compare("") == 0)
        {
            *reading_pro_sum = 0;
        }
        else
        {
            *reading_pro_sum = sql_query.value(1).toFloat();
        }
        if (sql_query.value(2) == NULL || sql_query.value(2).toString().compare("") == 0)
        {
            *art_learning_pro_sum = 0;
        }
        else
        {
            *art_learning_pro_sum = sql_query.value(2).toFloat();
        }
        if (sql_query.value(3) == NULL || sql_query.value(3).toString().compare("") == 0)
        {
            *computer_learning_pro_sum = 0;
        }
        else
        {
            *computer_learning_pro_sum = sql_query.value(3).toFloat();
        }
        if (sql_query.value(4) == NULL || sql_query.value(4).toString().compare("") == 0)
        {
            *total_pro_sum = 0;
        }
        else
        {
            *total_pro_sum = sql_query.value(4).toFloat();
        }
        return true;
    }
    return false;
}


bool sqlite_utils::get_pro_sum_by_year(float* const writing_pro_sum, float* const reading_pro_sum,
    float* const art_learning_pro_sum, float* const computer_learning_pro_sum, float* const total_pro_sum, const int year)
{
    QString year_str = "";
    year_str = QString("%1").arg(year);

    QString sql = "select sum(writing_pro),sum(reading_pro),sum(art_learning_pro),sum(computer_science_tech_pro),sum(total_pro) from pro where strftime('%Y',date)='";
    sql.append(year_str).append("';");

    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    if(sql_query.next())
    {
        if (sql_query.value(0) == NULL || sql_query.value(0).toString().compare("") == 0)
        {
            *writing_pro_sum = 0;
        }
        else
        {
            *writing_pro_sum = sql_query.value(0).toFloat();
        }
        if (sql_query.value(1) == NULL || sql_query.value(1).toString().compare("") == 0)
        {
            *reading_pro_sum = 0;
        }
        else
        {
            *reading_pro_sum = sql_query.value(1).toFloat();
        }
        if (sql_query.value(2) == NULL || sql_query.value(2).toString().compare("") == 0)
        {
            *art_learning_pro_sum = 0;
        }
        else
        {
            *art_learning_pro_sum = sql_query.value(2).toFloat();
        }
        if (sql_query.value(3) == NULL || sql_query.value(3).toString().compare("") == 0)
        {
            *computer_learning_pro_sum = 0;
        }
        else
        {
            *computer_learning_pro_sum = sql_query.value(3).toFloat();
        }
        if (sql_query.value(4) == NULL || sql_query.value(4).toString().compare("") == 0)
        {
            *total_pro_sum = 0;
        }
        else
        {
            *total_pro_sum = sql_query.value(4).toFloat();
        }
        return true;
    }
    return false;
}


bool sqlite_utils::get_pro_sum(float* const writing_pro_sum, float* const reading_pro_sum,
    float* const art_learning_pro_sum, float* const computer_learning_pro_sum, float* const total_pro_sum)
{
    QString sql = "select sum(writing_pro),sum(reading_pro),sum(art_learning_pro),sum(computer_science_tech_pro),sum(total_pro) from pro;";

    qDebug() << sql;
    QSqlQuery sql_query;
    sql_query.prepare(sql);
    if(!sql_query.exec())
    {
        return false;
    }
    if(sql_query.next())
    {
        if (sql_query.value(0) == NULL || sql_query.value(0).toString().compare("") == 0)
        {
            *writing_pro_sum = 0;
        }
        else
        {
            *writing_pro_sum = sql_query.value(0).toFloat();
        }
        if (sql_query.value(1) == NULL || sql_query.value(1).toString().compare("") == 0)
        {
            *reading_pro_sum = 0;
        }
        else
        {
            *reading_pro_sum = sql_query.value(1).toFloat();
        }
        if (sql_query.value(2) == NULL || sql_query.value(2).toString().compare("") == 0)
        {
            *art_learning_pro_sum = 0;
        }
        else
        {
            *art_learning_pro_sum = sql_query.value(2).toFloat();
        }
        if (sql_query.value(3) == NULL || sql_query.value(3).toString().compare("") == 0)
        {
            *computer_learning_pro_sum = 0;
        }
        else
        {
            *computer_learning_pro_sum = sql_query.value(3).toFloat();
        }
        if (sql_query.value(4) == NULL || sql_query.value(4).toString().compare("") == 0)
        {
            *total_pro_sum = 0;
        }
        else
        {
            *total_pro_sum = sql_query.value(4).toFloat();
        }
        return true;
    }
    return false;
}
