/*
管理数据库的DBMS
sqlite
*/

#ifndef SQLSPLITE_H
#define SQLSPLITE_H

//#include <sqlite3.h>
#include <QtSql\QSqlDatabase>
#include <QtSql\QSqlQuery>
#include <QtSql\QSqlError>
#include <QtDebug>
#include <QtSql\QSqlDriver>
#include <QtSql\QSqlRecord>

#include <vector>

#include "datastructcommon.h"
#include "mxnet_inference.h"
#include "common.h"
using namespace std;

class SqlSplite
{

public:
	SqlSplite();
	~SqlSplite();

	//插入：
	void insert(NDArray featureMap, struct InformationParam &infoParam);
	// 插入活动表
	bool insertActivity(const QString activity_name);
	//查询:
	vector<InformationParam> query();
	
	void close();
	// 活动表
	vector<QString> selectActivity();
	// 插入考勤记录表
	bool insertRecord(int recordId,QString no, int id);
	int getRecordId();
private:
	//扩容
	void expand();

	//创建表
	void createTable();


	//数据库	
	QSqlDatabase db;

};

#endif // !
