#ifndef DBTOOLS_H_H_HEAD__FILE__
#define DBTOOLS_H_H_HEAD__FILE__
#pragma once
#include <memory>
#include <qstring.h>

#include "../nameSpace/cylDB.h"
namespace cylDB {
	class QTEXTENDDB_EXPORT DBTools {
	public: // - 类型
		/// <summary>
		/// 支持类型
		/// </summary>
		enum Super_DB_Type {
			SQLite // sqlite
		};
	public: // - 友类
	public: // - 静态功能
		/// <summary>
		/// 链接一个数据库
		/// </summary>
		/// <param name="link">连接路径</param>
		/// <param name="db_type">数据库类型</param>
		/// <returns>数据库指针，失败返回 nullptr</returns>
		static DB_Shared linkDB( const QString &link, Super_DB_Type db_type );
		/// <summary>
		/// 链接一个数据库
		/// </summary>
		/// <param name="link">连接路径</param>
		/// <returns>数据库指针，失败返回 nullptr</returns>
		static DB_Shared linkDB( const QString &link ) {
			return linkDB( link, SQLite );
		}
	};
}


#endif // DBTOOLS_H_H_HEAD__FILE__
